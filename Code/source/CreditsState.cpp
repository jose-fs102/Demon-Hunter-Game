/***************************************************************
|	File:		CreditsState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	CreditState class handles the Credits menu
***************************************************************/


// Class Headers
#include "CreditsState.h"
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

// Preprocessor constant
#define Acceleration 85

// Singleton
CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState crS_instance;

	return &crS_instance;
}

// Load Resources
 void CreditsState::Enter(void)
{
	 // Set the parameters
	 xTitle = 50;
	 xLine1 = 200;
	 xLine2 = 300;
	 xLine3 = 400;
	 xLine4 = 500;
	 xLine5 = 600;

	 scrollingDone = false;

	 // Load Image
	 Credits_BG_IMG = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/JR_Credits_BG_IMG.png");
}

 // Unload Resources
 void CreditsState::Exit(void)
{
	 // Unload Image
	 SGD::GraphicsManager::GetInstance()->UnloadTexture(Credits_BG_IMG);
}

 // Handle player input
 bool CreditsState::Update(float elapsedTime)
 {
	 SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	 // Press Escape to quit
	 if (pInput->IsKeyPressed(SGD::Key::Escape) == true || scrollingDone)
	 {
		 // Change to Main Menu State
		 Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		 // Exit this state immediatly
		 return  true;
	 }

	 return true;
 }

 // Draw to the screen
 void CreditsState::Render(float elapsedTime)
 {
	 // Render Background Image
	 SGD::GraphicsManager::GetInstance()->DrawTexture(Credits_BG_IMG, { 0.0f, -100.0f });

	 // Use the game's font
	 const BitmapFont* pFont = Game::GetInstance()->GetFont();

	 // Align text based on window width
	 float width = Game::GetInstance()->GetScreenWidth();

	 pFont->Draw("Credits", { (width - (4 * 28 * 4.0f)) / 2, xTitle },
		 2.0f, { 255, 255, 255 });
	 xTitle -= elapsedTime * Acceleration;

	 pFont->Draw("Jose Rivera", { 35, xLine1 },
		 1.0f, { 255, 0, 0 });
	 xLine1 -= elapsedTime * Acceleration;

	 pFont->Draw("Structure of game design", { 100, xLine2 },
		 1.0f, { 255, 0, 0 });
	 xLine2 -= elapsedTime * Acceleration;

	 pFont->Draw("At full sail university", { 200, xLine3 },
		 1.0f, { 255, 0, 0 });
	 xLine3 -= elapsedTime * Acceleration;

	 pFont->Draw("Images and sounds come from", { 35, xLine4 },
		 1.0f, { 255, 0, 0 });
	 xLine4 -= elapsedTime * Acceleration;

	 pFont->Draw("The Internet", { 100, xLine5 },
		 1.0f, { 255, 0, 0 });
	 xLine5 -= elapsedTime * Acceleration;

	 if (xLine5 <= -60)
		 scrollingDone = true;
 }