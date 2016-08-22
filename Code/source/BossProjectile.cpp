/***************************************************************
|	File:		BossProjectile.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	BossProjectile Class to hold the Message
|				to create a Boss projectile
***************************************************************/

// Class Headers
#include "BossProjectile.h"
#include "Projectile.h"
#include "Game.h"
#include "DestroyEntityMessage.h"
#include "IEntity.h"
#include "Player.h"
#include "Enemy.h"

// Wrappers
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"

// C++ Headers

BossProjectile::~BossProjectile()
{
	Owner->Release();
}

void BossProjectile::Update(float elapsedTime)
{
	Entity::Update(elapsedTime);

	SGD::Rectangle Rect = this->GetRect();
	SGD::Rectangle ScreenRectangle = { 0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() };

	if (Rect.IsIntersecting(ScreenRectangle) == false)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;
	}
}

void BossProjectile::Render(void)
{
	SGD::Rectangle CollisionRect = GetRect();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(CollisionRect, { 0, 0, 0, 0 });

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, m_ptPosition);
}

SGD::Rectangle BossProjectile::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void BossProjectile::SetOwner(Entity* ptr)
{
	// Release existing owner
	if (Owner != nullptr)
		Owner->Release();

	// Store the new owner
	Owner = ptr;

	// Add reference to new owner
	if (Owner != nullptr)
		Owner->AddRef();
}

void BossProjectile::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;
	}
}