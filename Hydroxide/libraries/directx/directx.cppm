module;

#include <d3d11.h>
#include <dxgi.h>

#include <iostream>

export module directx;

ID3D11RenderTargetView* render_target_view = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* device_context = nullptr;

export namespace directx {
	using present_t = HRESULT(__stdcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t);
	using resize_buffers_t = HRESULT(__stdcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t, std::uint32_t, DXGI_FORMAT, std::uint32_t);

	enum class viewport_stage {
		rasterizer,
		output_merger
	};

	void init(IDXGISwapChain* swap_chain) {
		HRESULT got_device = swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
		device->GetImmediateContext(&device_context);
	}

	auto invoke_device(auto callback) {
		callback(device, device_context);
	}

	class render_target {
	public:
		render_target(IDXGISwapChain* swap_chain) {
			ID3D11Texture2D* buffer = nullptr;
			swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&buffer));

			if (buffer != nullptr) {
				device->CreateRenderTargetView(buffer, nullptr, &render_target_view);
				buffer->Release();
			}
		}

		static void clear() {
			device_context->OMSetRenderTargets(0, nullptr, nullptr);
			render_target_view->Release();
		}

		template<viewport_stage stage>
		static constexpr void bind() {
			if constexpr (stage == viewport_stage::output_merger) {
				device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
			}
			else if constexpr (stage == viewport_stage::rasterizer) {
				D3D11_VIEWPORT vp{
					.TopLeftX = 0,
					.TopLeftY = 0,
					.Width = 0,
					.Height = 0,
					.MinDepth = 0.0f,
					.MaxDepth = 1.0f,
				};
				device_context->RSSetViewports(1, &vp);
			}
		}
	};
}