/***************************************************************
|	File:		Projectile.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Projectile Class to hold the Projectile data
***************************************************************/

// Class Headers
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
#include <cassert>

Projectile::~Projectile()
{
	// Release the owner 
	Owner->Release();
}

void Projectile::Update(float elapsedTime)
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

void Projectile::Render()
{
	// Verify the image has been set
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "Projectile::Render() - image was not set!");

	SGD::Rectangle CollisionRect = GetRect();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(CollisionRect, { 0, 0, 0, 0 });

	Player* P;
	P = dynamic_cast<Player*> (Owner);

	if (P->GetDirection() == 1.0f)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, m_ptPosition, 0.0f, {}, {}, { P->GetDirection(), 1.0f });
	}
	else if (P->GetDirection() == -1.0f)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
		{ (m_ptPosition.x + (m_szSize.width * 2) - 135), (m_ptPosition.y) },
		0.0f, {}, {}, { P->GetDirection(), 1.0f });
	}
}

// Collision Rectangle
SGD::Rectangle Projectile::GetRect() const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Projectile::SetOwner(Entity* ptr)
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

// HandleCollision(const IEntity* pOther)
// - Queue a "TANK_HIT" event into the Event Manager
void Projectile::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ENEMY)
	{
		Player* P;
		P = dynamic_cast<Player*> (Owner);

		int sc = P->GetScore();
		sc += 10;
		P->SetScore(sc);

		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;
	}
	if (pOther->GetType() == ENT_BOSS)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;
	}
	if (pOther->GetType() == ENT_BOSS_PROJECTILE)
	{
		// Send a Destroy Entity Message
		DestroyEntityMessage* NewMsg = new DestroyEntityMessage(this);
		NewMsg->QueueMessage();
		NewMsg = nullptr;
	}
}