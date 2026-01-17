#pragma once

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
        bool pixels[128][64];
    };
}
