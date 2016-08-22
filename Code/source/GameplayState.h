/**********************************************************************
|	File:		GameplayState.h
|	Author:		Jose Rivera
|	Course:		SGD 1411
|	Purpose:	GameplayState class initializes & runs the game logic
**********************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

// Class Headers
#include "IGameState.h"							// uses IGameState

// Wrappers
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message

// C++ Headers

/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class Player;
class Boss;

/**************************************************************/
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:

	/**********************************************************/
	// Singleton Accessor:
	static GameplayState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

	void SetSpawnCount(unsigned int _sp) { spawnCount = _sp; }
	void SetEnemyBool(bool _enemy) { enemyOnScreen = _enemy; }
	bool GetBossBool(void) { return bossOnScreen; }

private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	GameplayState(void) = default;	// default constructor
	virtual ~GameplayState(void) = default;	// destructor

	GameplayState(const GameplayState&) = delete;	// copy constructor
	GameplayState& operator= (const GameplayState&) = delete;	// assignment operator

	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities = nullptr;

	/**********************************************************/
	// Message Callback Function:
	static void MessageProc(const SGD::Message* pMsg);

	/////////////////////////////////////////////////////////////////////////////////

	// Variables and Data
	bool IsPaused;
	bool enemyOnScreen = false;
	bool bossOnScreen = false;
	bool bossFifty = false;
	int m_nCursor;
	unsigned int spawnCount = 0;

	// Pointers
	Entity* newPlayer;
	Entity* newEnemy;
	Entity* newBoss;
	Entity* newProjectile;

	// SGD Image Handles
	SGD::HTexture GamePlay1_BG_IMG = SGD::INVALID_HANDLE;
	SGD::HTexture GamePlay2_BG_IMG = SGD::INVALID_HANDLE;
	SGD::HTexture PlayerIMG = SGD::INVALID_HANDLE;
	SGD::HTexture Player2IMG = SGD::INVALID_HANDLE;
	SGD::HTexture EnemyIMG = SGD::INVALID_HANDLE;
	SGD::HTexture BossIMG = SGD::INVALID_HANDLE;
	SGD::HTexture ProjectileIMG = SGD::INVALID_HANDLE;
	SGD::HTexture BossProjectileIMG = SGD::INVALID_HANDLE;
	SGD::HTexture HealthBarGreenIMG = SGD::INVALID_HANDLE;
	SGD::HTexture HealthBarYellowIMG = SGD::INVALID_HANDLE;
	SGD::HTexture HealthBarRedIMG = SGD::INVALID_HANDLE;

	// SGD Audio Handles
	SGD::HAudio GamePlayBackGround_MUS = SGD::INVALID_HANDLE;
	SGD::HAudio Victory_MUS = SGD::INVALID_HANDLE;
	SGD::HAudio CursorSelectionFX = SGD::INVALID_HANDLE;
	SGD::HAudio MenuSelectionFX = SGD::INVALID_HANDLE;
	SGD::HAudio ImpactFX = SGD::INVALID_HANDLE;

	// Entity methods
	Entity* CreatePlayer(void) const;
	Entity* CreateEnemy(void) const;
	Entity* CreateBoss(void) const;
	Entity* CreateProjectile1(Player* ptr) const;
	Entity* CreateBossProjectile1(Boss* ptr) const;
};

#endif //GAMEPLAYSTATE_H