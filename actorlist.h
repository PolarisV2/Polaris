#ifndef ACTORLIST_H
#define ACTORLIST_H

#include "common.h"
#include "window.h"

#include <imgui.h>

namespace polaris
{
	class ActorList : public Window
	{
	public:
		ActorList();
		void Draw() override;
	};
}

#endif // ACTORLIST_H
