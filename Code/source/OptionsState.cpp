/***************************************************************
|	File:		OptionsState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Options class handles the options menu
***************************************************************/

// Class Headers
#include "OptionsState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"

// Wrappers
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

// C++ Headers
#include <fstream>

// Singleton
OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState op_Instance;

	return &op_Instance;
}

// Load Resources
void OptionsState::Enter(void)
{
	// Set a clear background
	SGD::GraphicsManager::GetInstance()->SetClearColor();

	// Load Images
	OptionsBackGroundIMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Options_BG_IMG.png");

	// Load Audio
	CursorSelectionFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_CursorSelectionFX.wav");
	ProjectileHitFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_ProjectileHitFX.wav");
	GamePlayMUS = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_GamePlay_BG_MUS.xwm");

	// Reset the cursor to 0
	m_nCursor = 0;

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

		SetMasterVolume(mVolume);

		if (xVolume > 100)
			xVolume = 100;
		else if (xVolume < 0)
			xVolume = 0;
		else if (xVolume % 10 != 0)
			xVolume = 100;

		SetFXVolume(xVolume);

		in.close();
	}
	else
	{
		// If file fails to open, 
		// default volume levels to 100
		SetMasterVolume(100);
		SetFXVolume(100);
	}
}

// Unload Resources
void OptionsState::Exit(void)
{
	// Unload Images
	SGD::GraphicsManager::GetInstance()->UnloadTexture(OptionsBackGroundIMG);

	// Unload Audio
	SGD::AudioManager::GetInstance()->UnloadAudio(CursorSelectionFX);
	SGD::AudioManager::GetInstance()->UnloadAudio(ProjectileHitFX);
	SGD::AudioManager::GetInstance()->UnloadAudio(GamePlayMUS);
}

// Handle player input
bool OptionsState::Update(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && setMaster)
	{
		setMaster = false;
		SGD::AudioManager::GetInstance()->StopAudio(GamePlayMUS);
	}
	else if (pInput->IsKeyPressed(SGD::Key::Escape) == true && setFX)
	{
		setFX = false;
		SGD::AudioManager::GetInstance()->StopAudio(ProjectileHitFX);
	}
	else if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		// Read out volume levels to a file
		std::ofstream out;

		out.open("VolumeSettings.txt", std::ios_base::out);

		if (out.is_open())
		{
			out << "[VOLUME] // Increments of 10 ONLY! or Default to 100\n";
			out << "MasterVolume=" << VolumeM << '\n';
			out << "SoundFXVolume=" << VolumeFX;
		}
		out.close();

		// Change to Main Menu State
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		// Exit this state immediatly
		return  true;
	}

	if (!setMaster && !setFX)
	{
		if (pInput->IsKeyPressed(SGD::Key::Right) == true)
		{
			m_nCursor++;

			SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

			if (m_nCursor > 1)
				m_nCursor = 0;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Left) == true)
		{
			m_nCursor--;

			SGD::AudioManager::GetInstance()->PlayAudio(CursorSelectionFX);

			if (m_nCursor < 0)
				m_nCursor = 1;
		}

		if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
		{
			if (m_nCursor == 0)
			{
				setMaster = true;
				setFX = false;
			}
			else if (m_nCursor == 1)
			{
				setMaster = false;
				setFX = true;
			}
		}
	}

	if (setMaster)
	{
		if (pInput->IsKeyPressed(SGD::Key::Right) == true)
		{
			VolumeM += 10;

			if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(GamePlayMUS))
				SGD::AudioManager::GetInstance()->PlayAudio(GamePlayMUS);

			if (VolumeM > 100)
				VolumeM = 100;

			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, VolumeM);
		}
		else if (pInput->IsKeyPressed(SGD::Key::Left) == true)
		{
			VolumeM -= 10;

			if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(GamePlayMUS))
				SGD::AudioManager::GetInstance()->PlayAudio(GamePlayMUS);

			if (VolumeM < 0)
				VolumeM = 0;

			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, VolumeM);
		}
	}
	else if (setFX)
	{
		if (pInput->IsKeyPressed(SGD::Key::Right) == true)
		{
			VolumeFX += 10;

			if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(ProjectileHitFX))
				SGD::AudioManager::GetInstance()->PlayAudio(ProjectileHitFX);

			if (VolumeFX > 100)
				VolumeFX = 100;

			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, VolumeFX);
		}
		else if (pInput->IsKeyPressed(SGD::Key::Left) == true)
		{
			VolumeFX -= 10;

			if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(ProjectileHitFX))
				SGD::AudioManager::GetInstance()->PlayAudio(ProjectileHitFX);

			if (VolumeFX < 0)
				VolumeFX = 0;

			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, VolumeFX);
		}
	}

	return true;
}

