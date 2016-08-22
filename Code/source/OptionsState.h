/***************************************************************
|	File:		OptionsState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Options class handles the options menu
***************************************************************/
#pragma once

// Class Headers
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"

// C++ Headers

class OptionsState : public IGameState
{
private:

	OptionsState() = default;
	virtual ~OptionsState() = default;

	OptionsState(const OptionsState&) = delete;
	OptionsState& operator=(const OptionsState&) = delete;

	// Cursor
	int m_nCursor = 0;

	// Image Assets
	SGD::HTexture OptionsBackGroundIMG = SGD::INVALID_HANDLE;

	// Audio Assets
	SGD::HAudio CursorSelectionFX = SGD::INVALID_HANDLE;
	SGD::HAudio ProjectileHitFX = SGD::INVALID_HANDLE;
	SGD::HAudio GamePlayMUS = SGD::INVALID_HANDLE;

	bool setMaster;
	bool setFX;

	int VolumeM;
	int VolumeFX;

public:

	// Singleton
	static OptionsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

	// Volume Accessors
	int GetMasterVolume(void) const { return VolumeM; }
	int GetFXVolume(void) const { return VolumeFX; }

	// Volume Mutators
	void SetMasterVolume(int _VolumeM) { VolumeM = _VolumeM; }
	void SetFXVolume(int _VolumeFX) { VolumeFX = _VolumeFX; }
};