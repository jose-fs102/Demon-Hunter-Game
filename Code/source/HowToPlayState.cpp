/***************************************************************
|	File:		HowToPlayState.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	How To Play class handles the How To Play menu
***************************************************************/

// Class Headers
#include "HowToPlayState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionsState.h"

// Wrappers
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

// C++ Headers

HowToPlayState* HowToPlayState::GetInstance(void)
{
	static HowToPlayState htp_instance;

	return &htp_instance;
}

// Load Resources
void HowToPlayState::Enter(void)
{
	// Load Images
	HowToPlayBackGroundIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_HowToPlay_BG_IMG.png");
	PlayerIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Player_IMG.png");
	Player2IMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Player2_IMG.png");
	ProjectileIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Projectile_IMG.png");
	EnemyIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Enemy_IMG.png");
	BossIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Boss_IMG.png");
	BossFireballIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_BossProjectile_IMG.png");
}

// Unload Resources
void HowToPlayState::Exit(void)
{
	// Unload Images
	SGD::GraphicsManager::GetInstance()->UnloadTexture(HowToPlayBackGroundIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(PlayerIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(Player2IMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(ProjectileIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(EnemyIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(BossIMG);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(BossFireballIMG);
}

// Handle player input
bool HowToPlayState::Update(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && nextpage)
	{
		nextpage = false;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		// Go back to main menu
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;
	}
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true && !nextpage)
	{
		nextpage = true;
	}

	return true;
}

// Draw To the screen
void HowToPlayState::Render(float elapsedTime)
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	// Render the Background Image
	SGD::GraphicsManager::GetInstance()->DrawTexture(HowToPlayBackGroundIMG, { 0.0f, 0.0f });

	pFont->Draw("Player", { ((Game::GetInstance()->GetScreenWidth() / 2) - 450),
		(Game::GetInstance()->GetScreenHeight() / 2) + 100 },
		1.0f, { 255, 255, 0 });

	// Render the Player
	SGD::GraphicsManager::GetInstance()->DrawTexture(PlayerIMG, { ((Game::GetInstance()->GetScreenWidth() / 2) - 375),
		(Game::GetInstance()->GetScreenHeight() / 2) + 150 });

	// Render the Player2
	SGD::GraphicsManager::GetInstance()->DrawTexture(Player2IMG, { ((Game::GetInstance()->GetScreenWidth() / 2) - 350),
		(Game::GetInstance()->GetScreenHeight() / 2) + 150 });

		// Render the Projectile
		SGD::GraphicsManager::GetInstance()->DrawTexture(ProjectileIMG, { ((Game::GetInstance()->GetScreenWidth() / 2) - 100),
			(Game::GetInstance()->GetScreenHeight() / 2) + 250 });

	if (!nextpage)
	{ 

		// Render the Enemy
		SGD::GraphicsManager::GetInstance()->DrawTexture(EnemyIMG, { (Game::GetInstance()->GetScreenWidth() / 2) + 300,
			(Game::GetInstance()->GetScreenHeight() / 2) + 250 });

		pFont->Draw("How To Play", { (width - (4 * 40 * 4.0f)) / 2, 25 },
			2.0f, { 255, 255, 255 });

		pFont->Draw("The object of this game\nis to defeat incoming\nenemies before they drop\nyour health to zero", { 35, 100 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Kill enough enemies and\nthe final boss will appear!\n\nUse the Arrow Keys to move.", { 35, 250 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Use 'S' key for Sword Slash.\nUse 'D' key for projectile.", { 35, 375 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Projectile", { ((Game::GetInstance()->GetScreenWidth() / 2) - 150),
			(Game::GetInstance()->GetScreenHeight() / 2) + 200 },
			1.0f, { 255, 255, 0 });

		pFont->Draw("Enemy", { (Game::GetInstance()->GetScreenWidth() / 2) + 250,
			(Game::GetInstance()->GetScreenHeight() / 2) + 200 },
			1.0f, { 255, 255, 0 });

		pFont->Draw("()Next", { (Game::GetInstance()->GetScreenWidth() / 2) + 250,
			(Game::GetInstance()->GetScreenHeight() / 2) + 350 },
			1.0f, { 255, 255, 0 });
	}
	else if (nextpage)
	{
		pFont->Draw("How To Play", { (width - (4 * 40 * 4.0f)) / 2, 25 },
			2.0f, { 255, 255, 255 });

		pFont->Draw("The Boss will appear after\nyour score reaches 100", { 35, 100 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("The Boss is immune to\nyour sword attack", { 35, 175 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Use your projectile to attack.\nUse your sword to stop\nincoming fireballs.\n\nKeep Your Distance!!\nContact with the boss is \ninstant death!", { 35, 250 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Boss", { (Game::GetInstance()->GetScreenWidth() / 2) + 300,
			(Game::GetInstance()->GetScreenHeight() / 2) + 150 },
			1.0f, { 255, 255, 0 });

		// Render the Boss
		SGD::GraphicsManager::GetInstance()->DrawTexture(BossIMG, { (Game::GetInstance()->GetScreenWidth() / 2) + 450,
			(Game::GetInstance()->GetScreenHeight() / 2) + 200 }, 0.0f, {}, {}, { -1.0f, 1.0f });

		pFont->Draw("Projectiles", { ((Game::GetInstance()->GetScreenWidth() / 2) - 150),
			(Game::GetInstance()->GetScreenHeight() / 2) + 150 },
			1.0f, { 255, 255, 0 });

		// Render the Boss projectile
		SGD::GraphicsManager::GetInstance()->DrawTexture(BossFireballIMG, { (Game::GetInstance()->GetScreenWidth() / 2) + 250,
			(Game::GetInstance()->GetScreenHeight() / 2) + 200 }, 0.0f, {}, {}, { -1.0f, 1.0f });
	}
}