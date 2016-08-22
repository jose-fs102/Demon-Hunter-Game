/***************************************************************
|	File:		CreateProjectile.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreateProjectile Class to hold the Message 
|				to create a projectile 
***************************************************************/

// Class Headers
#include "CreateProjectile.h"
#include "MessageID.h"
#include "Player.h"

// Wrappers

// C++ Headers

CreateProjectile::CreateProjectile(Player* ptr) : Message(MessageID::MSG_CREATE_PROJECTILE)
{
	ProjectileOwner = ptr;

	ProjectileOwner->AddRef();
}

CreateProjectile::~CreateProjectile()
{
	ProjectileOwner->Release();
}