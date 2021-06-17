#ifndef OBJECTCACHE_H
#define OBJECTCACHE_H

#include "common.h"
#include "window.h"

#include <imgui.h>

namespace polaris
{
	class ObjectCache : public Window
	{
	public:
		ObjectCache();
		~ObjectCache();
		void Draw() override;
	};
}

#endif // OBJECTCACHE_H
