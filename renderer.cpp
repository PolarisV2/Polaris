#include "renderer.h"
#include "mainwindow.h"

#include <windows.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "platform.h"

polaris::MainWindow* pMainWindow;
bool bLockFortInput;
std::list<polaris::Window*> polaris::Renderer::pUiInstances;

WNDPROC lpPrevWndFunc;
static HWND hWnd = 0;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) LRESULT CALLBACK WndProcHook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (Msg == WM_KEYUP && (wParam == VK_HOME || (bLockFortInput && wParam == VK_ESCAPE))) {
		bLockFortInput = !bLockFortInput;

		ImGui::GetIO().MouseDrawCursor = bLockFortInput;
		pMainWindow->m_bShowWindow = bLockFortInput;
	}

	if (bLockFortInput)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

		return TRUE;
	}

	return CallWindowProc(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
}

// Rendering hook used to draw on top of the Fortnite window.
HRESULT(*Present)(IDXGISwapChain* pInstance, UINT SyncInterval, UINT Flags) = nullptr;
__declspec(dllexport) HRESULT PresentHook(IDXGISwapChain* pInstance, UINT SyncInterval, UINT Flags)
{
	static float fWidth = 0;
	static float fHeight = 0;

	// Poll and handle messages (inputs, window resize, etc)
	static MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	if (::PeekMessage(&msg, hWnd, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	// Jesus fucking christ.
	if (!gpRenderer->pCurrentDevice)
	{
		pInstance->GetDevice(__uuidof(gpRenderer->pCurrentDevice), reinterpret_cast<PVOID*>(&gpRenderer->pCurrentDevice));
		gpRenderer->pCurrentDevice->GetImmediateContext(&gpRenderer->pCurrentContext);

		ID3D11Texture2D* pTarget = nullptr;
		pInstance->GetBuffer(0, __uuidof(pTarget), reinterpret_cast<PVOID*>(&pTarget));

		gpRenderer->pCurrentDevice->CreateRenderTargetView(pTarget, nullptr, &gpRenderer->pCurrentView);

		pTarget->Release();

		ID3D11Texture2D* pBuffer = nullptr;
		pInstance->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<PVOID*>(&pBuffer));

		D3D11_TEXTURE2D_DESC desc = { };
		pBuffer->GetDesc(&desc);

		if (!fWidth)
			lpPrevWndFunc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook)));

		fWidth = static_cast<float>(desc.Width);
		fHeight = static_cast<float>(desc.Height);

		pBuffer->Release();

		// Handle application-specific workload.
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(gpRenderer->pCurrentDevice, gpRenderer->pCurrentContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Invoke the Draw event on all subscribed Uis.
	for (polaris::Window* ui : polaris::Renderer::pUiInstances)
	{
		if (ui->m_bShowWindow)
		{
			if (!bLockFortInput)
			{
				ImGui::SetNextWindowBgAlpha(0.5f);
			}

			ui->Draw();
		}
	}

	ImGui::SetNextWindowBgAlpha(1);

	ImGui::Render();
	gpRenderer->pCurrentContext->OMSetRenderTargets(1, &gpRenderer->pCurrentView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return Present(pInstance, SyncInterval, Flags);
}

// Resize hook.
HRESULT(*ResizeBuffers)(IDXGISwapChain* pInstance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) = nullptr;
__declspec(dllexport) HRESULT ResizeBuffersHook(IDXGISwapChain* pInstance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	// Invoke the Resize event on all subscribed Uis.
	for (polaris::Window* ui : polaris::Renderer::pUiInstances)
	{
		ui->Resize();
	}

	ImGui_ImplDX11_Shutdown();

	gpRenderer->pCurrentView->Release();
	gpRenderer->pCurrentContext->Release();
	gpRenderer->pCurrentDevice->Release();

	gpRenderer->pCurrentDevice = nullptr;

	return ResizeBuffers(pInstance, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

namespace polaris
{
	Renderer::Renderer()
	{
		Console::Log("Initializing Renderer");

		pMainWindow = new polaris::MainWindow;

		// Check if our renderer is already initialized.
		if (gpRenderer)
		{
			Util::ThrowFatalError(L"Renderer is already initialized!");
		}

		gpRenderer = this;

		IDXGISwapChain* pSwapChain;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;

		auto featureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC desc = { };

		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;

		hWnd = FindWindow(L"UnrealWindow", L"Fortnite ");

		desc.OutputWindow = hWnd;
		desc.Windowed = true;

		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Initialize and check if we failed to initialize our DirectX 11 device.
		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &desc, &pSwapChain, &pDevice, nullptr, &pContext)))
		{
			Util::ThrowFatalError(L"Failed to create DX11 device.");
		}

		auto pTable = *reinterpret_cast<PVOID**>(pSwapChain);

		auto pPresent = pTable[8];
		auto pResizeBuffers = pTable[13];

		// Prepare ImGui.
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Load Segoe UI as font.
		io.Fonts->AddFontFromFileTTF(Util::GetConcatPath(Platform::GetFontsDir(), "segoeui.ttf").c_str(), 20);

		pSwapChain->Release();
		pDevice->Release();
		pContext->Release();

		MH_CreateHook(pPresent, PresentHook, reinterpret_cast<PVOID*>(&Present));
		MH_EnableHook(pPresent);

		MH_CreateHook(pResizeBuffers, ResizeBuffersHook, reinterpret_cast<PVOID*>(&ResizeBuffers));
		MH_EnableHook(pResizeBuffers);
	}

	Renderer::~Renderer()
	{
		gpRenderer = nullptr;

		auto pPresent = reinterpret_cast<PVOID*>(&Present);

		MH_DisableHook(pPresent);
		MH_RemoveHook(pPresent);

		auto pResizeBuffers = reinterpret_cast<PVOID*>(&ResizeBuffers);

		MH_DisableHook(pResizeBuffers);
		MH_RemoveHook(pResizeBuffers);
	}
}
