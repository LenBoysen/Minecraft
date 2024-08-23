#pragma once
#include "Input.h"




class WindowsInput : public Input
{
protected:
	virtual bool isKeyPressedImpl(int keycode) override;

	virtual bool isMouseButtonPressedImpl(int keycode) override;
	virtual std::pair<float, float> getMousePositionImpl() override;
	virtual float getMouseXImpl() override;
	virtual float getMouseYImpl() override;
	virtual void setMousePositionImpl(int x, int y) override;
	virtual void setHideCursorImpl(bool hide) override;
};

