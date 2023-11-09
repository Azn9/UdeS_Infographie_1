#pragma once
#include <functional>
#include <set>
#include <vector>

#include "KeyCode.h"
#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Singleton.h"

struct ImGuiIO;
class CDIManipulateur;

class Input : public PM3D::CSingleton<Input>
{
public:
	Input() = default;
	~Input() = default;

	void Tick(CDIManipulateur& cdiManipulateur);

	[[nodiscard]] bool isKeyPressed(KeyCode keyCode) const;
	[[nodiscard]] bool isKeyReleased(KeyCode keyCode) const;
	[[nodiscard]] bool isKeyHeld(KeyCode keyCode) const;

	[[nodiscard]] int getMouseX() const;
	[[nodiscard]] int getMouseY() const;
	[[nodiscard]] float getMouseXPercentage() const;
	[[nodiscard]] float getMouseYPercentage() const;
	[[nodiscard]] double getMouseDeltaX() const;
	[[nodiscard]] double getMouseDeltaY() const;
	[[nodiscard]] bool isMouseLeftPressed() const;
	[[nodiscard]] bool isMouseLeftReleased() const;
	[[nodiscard]] bool isMouseLeftHeld() const;
	[[nodiscard]] bool isMouseRightPressed() const;
	[[nodiscard]] bool isMouseRightReleased() const;
	[[nodiscard]] bool isMouseRightHeld() const;

	void addKeyPressedListener(const std::function<void(KeyCode&)>& listener) { keyPressedListeners.push_back(listener); }
	void addKeyReleasedListener(const std::function<void(KeyCode&)>& listener) { keyReleasedListeners.push_back(listener); }
	void removeKeyPressedListener(const std::function<void(KeyCode&)>& listener);
	void removeKeyReleasedListener(const std::function<void(KeyCode&)>& listener);

	static bool IsKeyPressed(const KeyCode keyCode) { return GetInstance().isKeyPressed(keyCode);	}
	static bool IsKeyReleased(const KeyCode keyCode) { return GetInstance().isKeyReleased(keyCode); }
	static bool IsKeyHeld(const KeyCode keyCode) { return GetInstance().isKeyHeld(keyCode); }

	static int GetMouseX() { return GetInstance().getMouseX(); }
	static int GetMouseY() { return GetInstance().getMouseY(); }
	static float GetMouseXPercentage() { return GetInstance().getMouseXPercentage(); }
	static float GetMouseYPercentage() { return GetInstance().getMouseYPercentage(); }
	static double GetMouseDeltaX() { return GetInstance().getMouseDeltaX(); }
	static double GetMouseDeltaY() { return GetInstance().getMouseDeltaY(); }
	static bool IsMouseLeftPressed() { return GetInstance().isMouseLeftPressed(); }
	static bool IsMouseLeftReleased() { return GetInstance().isMouseLeftReleased(); }
	static bool IsMouseLeftHeld() { return GetInstance().isMouseLeftHeld(); }
	static bool IsMouseRightPressed() { return GetInstance().isMouseRightPressed(); }
	static bool IsMouseRightReleased() { return GetInstance().isMouseRightReleased(); }
	static bool IsMouseRightHeld() { return GetInstance().isMouseRightHeld(); }

	static void AddKeyPressedListener(const std::function<void(KeyCode&)>& listener) { GetInstance().addKeyPressedListener(listener); }
	static void AddKeyReleasedListener(const std::function<void(KeyCode&)>& listener) { GetInstance().addKeyReleasedListener(listener); }

protected:

private:
	std::set<KeyCode> currentlyPressedKeys;
	std::set<KeyCode> currentlyHeldKeys;
	std::set<KeyCode> currentlyReleasedKeys;
	std::vector<std::function<void(KeyCode&)>> keyPressedListeners;
	std::vector<std::function<void(KeyCode&)>> keyReleasedListeners;
	
	bool mouseLeftPressed = false;
	bool mouseLeftHeld = false;
	bool mouseLeftReleased = false;
	bool mouseRightPressed = false;
	bool mouseRightHeld = false;
	bool mouseRightReleased = false;
	
};
