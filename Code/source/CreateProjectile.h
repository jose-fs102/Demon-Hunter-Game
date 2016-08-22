/***************************************************************
|	File:		CreateProjectile.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreateProjectile Class to hold the Message 
|				to create a projectile 
***************************************************************/
#pragma once

// Class Headers
#include "MessageID.h"

// Wrappers
#include "../SGD Wrappers/SGD_Message.h"

// C++ Headers

// Forward Declarations
class Player;

class CreateProjectile : public SGD::Message
{
private:

	Player* ProjectileOwner = nullptr;

public:

	CreateProjectile(Player* ptr);
	~CreateProjectile();

	// Accessor for the owner
	Player* GetOwner(void) const { return ProjectileOwner; }
};