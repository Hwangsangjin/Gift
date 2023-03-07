#pragma once

#include "Window.h"

class Display : public Window
{
public:
	Display(App* app);
	virtual ~Display();

	void Update();

	const SwapChainPtr& GetSwapChain();

protected:
	virtual void OnSize(const Rect& size) override;

private:
	SwapChainPtr m_swap_chain = nullptr;
	App* m_app = nullptr;
};

