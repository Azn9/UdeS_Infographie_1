#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum class KeyCode
{
	ARROW_LEFT = DIK_LEFT,
	ARROW_RIGHT = DIK_RIGHT,
	ARROW_UP = DIK_UP,
	ARROW_DOWN = DIK_DOWN,
	ESCAPE = DIK_ESCAPE,
	ENTER = DIK_RETURN,
	SPACE = DIK_SPACE
};

// Used to iterate over all the keycodes
static constexpr KeyCode KeyCodeValues[] = {
	KeyCode::ARROW_LEFT,
	KeyCode::ARROW_RIGHT,
	KeyCode::ARROW_UP,
	KeyCode::ARROW_DOWN,
	KeyCode::ESCAPE,
	KeyCode::ENTER,
	KeyCode::SPACE
};
