module;

#include <Windows.h>

#include <d3d11.h>
#include <dxgi.h>

#include <mutex>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <unordered_map>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

export module directx.hook;

import directx;
import kiero;

import oh.assets;
import oh.explorer;

static WNDPROC roblox_processor{};
static directx::present_t present{};
static directx::resize_buffers_t resize_buffers{};

static LRESULT __stdcall window_processor(HWND hwnd, std::uint32_t message, std::uintptr_t wparam, std::uintptr_t lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam)) {
		return true;
	}

	if (ImGui::IsAnyWindowHovered() && message == WM_MOUSEWHEEL) {
		return true;
	}

	if (ImGui::IsAnyItemActive() && ImGui::GetIO().WantCaptureKeyboard) {
		return true;
	}

	return CallWindowProc(roblox_processor, hwnd, message, wparam, lparam);
}

static std::filesystem::path root_path;

static HRESULT __stdcall present_hook(IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags) {
	static std::once_flag present_init{};

	std::call_once(present_init, [=]() {
		directx::init(swap_chain);
		directx::render_target render_target{swap_chain};

		DXGI_SWAP_CHAIN_DESC desc{};
		swap_chain->GetDesc(&desc);

		ImGui::CreateContext();
		ImGui::GetIO().ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		ImGui_ImplWin32_Init(desc.OutputWindow);
		directx::invoke_device(ImGui_ImplDX11_Init);

		directx::invoke_device([](ID3D11Device* device, ID3D11DeviceContext* context) {
			oh::assets::explorer::load_icons(root_path, device);
			oh::assets::types::load_icons(root_path, device);
		});

		oh::explorer::initialize(root_path);

		const auto fonts_folder = root_path / "assets" / "fonts";
		const auto& fonts = ImGui::GetIO().Fonts;

		fonts->Fonts.clear();
		fonts->AddFontFromFileTTF((fonts_folder / "roboto.ttf").string().c_str(), 16);

		roblox_processor = reinterpret_cast<WNDPROC>(SetWindowLongA(desc.OutputWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_processor)));
	}); 

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	oh::explorer::render();

	//for (ig::window& window : ig::windows) {
	//	window.render();
	//}

	if (ImGui::IsAnyWindowHovered()) {
		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImDrawList* draw_list = ImGui::GetOverlayDrawList();

		draw_list->RenderMouseCursor(mouse_pos, 1, ImGuiMouseCursor_Arrow, ImGui::GetColorU32({ 1, 1, 1, 1 }), ImGui::GetColorU32({ 0, 0, 0, 1 }), 0);
	}
	else if (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive()) {
		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImDrawList* draw_list = ImGui::GetOverlayDrawList();

		draw_list->RenderMouseCursor(mouse_pos, 1, ImGuiMouseCursor_Hand, ImGui::GetColorU32({ 1, 1, 1, 1 }), ImGui::GetColorU32({ 0, 0, 0, 1 }), 0);
	}

	ImGui::Render();

	directx::render_target::bind<directx::viewport_stage::output_merger>();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return present(swap_chain, sync_interval, flags);
}

static HRESULT __stdcall resize_buffers_hook(IDXGISwapChain* swap_chain, std::uint32_t count, std::uint32_t width, std::uint32_t height, DXGI_FORMAT format, std::uint32_t flags) {
	directx::render_target::clear();

	HRESULT result = resize_buffers(swap_chain, count, width, height, format, flags);
	directx::render_target render_target{swap_chain};
	
	render_target.bind<directx::viewport_stage::output_merger>();
	render_target.bind<directx::viewport_stage::rasterizer>();

	return result;
}

export namespace directx {
	void hook(const std::string& path) {
		root_path = path;

		AddVectoredExceptionHandler(1, [](EXCEPTION_POINTERS* exception) -> long {
			if (exception->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
				return EXCEPTION_CONTINUE_EXECUTION;
			}

			return EXCEPTION_CONTINUE_SEARCH;
		});

		while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::AlreadyInitializedError) {
			kiero::bind(8, reinterpret_cast<void**>(&present), present_hook);
			kiero::bind(13, reinterpret_cast<void**>(&resize_buffers), resize_buffers_hook);
		}
	}
}