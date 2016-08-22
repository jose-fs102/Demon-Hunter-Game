/******************************************************
|	File:		Player.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Player class to hold the player data
*******************************************************/
#pragma once

// Class Headers
#include "Entity.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"
// C++ Headers

class Player : public Entity
{
private:

	int score;
	int health;
	
	float direction = 1.0f;
	float pAccel = 0.0f;
	float pSpeed = 0.0f;
	float ProjectileTimer = 0.0f;
	float swordattack = 0.0f;

	bool sword = false;

	SGD::HTexture SwordPlayer = SGD::INVALID_HANDLE;
	SGD::HAudio ProjectileHit_FX = SGD::INVALID_HANDLE;

public:

	Player();
	virtual ~Player();

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int	GetType(void) const override { return ENT_PLAYER; }
	virtual SGD::Rectangle GetRect(void) const override;

	void InBounds(void);

	virtual void	HandleCollision(const IEntity* pOther)	override;

	int GetScore(void) const { return score; }
	int GetHealth(void) const { return health; }

	float GetDirection(void) const { return direction; }
	float GetAccel(void) const { return pAccel; }
	float GetSpeed(void) const { return pSpeed; }

	void SetScore(int _score) { score = _score; }
	void SetHealth(int _health) { health = _health; }
	void SetAccel(float _Accel) { pAccel = _Accel; }
	void SetSpeed(float _Speed) { pSpeed = _Speed; }
};