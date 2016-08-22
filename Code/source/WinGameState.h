/***************************************************************
|	File:		WinGameState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	WinGameState displays a Win screen
***************************************************************/

#pragma once

// Class Headers
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class WinGameState : public IGameState
{

private:

	WinGameState() = default;
	virtual ~WinGameState() = default;

	WinGameState(const WinGameState&) = delete;
	WinGameState& operator=(const WinGameState&) = delete;

	SGD::HTexture Victory_BG = SGD::INVALID_HANDLE;
	SGD::HAudio VictoryMUS = SGD::INVALID_HANDLE;

public:

	// Singleton
	static WinGameState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu
};