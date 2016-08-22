/***************************************************************
|	File:		Boss.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Boss Class to hold the Boss data
***************************************************************/

// Class Headers
#include "Boss.h"
#include "CreateBossProjectile.h"
#include "Game.h"
#include "GameplayState.h"
#include "DestroyEntityMessage.h"

// Wrappers
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"

// C++ Headers

// Preprocessor Constant
#define BOSS_SHOT_DELAY 4.50f

void Boss::Update(float elapsedTime)
{
	fireTimer += elapsedTime;

	if (fireTimer > BOSS_SHOT_DELAY)
	{
		CreateBossProjectile* MSG = new CreateBossProjectile(this);
		MSG->QueueMessage();
		MSG = nullptr;

		fireTimer = 0.0f;
	}

	if (BossHealth < 35)
	{
		if (direction == -1.0f)
		{
			SGD::Vector vec = { -1.0f, 0.0f };

			vec *= 55;

			vec.Rotate(m_fRotation);

			m_vtVelocity = vec;
		}
		else if (direction == 1.0f)
		{
			SGD::Vector vec = { 1.0f, 0.0f };

			vec *= 55;

			vec.Rotate(m_fRotation);

			m_vtVelocity = vec;
		}
	}

	Entity::Update(elapsedTime);

	if (m_ptPosition.x < 0)
		direction = 1.0f;
}

void Boss::Render(void)
{
	SGD::Rectangle CollisionRect = GetRect();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(CollisionRect, { 0, 0, 0, 0 });

	if (direction == 1.0f)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { m_ptPosition.x + 15, m_ptPosition.y }, 0.0f, {}, {}, { direction, 1.0f });
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { m_ptPosition.x + 120, m_ptPosition.y }, 0.0f, {}, {}, { direction, 1.0f });
}

SGD::Rectangle Boss::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Boss::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PROJECTILE)
	{
		BossHealth -= 10;
	}
}