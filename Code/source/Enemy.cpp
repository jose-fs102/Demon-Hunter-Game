/***************************************************************
|	File:		Enemy.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Enemy Class to hold the Enemy data
***************************************************************/

// Class Headers
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "DestroyEntityMessage.h"
#include "GameplayState.h"

// Wrappers
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

// C++ Headers
#include <cstdlib>

void Enemy::Update(float elapsedTime)
{
	if (direction == 1.0f)
	{
		SGD::Vector vec = { -1.0f, 0.0f };

		vec *= 55;

		vec.Rotate(m_fRotation);

		m_vtVelocity = vec;
	}
	else if (direction == -1.0f)
	{
		SGD::Vector vec = { 1.0f, 0.0f };

		vec *= 55;

		vec.Rotate(m_fRotation);

		m_vtVelocity = vec;
	}

	Entity::Update(elapsedTime);

	if (m_ptPosition.x < m_szSize.width)
		direction = -1.0f;
}

void Enemy::Render(void)
{
	// Save the collision rectangle
	SGD::Rectangle CollisionRect = GetRect();

	// Draw the collision rectangle
	SGD::GraphicsManager::GetInstance()->DrawRectangle(CollisionRect, { 0, 0, 0, 0 });

	// Draw the enemy(s)

	if (direction == 1.0f)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { m_ptPosition.x - 15, m_ptPosition.y }, 0.0f, {}, {}, { direction, 1.0f });
	else if (direction == -1.0f)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { m_ptPosition.x + 70, m_ptPosition.y }, 0.0f, {}, {}, { direction, 1.0f });
}

SGD::Rectangle Enemy::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize * 2 };
}

void Enemy::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;

		if (GameplayState::GetInstance()->GetBossBool() == false)
		{
			GameplayState::GetInstance()->SetSpawnCount(spawncount);
			spawncount++;

			GameplayState::GetInstance()->SetEnemyBool(false);
		}
	}
	else if (pOther->GetType() == ENT_PROJECTILE)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;

		if (GameplayState::GetInstance()->GetBossBool() == false)
		{
			GameplayState::GetInstance()->SetSpawnCount(spawncount);
			spawncount++;

			GameplayState::GetInstance()->SetEnemyBool(false);
		}
	}
}

float Enemy::RandomPosX()
{
	m_ptPosition.x = rand() % (int)(Game::GetInstance()->GetScreenWidth());
	return m_ptPosition.x;
}