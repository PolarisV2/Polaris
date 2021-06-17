#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <string>
#include <iostream>
#include <map>

#include "SDK.hpp"
#include "mainwindow.h"
#include "util.h"

namespace polaris
{
	class Player
	{
	public:
		SDK::AFortPlayerPawnAthena* m_pPlayerPawn;

		Player();

		void InitializeHero();
		void EquipWeapon(const char* cItemDef, int guid = 0);
	};
}

#endif // PLAYER_H