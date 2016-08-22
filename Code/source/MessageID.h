/***************************************************************
|	File:		MessageID.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H

// Class Headers

// Wrappers

// C++ Headers

/**************************************************************/
// MessageID enum
//	- enumerated list of message identifiers
enum class MessageID
{
	MSG_UNKNOWN,
	MSG_DESTROY_ENTITY,
	MSG_CREATE_PROJECTILE,
	MSG_CREATE_BOSS_PROJECTILE,
};


#endif //MESSAGEID_H