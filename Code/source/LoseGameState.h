/***************************************************************
|	File:		LoseGameState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	LoseGameState displays a lose screen
***************************************************************/

#pragma once

// Class Headers
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class LoseGameState : public IGameState
{
private:

	LoseGameState() = default;
	virtual ~LoseGameState() = default;

	LoseGameState(const LoseGameState&) = delete;
	LoseGameState& operator=(const LoseGameState&) = delete;

	SGD::HTexture LoseGame_BG_IMG = SGD::INVALID_HANDLE;
	SGD::HAudio LostMUS = SGD::INVALID_HANDLE;

public:

	// Singleton
	static LoseGameState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

};