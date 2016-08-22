/***************************************************************
|	File:		DestroyEntityMessage.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	DestroyEntityMessage class stores the entity
|				to be removed from the Entity Manager
|				when the message is processed
***************************************************************/

#pragma once

// Class Headers

// Wrappers
#include "../SGD Wrappers/SGD_Message.h"	// uses SGD::Message

// C++ Headers

// Forward Class Declarations
class Entity;								// uses Entity*


/**************************************************************/
// DestroyEntityMessage class
//	- stores the entity to be removed from the Entity Manager
class DestroyEntityMessage : public SGD::Message
{
public:
	/**********************************************************/
	// Constructor REQUIRES the entity to remove
	DestroyEntityMessage( Entity* ptr );
	virtual ~DestroyEntityMessage();

	/**********************************************************/
	// Accessor
	Entity*		GetEntity( void ) const		{	return m_pEntity;	}

private:
	/**********************************************************/
	// entity to destroy
	Entity*		m_pEntity;
};

