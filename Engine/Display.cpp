#include "pch.h"
#include "Display.h"
#include "App.h"
#include "Engine.h"
#include "Graphics.h"
#include "SwapChain.h"

Display::Display(App* game)
	: m_app(game)
{
	auto client_size = GetClientSize();
	m_swap_chain = game->GetEngine()->GetGraphics()->CreateSwapChain(static_cast<HWND>(m_hwnd), client_size.m_width, client_size.m_height);
}

Display::~Display()
{
}

const SwapChainPtr& Display::GetSwapChain()
{
	return m_swap_chain;
}

void Display::OnSize(const Rect& size)
{
	m_swap_chain->Resize(size.m_width, size.m_height);
	m_app->Resize(size);
}
