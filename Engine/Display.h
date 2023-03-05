#pragma once

#include "Window.h"
#include "Graphics.h"

class Display : public Window
{
public:
	Display(App* app);
	virtual ~Display() override;

	const SwapChainPtr& GetSwapChain();

protected:
	virtual void OnSize(const Rect& size);

private:
	SwapChainPtr m_swap_chain = nullptr;
	App* m_app = nullptr;
};

