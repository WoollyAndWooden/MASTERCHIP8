#include "EmulatorDisplay.h"
#include <QPainter>

EmulatorDisplay::EmulatorDisplay(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void EmulatorDisplay::setDisplay(SuperChip8::Display* display)
{
    m_display = display;
    update(); // Trigger a repaint
}

void EmulatorDisplay::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

void EmulatorDisplay::resizeGL(int w, int h)
{
}

void EmulatorDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_display) {
        return; // Draw nothing (black) if no emulator is loaded
    }

    QPainter painter(this);

    // CHIP-8 Super resolution
    const int chipWidth = 128;
    const int chipHeight = 64;
    const float targetAspectRatio = static_cast<float>(chipWidth) / chipHeight;

    int w = width();
    int h = height();

    int drawWidth = w;
    int drawHeight = h;
    int xOffset = 0;
    int yOffset = 0;

    // Calculate aspect preserving dimensions (Letterboxing)
    if (static_cast<float>(w) / h > targetAspectRatio) {
        drawWidth = static_cast<int>(h * targetAspectRatio);
        xOffset = (w - drawWidth) / 2;
    } else {
        drawHeight = static_cast<int>(w / targetAspectRatio);
        yOffset = (h - drawHeight) / 2;
    }

    float pixelW = static_cast<float>(drawWidth) / chipWidth;
    float pixelH = static_cast<float>(drawHeight) / chipHeight;

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::yellow); // Classic monochrome color

    painter.translate(xOffset, yOffset);

    // Draw pixels from the Emulator's Display memory
    for (int y = 0; y < chipHeight; ++y) {
        for (int x = 0; x < chipWidth; ++x) {
            if (m_display->getPixel(x, y)) {
                painter.drawRect(QRectF(x * pixelW, y * pixelH, pixelW, pixelH));
            }
        }
    }
}
