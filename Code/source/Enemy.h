/***************************************************************
|	File:		Enemy.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Enemy Class to hold the Enemy data
***************************************************************/
#pragma once

// Class Headers
#include "Entity.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class Enemy : public Entity
{
private:

	float direction = 1.0f;
	int spawncount = 0;

public:

	Enemy() = default;
	virtual ~Enemy() = default;

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int	GetType(void) const override { return ENT_ENEMY; }
	virtual SGD::Rectangle GetRect(void) const override;

	virtual void HandleCollision(const IEntity* pOther)	override;

	float RandomPosX();
};