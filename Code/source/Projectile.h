/***************************************************************
|	File:		Projectile.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Projectile Class to hold the Projectile data
***************************************************************/

#pragma once

// Class Headers
#include "Entity.h"

// Wrappers

// C++ Headers

class player;

class Projectile : public Entity
{
private:

	Entity* Owner = nullptr;

public:

	Projectile() = default;
	virtual ~Projectile();

	// Override the GetType method to return ENT_BULLET
	virtual int	GetType(void) const	override { return ENT_PROJECTILE; }

	virtual void Update(float elapsedTime) override;

	// Override the Render method to draw the entity's image rotated and centered on its position,
	// offset from the camera's position
	virtual void Render(void) override;

	// override the GetRect method to return the collision rectangle 
	// centered on its position (to match how the image is rendered).
	virtual SGD::Rectangle GetRect(void) const override;

	// Accessor
	Entity* GetOwner(void) const { return Owner; }

	// Mutator
	void SetOwner(Entity* ptr);

	// Override the Handle Collision method
	virtual void HandleCollision(const IEntity* pOther)	override;
};