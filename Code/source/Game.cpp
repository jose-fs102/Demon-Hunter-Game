/***************************************************************
|	File:		Game.cpp
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

// Class Headers
#include "Game.h"
#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"

// Wrappers
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

// C++ Headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <cassert>

/**************************************************************/
// Singleton
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the ONE instance & return it
/*static*/ Game* Game::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- enter the Main Menu state
bool Game::Initialize(float width, float height)
{
	// Seed First!
	srand((unsigned int)time(nullptr));
	rand();

	// Initialize the wrappers
	if (SGD::AudioManager::GetInstance()->Initialize() == false
		|| SGD::GraphicsManager::GetInstance()->Initialize(false) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false)
		return false;


	// Store the size parameters
	m_fScreenWidth = width;
	m_fScreenHeight = height;

	// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

	MenuSelectionFX = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/JR_MenuSelectionFX.wav");

	// Start the game in the Main Menu state
	ChangeState(MainMenuState::GetInstance());

	// Store the current time (in milliseconds)
	m_ulGameTime = GetTickCount();

	return true;	// success!
}

/**************************************************************/
// Update
//	- update the SGD wrappers
//	- run the current state
int Game::Update(void)
{
	// Update the wrappers
	if (SGD::AudioManager::GetInstance()->Update() == false
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
		return -10;		// exit FAILURE!


	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;	// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

	// Cap the elapsed time to 1/8th of a second
	if (elapsedTime > 0.125f)
		elapsedTime = 0.125f;

	// Toggle Fullscreen mode on and off
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt) == true
		&& SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) == true)
	{
		SGD::GraphicsManager::GetInstance()->Resize(SGD::Size{ m_fScreenWidth, m_fScreenHeight }, fullScreen);
		fullScreen = !fullScreen;
		return 0;
	}

	// Let the current state handle input
	if (m_pCurrState->Update(elapsedTime) == false)
		return 1;	// exit success!

	// Let the current state render
	m_pCurrState->Render(elapsedTime);

	return 0;		// keep playing!
}

/**************************************************************/
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate(void)
{
	// Exit the current state
	ChangeState(nullptr);

	// Unload Audio
	SGD::AudioManager::GetInstance()->UnloadAudio(MenuSelectionFX);

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

	// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
}

/**************************************************************/
// ChangeState
//	- exit the current state to release resources
//	- enter the new state to allocate resources
//	- DANGER! Exiting the current state can CRASH the program!
//	  The state can ONLY be exited from the
//	  Input, Update, and Render methods!!!
void Game::ChangeState(IGameState* pNewState)
{
	// Exit the old state
	if (m_pCurrState != nullptr)
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	if (!firstchoice)
		SGD::AudioManager::GetInstance()->PlayAudio(MenuSelectionFX);

	// Enter the new state
	if (m_pCurrState != nullptr)
		m_pCurrState->Enter();
}