// Draw to the screen
void OptionsState::Render(float elapsedTime)
{
	// Draw Background
	SGD::GraphicsManager::GetInstance()->DrawTexture(OptionsBackGroundIMG, { 0.0f, 0.0f });

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	if (!setMaster && !setFX)
	{
		pFont->Draw("Options", { (width - (4 * 28 * 4.0f)) / 2, 50 },
			2.0f, { 255, 255, 255 });

		pFont->Draw(" Master\n    Volume", { 35, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });

		pFont->Draw(" SoundFX\n    Volume", { width - 350, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });

		switch (m_nCursor)
		{
		case 0:
			pFont->Draw("-(", { 0, 385.0f + 50 * m_nCursor },
				1.0f, { 156, 152, 78 });
			break;
		case 1:
			pFont->Draw(")-", { width - 95, 330.0f + 50 * m_nCursor },
				1.0f, { 156, 152, 78 });
			break;
		}
	}
	else if (setMaster)
	{
		pFont->Draw("Options", { (width - (4 * 28 * 4.0f)) / 2, 50 },
			2.0f, { 255, 255, 255 });

		pFont->Draw(" Master\n    Volume", { 35, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });

		switch (VolumeM)
		{
		case 0:
			pFont->Draw("(Vol = 0)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 10:
			pFont->Draw("(Vol = 10)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 20:
			pFont->Draw("(Vol = 20)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 30:
			pFont->Draw("(Vol = 30)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 40:
			pFont->Draw("(Vol = 40)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 50:
			pFont->Draw("(Vol = 50)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 60:
			pFont->Draw("(Vol = 60)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 70:
			pFont->Draw("(Vol = 70)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 80:
			pFont->Draw("(Vol = 80)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 90:
			pFont->Draw("(Vol = 90)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 100:
			pFont->Draw("(Vol = 100)", { 35, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		}

		pFont->Draw(" SoundFX\n    Volume", { width - 350, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });
	}
	else if (setFX)
	{
		pFont->Draw("Options", { (width - (4 * 28 * 4.0f)) / 2, 50 },
			2.0f, { 255, 255, 255 });

		pFont->Draw(" Master\n    Volume", { 35, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });

		pFont->Draw(" SoundFX\n    Volume", { width - 350, Game::GetInstance()->GetScreenHeight() / 2 },
			1.0f, { 255, 0, 0 });

		switch (VolumeFX)
		{
		case 0:
			pFont->Draw("(Vol = 0)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 10:
			pFont->Draw("(Vol = 10)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 20:
			pFont->Draw("(Vol = 20)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 30:
			pFont->Draw("(Vol = 30)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 40:
			pFont->Draw("(Vol = 40)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 50:
			pFont->Draw("(Vol = 50)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 60:
			pFont->Draw("(Vol = 60)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 70:
			pFont->Draw("(Vol = 70)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 80:
			pFont->Draw("(Vol = 80)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 90:
			pFont->Draw("(Vol = 90)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		case 100:
			pFont->Draw("(Vol = 100)", { width - 350, (Game::GetInstance()->GetScreenHeight() / 2) + 100 },
				1.0f, { 255, 0, 0 });
			break;
		}

	}
}