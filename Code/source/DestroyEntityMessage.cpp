/***************************************************************
|	File:		DestroyEntityMessage.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	DestroyEntityMessage class stores the entity
|				to be removed from the Entity Manager
|				when the message is processed
***************************************************************/

// Class Headers
#include "DestroyEntityMessage.h"
#include "MessageID.h"
#include "Entity.h"

// Wrappers

// C++ Headers
#include <cassert>


/**************************************************************/
// CONSTRUCTOR
//	- hold a reference to the pointer
DestroyEntityMessage::DestroyEntityMessage( Entity* ptr )
	: Message( MessageID::MSG_DESTROY_ENTITY )
{
	// Validate the parameter
	assert( ptr != nullptr 
			&& "DestroyEntityMessage - cannot destroy null" );

	// Store the parameter
	m_pEntity = ptr;
	m_pEntity->AddRef();
}

/**************************************************************/
// DESTRUCTOR
//	- release the reference
DestroyEntityMessage::~DestroyEntityMessage()
{
	m_pEntity->Release();
	m_pEntity = nullptr;
}
