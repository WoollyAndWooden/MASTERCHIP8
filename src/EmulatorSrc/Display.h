#pragma once


class Display
{
public:
    Display();
    void setPixel(int width, int height);
private:
    bool pixels[128][64] = {};
};