/***************************************************************
|	File:		MainMenuState.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

// Class Headers
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionsState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"

// Wrappers
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

// C++ Headers
#include <fstream>

/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ MainMenuState* MainMenuState::GetInstance(void)
{
	static MainMenuState s_Instance;

	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset the game & load resources
/*virtual*/ void MainMenuState::Enter(void)
{
	// Load Images
	MainMenuBackgroundIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_MainMenu_BG_IMG.png");

	// Load Audio
	CursorSelectionFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_CursorSelectionFX.wav");

	// Local bool to tell ChangeState that we've 
	// entered the Main Menu for the first time
	bool b = false;
	Game::GetInstance()->SetChoice(b);

	// Read in volume levels 
	std::fstream in;

	in.open("VolumeSettings.txt", std::ios_base::in);

	if (in.is_open())
	{
		int mVolume, xVolume;

		in.ignore(INT_MAX, '\n');
		in.ignore(INT_MAX, '=');

		in >> mVolume;

		in.ignore(INT_MAX, '\n');
		in.ignore(INT_MAX, '=');

		in >> xVolume;

		if (mVolume > 100)
			mVolume = 100;
		else if (mVolume < 0)
			mVolume = 0;
		else if (mVolume % 10 != 0)
			mVolume = 100;

		OptionsState::GetInstance()->SetMasterVolume(mVolume);

		if (xVolume > 100)
			xVolume = 100;
		else if (xVolume < 0)
			xVolume = 0;
		else if (xVolume % 10 != 0)
			xVolume = 100;

		OptionsState::GetInstance()->SetFXVolume(xVolume);

		in.close();
	}
	else
	{
		// If file fails to open, 
		// default volume levels to 100
		OptionsState::GetInstance()->SetMasterVolume(100);
		OptionsState::GetInstance()->SetFXVolume(100);
	}

	// Set the volume
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, OptionsState::GetInstance()->GetMasterVolume());
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, OptionsState::GetInstance()->GetFXVolume());
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void MainMenuState::Exit(void)
{
	// Unload Images
	SGD::GraphicsManager::GetInstance()->UnloadTexture(MainMenuBackgroundIMG);

	// Unload Audio
	SGD::AudioManager::GetInstance()->UnloadAudio(CursorSelectionFX);
}

/**************************************************************/
// Update
//	- handle input & update entites
/*virtual*/ bool MainMenuState::Update(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		// Move cursor to "Exit"
		m_nCursor = 4;
	}

	// Move cursor
	if (pInput->IsKeyPressed(SGD::Key::Down) == true)
	{
		++m_nCursor;

		SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

		// Wrap around
		if (m_nCursor > 4)		// should enumerate the options
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true)
	{
		--m_nCursor;

		SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

		// Wrap around
		if (m_nCursor < 0)
			m_nCursor = 4;
	}


	// Select an option?
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		if (m_nCursor == 0) // Play game
		{
			// Change to GamePlay State
			Game::GetInstance()->ChangeState(GameplayState::GetInstance());

			// Exit this state immediately
			return true;
		}
		else if (m_nCursor == 1) // Options
		{
			// Change to Options State
			Game::GetInstance()->ChangeState(OptionsState::GetInstance());

			// Exit this state immediately
			return true;
		}
		else if (m_nCursor == 2) // How To Play
		{
			// Change to How To Play State
			Game::GetInstance()->ChangeState(HowToPlayState::GetInstance());

			// Exit this state immediately
			return true;
		}
		else if (m_nCursor == 3) // Credits
		{
			// Change to Credits State
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());

			// Exit this state immediately
			return true;
		}
		else if (m_nCursor == 4) // Exit
		{
			// Exit the Program
			return false;
		}
	}

	return true;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void MainMenuState::Render(float elapsedTime)
{
	// Draw Background Image
	SGD::GraphicsManager::GetInstance()->DrawTexture(MainMenuBackgroundIMG, { 0.0f, 0.0f });

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	// Display the game title centered at 4x scale
	pFont->Draw("DemonHunter", { (width - (4 * 42 * 4.0f)) / 2, 50 },
		2.0f, { 255, 255, 255 });

	// Display the menu options centered at 1x scale
	pFont->Draw("Play", { (width - (4 * 45)) / 2, 300 },
		1.0f, { 255, 0, 0 });

	pFont->Draw("Options", { (width - (4 * 45)) / 2, 350 },
		1.0f, { 255, 0, 0 });

	pFont->Draw("How To Play", { (width - (4 * 45)) / 2, 400 },
		1.0f, { 255, 0, 0 });

	pFont->Draw("Credits", { (width - (4 * 45)) / 2, 450 },
		1.0f, { 255, 0, 0 });

	pFont->Draw("Exit", { (width - (4 * 45)) / 2, 500 },
		1.0f, { 255, 0, 0 });

	switch (m_nCursor)
	{
	case 0:
		pFont->Draw("(    )", { (width - (7 * 35)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 0, 0 });
		break;
	case 1:
		pFont->Draw("(       )", { (width - (7 * 35)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 0, 0 });
		break;
	case 2:
		pFont->Draw("(           )", { (width - (7 * 35)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 0, 0 });
		break;
	case 3:
		pFont->Draw("(       )", { (width - (7 * 35)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 0, 0 });
		break;
	case 4:
		pFont->Draw("(    )", { (width - (7 * 35)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 0, 0 });
		break;
	}
}
