/*
* Copyright (c) 2021, Polaris All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef IMGUI_TEXT_EXTENSION_H
#define IMGUI_TEXT_EXTENSION_H

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

namespace ImGui
{
    static void Header(const char* text)
    {
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[2];

		ImGui::PushFont(headerFont);
		ImGui::Text(text);
		ImGui::PopFont();
    }

	static void Header(ImVec4 color, const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[2];

		ImGui::PushFont(headerFont);
		ImGui::TextColored(color, text);
		ImGui::PopFont();
	}

	static void Header2(const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[3];

		ImGui::PushFont(headerFont);
		ImGui::Text(text);
		ImGui::PopFont();
	}

	static void Header2(ImVec4 color, const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[3];

		ImGui::PushFont(headerFont);
		ImGui::TextColored(color, text);
		ImGui::PopFont();
	}

	static void SmallText(const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[1];

		ImGui::PushFont(headerFont);
		ImGui::Text(text);
		ImGui::PopFont();
	}

	static void SmallText(ImVec4 color, const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[1];

		ImGui::PushFont(headerFont);
		ImGui::TextColored(color, text);
		ImGui::PopFont();
	}

	static void BoldText(const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[4];

		ImGui::PushFont(headerFont);
		ImGui::Text(text);
		ImGui::PopFont();
	}

	static void BoldText(ImVec4 color, const char* text)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* headerFont = atlas->Fonts[4];

		ImGui::PushFont(headerFont);
		ImGui::TextColored(color, text);
		ImGui::PopFont();
	}
}

#endif // !IMGUI_TEXT_EXTENSION_H
