#pragma once

namespace PM3D_API
{
    class Vector2
    {
    public:
        Vector2(const float x, const float y)
            : x(x),
              y(y)
        {
        }

        float GetX() const { return x; }
        float GetY() const { return y; }

        static Vector2 Zero() { return {0.0f, 0.0f}; }
        static Vector2 One() { return {1.0f, 1.0f}; }

    private:
        float x;
        float y;
    };
}
