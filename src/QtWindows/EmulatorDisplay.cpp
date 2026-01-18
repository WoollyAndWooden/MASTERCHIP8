#include "EmulatorDisplay.h"
#include <QPainter>

EmulatorDisplay::EmulatorDisplay(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void EmulatorDisplay::setDisplay(SuperChip8::Display* display)
{
    m_display = display;
    update();
}

void EmulatorDisplay::setColors(QColor pixel, QColor bg)
{
    m_pixelColor = pixel;
    m_bgColor = bg;
    update();
}

void EmulatorDisplay::setGridEnabled(bool enabled)
{
    m_gridEnabled = enabled;
    update();
}

void EmulatorDisplay::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void EmulatorDisplay::resizeGL(int w, int h)
{
}

void EmulatorDisplay::paintGL()
{
    // Use the background color for clearing
    glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_display) {
        return;
    }

    QPainter painter(this);

    // CHIP-8 Standard resolution
    const int chipWidth = 64;
    const int chipHeight = 32;
    const float targetAspectRatio = static_cast<float>(chipWidth) / chipHeight;

    int w = width();
    int h = height();

    int drawWidth = w;
    int drawHeight = h;
    int xOffset = 0;
    int yOffset = 0;

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
    painter.setBrush(m_pixelColor);

    painter.translate(xOffset, yOffset);

    for (int y = 0; y < chipHeight; ++y) {
        for (int x = 0; x < chipWidth; ++x) {
            if (m_display->getPixel(x, y)) {
                if (m_gridEnabled) {
                    // Draw slightly smaller rect to show grid
                    // Reduce size by 10% or at least 1 pixel
                    float gapX = pixelW * 0.1f;
                    float gapY = pixelH * 0.1f;
                    if (gapX < 1.0f) gapX = 1.0f;
                    if (gapY < 1.0f) gapY = 1.0f;

                    painter.drawRect(QRectF(x * pixelW + gapX/2, y * pixelH + gapY/2, pixelW - gapX, pixelH - gapY));
                } else {
                    painter.drawRect(QRectF(x * pixelW, y * pixelH, pixelW, pixelH));
                }
            }
        }
    }
}
