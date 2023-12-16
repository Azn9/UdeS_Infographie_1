#pragma once
#include "Api/Public/EventSystem/Event.h"

class CheckpointEvent_1 final : public PM3D_API::Event
{
public:
    explicit CheckpointEvent_1(const bool checked) : _checked(checked)
    {
    }

    [[nodiscard]] bool IsWon() const { return _checked; }

private:
    bool _checked;
};

class CheckpointEvent_2 final : public PM3D_API::Event
{
public:
    explicit CheckpointEvent_2(const bool checked) : _checked(checked)
    {
    }

    [[nodiscard]] bool IsWon() const { return _checked; }

private:
    bool _checked;
};

class CheckpointEvent_3 final : public PM3D_API::Event
{
public:
    explicit CheckpointEvent_3(const bool checked) : _checked(checked)
    {
    }

    [[nodiscard]] bool IsWon() const { return _checked; }

private:
    bool _checked;
};

class CheckpointEvent_4 final : public PM3D_API::Event
{
public:
    explicit CheckpointEvent_4(const bool checked) : _checked(checked)
    {
    }

    [[nodiscard]] bool IsWon() const { return _checked; }

private:
    bool _checked;
};

class CheckpointEvent_5 final : public PM3D_API::Event
{
public:
    explicit CheckpointEvent_5(const bool checked) : _checked(checked)
    {
    }

    [[nodiscard]] bool IsWon() const { return _checked; }

private:
    bool _checked;
};
