#pragma once
#include <array>

namespace SuperChip8
{
    class Display
    {
    public:
        Display();
        bool flipPixel(int width, int height);
        void set_pixel(int width, int height, bool pixel);
        bool getPixel(int width, int height);
    private:
        std::array<std::array<bool, 128>, 64> pixels;
    };
}
