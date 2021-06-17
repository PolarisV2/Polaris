#ifndef CREDITSWINDOW_H
#define CREDITSWINDOW_H

#include "common.h"
#include "window.h"

namespace polaris
{
	class CreditsWindow : public Window
	{
	public:
		const char* credits[18] = {
			"Creator:",
			"@uguuCyuubi",
			"",
			"Developer:",
			"@omairma",
			"",
			"Contributors:",
			"@BattleDashBR",
			"@kann3r",
			"@pivotman319",
			"@wiktorwiktor12a",
			"",
			"Special Thanks:",
			"@crusherrrz",
			"@DarkbladeEU",
			"@doshisgone",
			"@notsamiccc",
			"@Slushia"
		};

		CreditsWindow();
		~CreditsWindow();
		void Draw() override;
	};
}

#endif // CREDITSWINDOW_H
