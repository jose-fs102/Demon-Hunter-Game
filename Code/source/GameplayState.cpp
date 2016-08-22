/***********************************************************************\
|	File:		GameplayState.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	GameplayState class initializes & runs the game logic
************************************************************************/

// Class Headers
#include "GameplayState.h"
#include "OptionsState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "MessageID.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "BitmapFont.h"
#include "Enemy.h"
#include "Projectile.h"
#include "CreateProjectile.h"
#include "DestroyEntityMessage.h"
#include "LoseGameState.h"
#include "Boss.h"
#include "CreateBossProjectile.h"
#include "WinGameState.h"
#include "BossProjectile.h"

// Wrappers
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"

// C++ Headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <fstream>

/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void GameplayState::Enter(void)	/*override*/
{
	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	// Load Images
	GamePlay1_BG_IMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Gameplay1_BG_IMG.png");
	PlayerIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Player_IMG.png");
	ProjectileIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Projectile_IMG.png");
	BossProjectileIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_BossProjectile_IMG.png");
	EnemyIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Enemy_IMG.png");
	BossIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Boss_IMG.png");
	HealthBarGreenIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_HealthBar_Green_HUD_IMG.png");
	HealthBarYellowIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_HealthBar_Yellow_HUD_IMG.png");
	HealthBarRedIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_HealthBar_Red_HUD_IMG.png");

	// Load Audio
	GamePlayBackGround_MUS = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_GamePlay_BG_MUS.xwm");
	Victory_MUS = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_VictoryMUS.xwm");
	CursorSelectionFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_CursorSelectionFX.wav");
	MenuSelectionFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_MenuSelectionFX.wav");
	ImpactFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_ImpactFX.wav");

	// Create a player, add to manager in bucket #0
	newPlayer = CreatePlayer();
	m_pEntities->AddEntity(newPlayer, 0);
	newPlayer->Release();

	// Set Parameters
	IsPaused = false;
	m_nCursor = 0;
	spawnCount = 0;
	enemyOnScreen = false;
	bossOnScreen = false;
	bossFifty = false;

	// Read in volume levels 
	std::fstream in;

	in.open("VolumeSettings.txt", std::ios_base::in);

	if (in.is_open())
	{
		int mVolume, xVolume;

		in.ignore(INT_MAX, '\n');
		in.ignore(INT_MAX, '=');

		in >> mVolume;

		in.ignore(INT_MAX, '\n');
		in.ignore(INT_MAX, '=');

		in >> xVolume;

		if (mVolume > 100)
			mVolume = 100;
		else if (mVolume < 0)
			mVolume = 0;
		else if (mVolume % 10 != 0)
			mVolume = 100;

		OptionsState::GetInstance()->SetMasterVolume(mVolume);

		if (xVolume > 100)
			xVolume = 100;
		else if (xVolume < 0)
			xVolume = 0;
		else if (xVolume % 10 != 0)
			xVolume = 100;

		OptionsState::GetInstance()->SetFXVolume(xVolume);

		in.close();
	}
	else
	{
		// If file fails to open, 
		// default volume levels to 100
		OptionsState::GetInstance()->SetMasterVolume(100);
		OptionsState::GetInstance()->SetFXVolume(100);
	}

	// Set the volume
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, OptionsState::GetInstance()->GetMasterVolume());
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, OptionsState::GetInstance()->GetFXVolume());

	// Play the Background Music
	SGD::AudioManager::GetInstance()->PlayAudio(GamePlayBackGround_MUS, true);
}

