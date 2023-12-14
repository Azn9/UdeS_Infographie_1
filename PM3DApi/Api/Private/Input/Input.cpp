#include "Api/Public/Input/Input.h"

#include "MouseClickEvent.h"
#include "Core/Public/Core/CDIManipulateur.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/EventSystem/EventSystem.h"

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

    /*
    mouseLeftReleased = false;
    mouseRightReleased = false;

    const auto [lX, lY, lZ, rgbButtons] = cdiManipulateur.EtatSouris();

    if (rgbButtons[0])
    {
        if (mouseLeftPressed) mouseLeftHeld = true;
        else
        {
            mouseLeftPressed = true;
            PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, true, true));
        }
    }
    else
    {
        if (mouseLeftPressed)
        {
            mouseLeftPressed = false;
            mouseLeftReleased = true;
            PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, true, false));
        }
    }

    if (rgbButtons[1])
    {
        if (mouseRightPressed) mouseRightHeld = true;
        else
        {
            mouseRightPressed = true;
            PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, false, true));
        }
    }
    else
    {
        if (mouseRightPressed)
        {
            mouseRightPressed = false;
            mouseRightReleased = true;
            PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, false, false));
        }
    }
    */
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
    return mouseX;
}

int Input::getMouseY() const
{
    return mouseY;
}

float Input::getMouseXPercentage() const
{
    return static_cast<float>(mouseX) / PM3D_API::GameHost::GetInstance()->GetScreenWidth();
}

float Input::getMouseYPercentage() const
{
    return static_cast<float>(mouseY) / PM3D_API::GameHost::GetInstance()->GetScreenHeight();
}

double Input::getMouseDeltaX() const
{
    return deltaMouseX;
}

double Input::getMouseDeltaY() const
{
    return deltaMouseY;
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

void Input::SetMousePosition(const int x, const int y)
{
    deltaMouseX = x - mouseX;
    deltaMouseY = y - mouseY;

    mouseX = x;
    mouseY = y;

    if (const auto canvas = PM3D_API::GameHost::GetInstance()->GetScene()->GetUICanvas())
        canvas->UpdateHover(x, y);
}

void Input::SetMouseLeftPressed(const bool cond)
{
    mouseLeftPressed = cond;
    PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, true, cond));
}

void Input::SetMouseRightPressed(const bool cond)
{
    mouseRightPressed = cond;
    PM3D_API::EventSystem::GetInstance().publish(MouseClickEvent(mouseX, mouseY, false, cond));
}
