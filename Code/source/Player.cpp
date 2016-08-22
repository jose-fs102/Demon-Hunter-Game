/******************************************************
|	File:		Player.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Player class to hold the player data
*******************************************************/

// Class Headers
#include "Player.h"
#include "Game.h"
#include "CreateProjectile.h"
#include "GameplayState.h"

// Wrappers
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

// C++ Headers

// Preprocessor Constant
#define SHOT_DELAY 2.5f
#define SWORD_DELAY 1.0f

Player::Player()
{
	SwordPlayer = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Player2_IMG.png");
	ProjectileHit_FX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_ProjectileHitFX.wav");
}

Player::~Player()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(SwordPlayer);
	SGD::AudioManager::GetInstance()->UnloadAudio(ProjectileHit_FX);
}

void Player::Update(float elapsedTime)
{
	ProjectileTimer += elapsedTime;

	if (sword == true)
	{
		swordattack += elapsedTime;

		if (swordattack >= SWORD_DELAY)
		{
			sword = false;
			swordattack = 0.0f;
		}
	}

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyDown(SGD::Key::Right) == true)
	{
		pSpeed = 0.0f;
		pSpeed += pAccel * elapsedTime;
		direction = 1.0f;

		SGD::Vector newVec;

		newVec = { 1.0f, 0.0f };

		newVec *= pSpeed;

		m_vtVelocity = newVec;
	}

	if (pInput->IsKeyDown(SGD::Key::Left) == true)
	{
		pSpeed = 0.0f;
		pSpeed -= pAccel * elapsedTime;
		direction = -1.0f;

		SGD::Vector newVec;

		newVec = { 1.0f, 0.0f };

		newVec *= pSpeed;

		m_vtVelocity = newVec;
	}
	if (pInput->IsKeyDown(SGD::Key::D) == true && ProjectileTimer > SHOT_DELAY)
	{
		ProjectileTimer = 0.0f;

		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(ProjectileHit_FX))
			SGD::AudioManager::GetInstance()->PlayAudio(ProjectileHit_FX);

		CreateProjectile* MSG = new CreateProjectile(this);
		MSG->QueueMessage();
		MSG = nullptr;
	}
	if (pInput->IsKeyPressed(SGD::Key::S) == true)
	{
		sword = true;
	}

	Entity::Update(elapsedTime);

	InBounds();
}

void Player::Render(void)
{
	// Save the Collision Rectangle
	SGD::Rectangle CollisionRect = GetRect();

	// Draw Rectangle
	SGD::GraphicsManager::GetInstance()->DrawRectangle(CollisionRect, { 0, 0, 0, 0 });

	if (direction == 1.0f)
	{
		if (sword)
		{
			// Draw the player
			SGD::GraphicsManager::GetInstance()->DrawTexture(SwordPlayer, { m_ptPosition.x - 50, m_ptPosition.y - 55 }, 0.0f, {}, {}, { direction, 1.0f });
		}
		else if (!sword)
		{
			// Draw the player
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, m_ptPosition, 0.0f, {}, {}, { direction, 1.0f });
		}
	}
	else if (direction == -1.0f)
	{
		if (sword)
		{
			// Draw the player with adjusted position for collision rectangle
			SGD::GraphicsManager::GetInstance()->DrawTexture(SwordPlayer,
			{ ((m_ptPosition.x + m_szSize.width * 3) + 55), (m_ptPosition.y - 55) },
			0.0f, {}, {}, { direction, 1.0f });
		}
		else if (!sword)
		{
			// Draw the player with adjusted position for collision rectangle
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
			{ (m_ptPosition.x + m_szSize.width * 3), (m_ptPosition.y) },
			0.0f, {}, {}, { direction, 1.0f });
		}
	}
}

SGD::Rectangle Player::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize * 3 };
}

void Player::InBounds(void)
{
	// Check right edge
	if (m_ptPosition.x > (Game::GetInstance()->GetScreenWidth() - m_szSize.width * 2) - 25)
	{
		m_ptPosition.x = (Game::GetInstance()->GetScreenWidth() - m_szSize.width * 2) - 25;
		pSpeed = 0.0f;
	}

	// Check left edge
	if (m_ptPosition.x < (m_szSize.width) - 35)
	{
		m_ptPosition.x = (m_szSize.width) - 35;
		pSpeed = 0.0f;
	}

	// Check the top edge
	if (m_ptPosition.y < Game::GetInstance()->GetScreenHeight() / 2)
	{
		m_ptPosition.y = Game::GetInstance()->GetScreenHeight() / 2;
	}

	// Check the bottom edge
	if (m_ptPosition.y >(Game::GetInstance()->GetScreenHeight() / 2) + 200)
	{
		m_ptPosition.y = (Game::GetInstance()->GetScreenHeight() / 2) + 200;
	}
}

void Player::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ENEMY)
	{
		if (sword == false)
			health -= 10;

		score += 10;
	}
	else if (pOther->GetType() == ENT_BOSS_PROJECTILE)
	{
		if (sword == false)
			health -= 20;
	}
	else if (pOther->GetType() == ENT_BOSS)
	{
		health = 0;
	}
}