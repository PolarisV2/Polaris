/*
 * [irma] This class is unfinished, but I plan to have a system here that loads the main Polaris behaviour when the player readies up
*/

#ifndef FRONTENDMANAGER_H
#define FRONTENDMANAGER_H

#include "common.h"

namespace polaris 
{
	class FrontendManager 
	{
	public:
		FrontendManager();
		~FrontendManager();
	};
}

static polaris::FrontendManager* gpFrontendManager;

#endif // FRONTENDMANAGER_H
