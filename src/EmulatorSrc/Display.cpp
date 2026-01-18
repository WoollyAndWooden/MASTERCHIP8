#include "Display.h"

namespace SuperChip8
{
    Display::Display()
    {
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 32; j++)
                pixels[i][j] = false;
        }
    }

    bool Display::flipPixel(int width, int height)
    {
        pixels[width][height] = !pixels[width][height];
        return pixels[width][height];
    }

    void Display::set_pixel(int width, int height, bool pixel)
    {
        pixels[width][height] = pixel;
    }

    bool Display::getPixel(int width, int height)
    {
        return pixels[width][height];
    }
}