/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)	/*override*/
{
	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	// Unload Images
	SGD::GraphicsManager::GetInstance()->UnloadTexture(GamePlay1_BG_IMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(PlayerIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(ProjectileIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(BossProjectileIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(EnemyIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(BossIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(HealthBarGreenIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(HealthBarYellowIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(HealthBarRedIMG);

	// Unload Audio
	SGD::AudioManager::GetInstance()->UnloadAudio(GamePlayBackGround_MUS);
	SGD::AudioManager::GetInstance()->UnloadAudio(Victory_MUS);
	SGD::AudioManager::GetInstance()->UnloadAudio(CursorSelectionFX);
	SGD::AudioManager::GetInstance()->UnloadAudio(MenuSelectionFX);
	SGD::AudioManager::GetInstance()->UnloadAudio(ImpactFX);

	// Terminate & deallocate the Managers
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();

	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();
}

/**************************************************************/
// Update
//	- handle input & update entities
/*virtual*/ bool GameplayState::Update(float elapsedTime)	/*override*/
{

	if (elapsedTime == 10.0)
		bossOnScreen = false;

	// Used for input
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Window width
	float width = Game::GetInstance()->GetScreenWidth();

	// Window Height
	float height = Game::GetInstance()->GetScreenHeight();

	Player* P;
	P = dynamic_cast<Player*> (newPlayer);

	if (P->GetHealth() <= 0)
	{
		Game::GetInstance()->ChangeState(LoseGameState::GetInstance());

		return true;
	}
	if (!bossOnScreen && P->GetScore() == 100)
	{
		newBoss = CreateBoss();
		m_pEntities->AddEntity(newBoss, 1);
		newBoss->Release();
		bossOnScreen = true;
	}
	if (bossOnScreen)
	{
		Boss* B;
		B = dynamic_cast<Boss*> (newBoss);

		if (B->GetBossHealth() <= 0)
		{
			Game::GetInstance()->ChangeState(WinGameState::GetInstance());

			return true;
		}
		if (!bossFifty && B->GetBossHealth() == 50)
		{
			for (unsigned int i = 0; i < 5; i++)
			{
				newEnemy = CreateEnemy();
				m_pEntities->AddEntity(newEnemy, 1);
				newEnemy->Release();
			}

			bossFifty = true;
		}

		switch (B->GetBossHealth())
		{
		case 0:
			pFont->Draw("Boss Health: 100", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 10:
			pFont->Draw("Boss Health: 10", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 20:
			pFont->Draw("Boss Health: 20", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 30:
			pFont->Draw("Boss Health: 30", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 40:
			pFont->Draw("Boss Health: 40", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 50:
			pFont->Draw("Boss Health: 50", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 60:
			pFont->Draw("Boss Health: 60", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 70:
			pFont->Draw("Boss Health: 70", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 80:
			pFont->Draw("Boss Health: 80", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 90:
			pFont->Draw("Boss Health: 90", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		case 100:
			pFont->Draw("Boss Health: 100", { width - 265, height - 25 },
				0.5f, { 255, 0, 0 });
			break;
		}
	}
	if (!enemyOnScreen && !bossOnScreen && spawnCount < 5)
	{
		// Create an Enemy, add to manager in bucket #1
		newEnemy = CreateEnemy();
		m_pEntities->AddEntity(newEnemy, 1);
		newEnemy->Release();
		enemyOnScreen = true;
	}

	// Press Escape to pause game
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		IsPaused = !IsPaused;
	}

	// If game is paused, sets up cursor for pause menu selection /////////////////////////
	if (IsPaused)
	{
		if (pInput->IsKeyPressed(SGD::Key::Up) == true)
		{
			m_nCursor--;

			SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

			if (m_nCursor < 0)
				m_nCursor = 1;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Down) == true)
		{
			m_nCursor++;

			SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

			if (m_nCursor > 1)
				m_nCursor = 0;
		}

		if (pInput->IsKeyPressed(SGD::Key::Enter) == true && m_nCursor == 0)
		{
			IsPaused = !IsPaused;
			SGD::AudioManager::GetInstance()->PlayAudio(MenuSelectionFX);
		}
		else if (pInput->IsKeyPressed(SGD::Key::Enter) == true && m_nCursor == 1)
		{
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////

	// If game is not paused, update the entities
	if (!IsPaused)
	{
		// Update the entities
		m_pEntities->UpdateAll(elapsedTime);

		// Check collision between player and enemy
		m_pEntities->CheckCollisions(0, 1);

		// Check collision between enemy and projectile
		m_pEntities->CheckCollisions(1, 2);

		// Check collision between player and Boss projectile
		m_pEntities->CheckCollisions(0, 2);

		// Check collision between projectile and boss projectile
		m_pEntities->CheckCollisions(2, 2);

		// Process the events & messages
		SGD::EventManager::GetInstance()->Update();
		SGD::MessageManager::GetInstance()->Update();
	}

	return true;	// keep playing
}

/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(float elapsedTime)	/*override*/
{
	Player* P;
	P = dynamic_cast<Player*> (newPlayer);

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Window width
	float width = Game::GetInstance()->GetScreenWidth();

	// Window Height
	float height = Game::GetInstance()->GetScreenHeight();

	// Set a grey background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 84, 84, 84 });

	// Render the Background
	SGD::GraphicsManager::GetInstance()->DrawTexture(GamePlay1_BG_IMG, { 0.0f, 180.0f });

	switch (P->GetHealth())
	{
	case 0:
		pFont->Draw("Health: 0", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarRedIMG, { -150.0f, -100.0f });
		break;

	case 10:
		pFont->Draw("Health: 10", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarRedIMG, { -150.0f, -100.0f });
		break;

	case 20:
		pFont->Draw("Health: 20", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarRedIMG, { -150.0f, -100.0f });
		break;

	case 30:
		pFont->Draw("Health: 30", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarRedIMG, { -150.0f, -100.0f });
		break;

	case 40:
		pFont->Draw("Health: 40", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarYellowIMG, { -150.0f, -100.0f });
		break;

	case 50:
		pFont->Draw("Health: 50", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarYellowIMG, { -150.0f, -100.0f });
		break;

	case 60:
		pFont->Draw("Health: 60", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarYellowIMG, { -150.0f, -100.0f });
		break;

	case 70:
		pFont->Draw("Health: 70", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarGreenIMG, { -150.0f, -100.0f });
		break;

	case 80:
		pFont->Draw("Health: 80", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarGreenIMG, { -150.0f, -100.0f });
		break;

	case 90:
		pFont->Draw("Health: 90", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarGreenIMG, { -150.0f, -100.0f });
		break;

	case 100:
		pFont->Draw("Health: 100", { width - 200, height - 55 },
			0.5f, { 255, 0, 0 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(HealthBarGreenIMG, { -150.0f, -100.0f });
		break;
	}

	// Show the objective
	pFont->Draw("Objective - Defend yourself against the enemies!\nDont let your health drop to zero!", { 0.0f, height - 75 },
		0.5f, { 255, 0, 0 });

	switch (P->GetScore())
	{
	case 0:
		pFont->Draw("Score: 0", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 10:
		pFont->Draw("Score: 10", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 20:
		pFont->Draw("Score: 20", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 30:
		pFont->Draw("Score: 30", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 40:
		pFont->Draw("Score: 40", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 50:
		pFont->Draw("Score: 50", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 60:
		pFont->Draw("Score: 60", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 70:
		pFont->Draw("Score: 70", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 80:
		pFont->Draw("Score: 80", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 90:
		pFont->Draw("Score: 90", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 100:
		pFont->Draw("Score: 100", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 110:
		pFont->Draw("Score: 110", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 120:
		pFont->Draw("Score: 120", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 130:
		pFont->Draw("Score: 130", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 140:
		pFont->Draw("Score: 140", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;

	case 150:
		pFont->Draw("Score: 150", { width - 200, height - 75 },
			0.5f, { 255, 0, 0 });
		break;
	}

	// if game is not paused, update the entities
	if (!IsPaused)
	{
		// Update the entities
		m_pEntities->UpdateAll(elapsedTime);

		// Render the entities
		m_pEntities->RenderAll();
	}
	else
	{
		// Render the entities
		m_pEntities->RenderAll();

		// Display "Paused" screen w/ options
		pFont->Draw("Paused", { (width / 2) - 70, (height / 2) - 200 },
			1.0f, { 255, 255, 255 });

		pFont->Draw("Resume", { (width / 2) - 70, (height / 2) - 150 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Quit to main menu", { (width / 2) - 225, (height / 2) - 100 },
			1.0f, { 255, 0, 0 });

		switch (m_nCursor)
		{
		case 0:
			// Surround "Resume"
			pFont->Draw("(      )", { (width / 2) - 100, 235.0f + 100 * m_nCursor },
				1.0f, { 0, 0, 0 });
			break;

		case 1:
			// Surround "Quit to main menu"
			pFont->Draw("(                 )", { (width / 2) - 255, 255.0f + 25 * m_nCursor },
				1.0f, { 0, 0, 0 });
			break;
		}
	}
}

/**************************************************************/
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD
//	- does NOT have invoking object!!!
//	- must use singleton to access members
/*static*/ void GameplayState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_DESTROY_ENTITY:

		SGD::AudioManager::GetInstance()->PlayAudio(GameplayState::GetInstance()->ImpactFX);

		const DestroyEntityMessage* pDestroyMsg;
		pDestroyMsg = dynamic_cast<const DestroyEntityMessage*>(pMsg);

		assert(pDestroyMsg != nullptr
			&& "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

		Entity* ptr;
		ptr = pDestroyMsg->GetEntity();

		GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);

		break;

	case MessageID::MSG_CREATE_PROJECTILE:

		const CreateProjectile* ProjMSG;
		ProjMSG = dynamic_cast<const CreateProjectile*>(pMsg);

		assert(ProjMSG != nullptr
			&& "GameplayState::MessageProc - ProjMsg is NULL");

		Player* owner;
		owner = ProjMSG->GetOwner();

		Entity* Projectile;

		Projectile = GameplayState::GetInstance()->CreateProjectile1(owner);

		GameplayState::GetInstance()->m_pEntities->AddEntity(Projectile, 2);

		Projectile->Release();

		break;

	case MessageID::MSG_CREATE_BOSS_PROJECTILE:

		const CreateBossProjectile* BossMSG;
		BossMSG = dynamic_cast<const CreateBossProjectile*>(pMsg);

		assert(BossMSG != nullptr
			&& "GameplayState::MessageProc - BossMSG is NULL");

		Boss* BossOwner;
		BossOwner = BossMSG->GetBossOwner();

		Entity* BossProjectile;

		BossProjectile = GameplayState::GetInstance()->CreateBossProjectile1(BossOwner);

		GameplayState::GetInstance()->m_pEntities->AddEntity(BossProjectile, 2);

		BossProjectile->Release();

		break;
	}

	/* Restore previous warning levels */
#pragma warning( pop )
}

// CreatePlayer()
// - creates a player
// - sets its values and returns it
Entity* GameplayState::CreatePlayer(void) const
{
	Player* player = new Player;

	player->SetImage(PlayerIMG);
	player->SetHealth(100);
	player->SetScore(0);
	player->SetPosition({ ((Game::GetInstance()->GetScreenWidth() / 2) - 520),
		((Game::GetInstance()->GetScreenHeight() / 2)) + 200 });

	player->SetSize({ 32.0f, 32.0f });
	player->SetScore(0);
	player->SetAccel(5000.0f);
	player->SetSpeed(0.0f);

	return player;
}

// CreateEnemy()
// - create an enemy
// - set its values and return it
Entity* GameplayState::CreateEnemy(void) const
{
	Enemy* enemy = new Enemy;

	enemy->SetImage(EnemyIMG);
	enemy->SetPosition({ (enemy->RandomPosX() - 150),
		((Game::GetInstance()->GetScreenHeight() / 2)) + 215 });

	enemy->SetSize({ 32.0f, 32.0f });

	return enemy;
}

// CreateBoss()
// - create the boss character
// - set its values and return it
Entity* GameplayState::CreateBoss(void) const
{
	Boss* boss = new Boss;

	boss->SetImage(BossIMG);
	boss->SetPosition({ Game::GetInstance()->GetScreenWidth() - 150,
		(Game::GetInstance()->GetScreenHeight() / 2) + 175 });

	boss->SetSize({ 114.0f, 73.0f });
	boss->SetBossHealth(100);

	return boss;
}

// CreateProjectile()
// - Create a projectile
// - set its values and return it
Entity* GameplayState::CreateProjectile1(Player* ptr) const
{
	Projectile* newProjectile = new Projectile;

	newProjectile->SetImage(ProjectileIMG);
	newProjectile->SetSize({ 128.0f, 64.0f });
	newProjectile->SetOwner(ptr);
	newProjectile->SetPosition(ptr->GetPosition());

	if (ptr->GetDirection() == 1.0f)
	{
		SGD::Vector ProjectileVec = { 1.0f, 0.0f };

		if (ptr->GetSpeed() > 0)
			ProjectileVec *= 300 + ptr->GetSpeed();
		else if (ptr->GetSpeed() <= 0)
			ProjectileVec *= 300;

		newProjectile->SetVelocity(ProjectileVec);
	}
	else if (ptr->GetDirection() == -1.0f)
	{
		SGD::Vector ProjectileVec = { -1.0f, 0.0f };

		if (ptr->GetSpeed() > 0)
			ProjectileVec *= 300 + ptr->GetSpeed();
		else if (ptr->GetSpeed() <= 0)
			ProjectileVec *= 300;

		newProjectile->SetVelocity(ProjectileVec);
	}

	return newProjectile;
}

// CreateBossProjectile
// - Creates the boss's projectile 
// - set its values and return it
Entity* GameplayState::CreateBossProjectile1(Boss* ptr) const
{
	BossProjectile* bossProjectile = new BossProjectile;

	bossProjectile->SetImage(BossProjectileIMG);
	bossProjectile->SetSize({ 225.0f, 225.0f });
	bossProjectile->SetOwner(ptr);
	bossProjectile->SetPosition(ptr->GetPosition());

	if (ptr->GetDirection() == 1.0f)
	{
		SGD::Vector ProjectileVec = { 1.0f, 0.0f };

		ProjectileVec *= 300;

		bossProjectile->SetVelocity(ProjectileVec);
	}
	else if (ptr->GetDirection() == -1.0f)
	{
		SGD::Vector ProjectileVec = { -1.0f, 0.0f };

		ProjectileVec *= 300;

		bossProjectile->SetVelocity(ProjectileVec);
	}

	return bossProjectile;
}