/***************************************************************
|	File:		HowToPlayState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	How To Play class handles the How To Play menu
***************************************************************/
#pragma once

// Class Headers
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class HowToPlayState : public IGameState
{
private:

	HowToPlayState() = default;
	virtual ~HowToPlayState() = default;

	HowToPlayState(const HowToPlayState&) = delete;
	HowToPlayState& operator=(const HowToPlayState&) = delete;

	// Cursor
	int m_nCursor = 0;

	// Image Assets
	SGD::HTexture HowToPlayBackGroundIMG = SGD::INVALID_HANDLE;
	SGD::HTexture PlayerIMG = SGD::INVALID_HANDLE;
	SGD::HTexture Player2IMG = SGD::INVALID_HANDLE;
	SGD::HTexture ProjectileIMG = SGD::INVALID_HANDLE;
	SGD::HTexture EnemyIMG = SGD::INVALID_HANDLE;
	SGD::HTexture BossIMG = SGD::INVALID_HANDLE;
	SGD::HTexture BossFireballIMG = SGD::INVALID_HANDLE;

	bool nextpage = false;

public:

	// Singleton
	static HowToPlayState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu
};