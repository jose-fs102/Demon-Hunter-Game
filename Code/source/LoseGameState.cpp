/***************************************************************
|	File:		LoseGameState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	LoseGameState displays a lose screen
***************************************************************/

// Class Headers
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
LoseGameState* LoseGameState::GetInstance(void)
{
	static LoseGameState LGS_Instance;

	return &LGS_Instance;
}


void LoseGameState::Enter(void)
{
	LoseGame_BG_IMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_LoseGameState_BG_IMG.png");
	LostMUS = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_YouFailedMUS.xwm");

	SGD::AudioManager::GetInstance()->PlayAudio(LostMUS);
}

void LoseGameState::Exit(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(LoseGame_BG_IMG);
	SGD::AudioManager::GetInstance()->UnloadAudio(LostMUS);
}

bool LoseGameState::Update(float elapsedTime)
{
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(LostMUS))
	{
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		return true;
	}

	return true;
}

void LoseGameState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(LoseGame_BG_IMG, { 0.0f, 0.0f });

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	float height = Game::GetInstance()->GetScreenHeight();

	pFont->Draw("You lose", { (width - (4 * 28 * 4.0f)) / 2, 50 },
		2.0f, { 255, 255, 255 });
}