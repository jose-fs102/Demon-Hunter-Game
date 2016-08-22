/***************************************************************
|	File:		CreateBossProjectile.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreateBossProjectile Class to hold the Message
|				to create a Boss projectile
***************************************************************/

#pragma once

// Class Headers
#include "MessageID.h"

// Wrappers
#include "../SGD Wrappers/SGD_Message.h"

// C++ Headers

// Forward Declarations
class Boss;

class CreateBossProjectile : public SGD::Message
{
private:

	Boss* owner = nullptr;

public:

	CreateBossProjectile(Boss* ptr);
	virtual ~CreateBossProjectile();

	Boss* GetBossOwner(void) const { return owner; }
};