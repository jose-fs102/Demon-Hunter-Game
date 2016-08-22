/***************************************************************
|	File:		Boss.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Boss Class to hold the Boss data
***************************************************************/

#pragma once

// Class Headers
#include "Entity.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class Boss : public Entity
{
private:

	int BossHealth = 100;
	float direction = -1.0f;
	float fireTimer = 0.0f;

	SGD::HAudio ProjectileHit_FX = SGD::INVALID_HANDLE;

public:

	Boss() = default;
	virtual ~Boss() = default;

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int	GetType(void) const override { return ENT_BOSS; }
	virtual SGD::Rectangle GetRect(void) const override;

	virtual void HandleCollision(const IEntity* pOther)	override;

	int GetBossHealth(void) const { return BossHealth; }

	float GetDirection(void) const { return direction; }

	void SetBossHealth(int _bosshealth) { BossHealth = _bosshealth; }
};