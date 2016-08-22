/***************************************************************
|	File:		CreditsState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreditState class handles the Credits menu
***************************************************************/
#pragma once

// Class Headers
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class CreditsState : public IGameState
{
private:

	CreditsState() = default;
	virtual ~CreditsState() = default;

	CreditsState(const CreditsState&) = delete;
	CreditsState& operator=(const CreditsState&) = delete;

	float xTitle;
	float xLine1;
	float xLine2;
	float xLine3;
	float xLine4;
	float xLine5;

	bool scrollingDone;

	SGD::HTexture Credits_BG_IMG = SGD::INVALID_HANDLE;

public:
	
	// Singleton
	static CreditsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu
};