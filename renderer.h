#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "window.h"

#include <list>

namespace polaris
{
	class Renderer
	{
	public:
		static std::list<polaris::Window*> pUiInstances;

		ID3D11Device* pCurrentDevice = nullptr;
		ID3D11DeviceContext* pCurrentContext = nullptr;
		ID3D11RenderTargetView* pCurrentView = nullptr;

		Renderer();
		~Renderer();

		static void SubscribeUIToEvents(Window* ui)
		{
			pUiInstances.push_back(ui);
		}
	};
}

static polaris::Renderer* gpRenderer;

#endif // RENDERER_H
