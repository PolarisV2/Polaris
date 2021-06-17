#ifndef WATERMARK_H
#define WATERMARK_H

#include "common.h"
#include "window.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "renderer.h"
#include "polarisflags.h"

namespace polaris
{
	class Watermark : public Window
	{
	public:
		Watermark();
		~Watermark();
		void Draw() override;
	};
}

#undef STB_IMAGE_IMPLEMENTATION
#endif // WATERMARK_H