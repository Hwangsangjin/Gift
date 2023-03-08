#include "pch.h"
#include "Display.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "SwapChain.h"

Display::Display(Engine* engine)
	: m_engine(engine)
{
	auto client_size = GetClientSize();
	m_swap_chain = engine->GetRenderSystem()->CreateSwapChain(static_cast<HWND>(m_hwnd), client_size.m_width, client_size.m_height);

    // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_engine->GetRenderSystem()->GetD3DDevice(), m_engine->GetRenderSystem()->GetImmediateContext());
}

Display::~Display()
{
    // ImGui Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Display::Update()
{
	// Assemble Together Draw Data
	ImGui::Render();
	// Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Page Flipping
	m_swap_chain->Present(true);
}

const SwapChainPtr& Display::GetSwapChain()
{
	return m_swap_chain;
}

void Display::OnSize(const Rect& size)
{
	m_swap_chain->Resize(size.m_width, size.m_height);
	m_engine->Resize(size);
}
