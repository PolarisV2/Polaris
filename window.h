#ifndef UI_H
#define UI_H

#include <string>

namespace polaris
{
	class Window
	{
	public:
		bool m_bShowWindow = false;
		bool m_bInteractable = true;

		Window();
		virtual void Draw();
		virtual void Resize();
	};
}

#endif // UI_H
