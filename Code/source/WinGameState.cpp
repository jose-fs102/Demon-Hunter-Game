/***************************************************************
|	File:		LoseGameState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	LoseGameState displays a lose screen
***************************************************************/

// Class Headers
#include "WinGameState.h"
#include "LoseGameState.h"
#include "BitmapFont.h"
#include "Game.h"
#include "MainMenuState.h"
#include "Player.h"
#include "GameplayState.h"
#include "CreditsState.h"

// Wrappers
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

// C++ Headers

// Singleton
WinGameState* WinGameState::GetInstance(void)
{
	static WinGameState WGS_Instance;

	return &WGS_Instance;
}

void WinGameState::Enter(void)
{
	Victory_BG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_WinGameState_BG_IMG.png");
	VictoryMUS = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_VictoryMUS.xwm");

	SGD::AudioManager::GetInstance()->PlayAudio(VictoryMUS);
}

void WinGameState::Exit(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(Victory_BG);
	SGD::AudioManager::GetInstance()->UnloadAudio(VictoryMUS);
}

bool WinGameState::Update(float elapsedTime)
{
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(VictoryMUS))
	{
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		return true;
	}

	return true;
}

void WinGameState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(Victory_BG, { 0.0f, 0.0f });

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	float height = Game::GetInstance()->GetScreenHeight();

	pFont->Draw("You Win!", { (width - (4 * 28 * 4.0f)) / 2, 50 },
		2.0f, { 255, 255, 255 });
}