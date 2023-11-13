#pragma once
#include "Api/Public/EventSystem/Event.h"

class GameOverEvent final : public PM3D_API::Event
{
public:
    explicit GameOverEvent(const bool won) : _won(won) {}

    [[nodiscard]] bool IsWon() const { return _won; }
private:
    bool _won;
};