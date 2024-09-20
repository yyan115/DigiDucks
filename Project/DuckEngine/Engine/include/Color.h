#pragma once

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color(float r = 255, float g = 255, float b = 255, float a = 255)
        : r(r), g(g), b(b), a(a) {}
};
