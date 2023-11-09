#include "../../Public/Input/Input.h"

#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../PetitMoteur3D/Core/Public/Core/CDIManipulateur.h"
#include "../../Public/GameHost.h"

struct ImGuiIO;

void Input::Tick(CDIManipulateur& cdiManipulateur)
{
	currentlyReleasedKeys.clear();
	
	for (const KeyCode& keyCode : KeyCodeValues)
	{
		if (cdiManipulateur.ToucheAppuyee(static_cast<UINT>(keyCode)))
		{
			if (currentlyHeldKeys.contains(keyCode))
			{
				continue;
			}
			
			if (currentlyPressedKeys.contains(keyCode))
			{
				currentlyHeldKeys.insert(keyCode);
				currentlyPressedKeys.erase(keyCode);
			}
			else
			{
				currentlyPressedKeys.insert(keyCode);

				for (const auto& listener : keyPressedListeners)
				{
					listener(const_cast<KeyCode&>(keyCode));
				}
			}
		}
		else
		{
			if (currentlyHeldKeys.contains(keyCode) || currentlyPressedKeys.contains(keyCode))
			{
				currentlyPressedKeys.erase(keyCode);
				currentlyHeldKeys.erase(keyCode);
				currentlyReleasedKeys.insert(keyCode);

				for (const auto& listener : keyReleasedListeners)
				{
					listener(const_cast<KeyCode&>(keyCode));
				}
			}
		}
	}

	mouseLeftReleased = false;
	mouseRightReleased = false;

	const auto [lX, lY, lZ, rgbButtons] = cdiManipulateur.EtatSouris();
	
	if (rgbButtons[0])
	{
		if (mouseLeftPressed) mouseLeftHeld = true;
		else mouseLeftPressed = true;
	} else
	{
		if (mouseLeftPressed)
		{
			mouseLeftPressed = false;
			mouseLeftReleased = true;
		}
	}

	if (rgbButtons[1])
	{
		if (mouseRightPressed) mouseRightHeld = true;
		else mouseRightPressed = true;
	} else
	{
		if (mouseRightPressed)
		{
			mouseRightPressed = false;
			mouseRightReleased = true;
		}
	}
}

bool Input::isKeyPressed(const KeyCode keyCode) const
{
	return currentlyPressedKeys.contains(keyCode);
}

bool Input::isKeyReleased(const KeyCode keyCode) const
{
	return currentlyReleasedKeys.contains(keyCode);
}

bool Input::isKeyHeld(const KeyCode keyCode) const
{
	return currentlyHeldKeys.contains(keyCode);
}

int Input::getMouseX() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return static_cast<int>(io.MousePos.x);
}

int Input::getMouseY() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return static_cast<int>(io.MousePos.y);
}

float Input::getMouseXPercentage() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.MousePos.x / PM3D_API::GameHost::GetInstance()->GetScreenWidth();
}

float Input::getMouseYPercentage() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.MousePos.y / PM3D_API::GameHost::GetInstance()->GetScreenHeight();
}

double Input::getMouseDeltaX() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.MouseDelta.x;
}

double Input::getMouseDeltaY() const
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.MouseDelta.y;
}

bool Input::isMouseLeftPressed() const
{
	return mouseLeftPressed;
}

bool Input::isMouseLeftReleased() const
{
	return mouseLeftReleased;
}

bool Input::isMouseLeftHeld() const
{
	return mouseLeftHeld;
}

bool Input::isMouseRightPressed() const
{
	return mouseRightPressed;
}

bool Input::isMouseRightReleased() const
{
	return mouseRightReleased;
}

bool Input::isMouseRightHeld() const
{
	return mouseRightHeld;
}

void Input::removeKeyPressedListener(const std::function<void(KeyCode&)>& listener)
{
	std::erase_if(keyPressedListeners, [&](const std::function<void(KeyCode&)>& f)
	{
		return f.target<std::function<void(KeyCode&)>>() == listener.target<std::function<void(KeyCode&)>>();
	});
}

void Input::removeKeyReleasedListener(const std::function<void(KeyCode&)>& listener)
{
	std::erase_if(keyReleasedListeners, [&](const std::function<void(KeyCode&)>& f)
		{
			return f.target<std::function<void(KeyCode&)>>() == listener.target<std::function<void(KeyCode&)>>();
		});
}
