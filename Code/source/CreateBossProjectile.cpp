/***************************************************************
|	File:		CreateBossProjectile.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreateBossProjectile Class to hold the Message
|				to create a Boss projectile
***************************************************************/

// Class Headers
#include "CreateBossProjectile.h"
#include "MessageID.h"
#include "Boss.h"

// Wrappers

// C++ Headers


CreateBossProjectile::CreateBossProjectile(Boss* ptr) : Message(MessageID::MSG_CREATE_BOSS_PROJECTILE)
{
	owner = ptr;

	owner->AddRef();
}


CreateBossProjectile::~CreateBossProjectile()
{
	owner->Release();
}