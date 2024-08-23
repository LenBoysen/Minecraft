#pragma once
#include <utility>

#include "KeyCodes.h"
#include "MouseButtonCodes.h"

class Input {
public:

	inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }

	inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
	inline static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }
	inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
	inline static float getMouseY() { return s_Instance->getMouseYImpl(); }
	inline static void setMousePosition(int x, int y) { s_Instance->setMousePositionImpl(x, y); }
	inline static void setHideCursor(bool hide) { s_Instance->setHideCursorImpl(hide); }


protected:
	virtual bool isKeyPressedImpl(int keycode) = 0;

	virtual bool isMouseButtonPressedImpl(int button) = 0;
	virtual std::pair<float, float> getMousePositionImpl() = 0;
	virtual float getMouseXImpl() = 0;
	virtual float getMouseYImpl() = 0;
	virtual void setMousePositionImpl(int x, int y) = 0;
	virtual void setHideCursorImpl(bool hide) = 0;

private:
	static Input* s_Instance;
};