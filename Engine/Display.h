#pragma once

#include "Window.h"

class Display : public Window
{
public:
	Display(Engine* engine);
	virtual ~Display();

	void Update();

	const SwapChainPtr& GetSwapChain();

protected:
	virtual void OnSize(const Rect& size) override;

private:
	SwapChainPtr m_swap_chain = nullptr;
	Engine* m_engine = nullptr;
};

