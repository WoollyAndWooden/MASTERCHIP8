#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include "../EmulatorSrc/Display.h"

class EmulatorDisplay : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit EmulatorDisplay(QWidget *parent = nullptr);
    void setDisplay(SuperChip8::Display* display);

    void setColors(QColor pixel, QColor bg);
    void setGridEnabled(bool enabled);

    QColor getPixelColor() const { return m_pixelColor; }
    QColor getBackgroundColor() const { return m_bgColor; }
    bool isGridEnabled() const { return m_gridEnabled; }

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    SuperChip8::Display* m_display = nullptr;
    QColor m_pixelColor = Qt::yellow;
    QColor m_bgColor = Qt::black;
    bool m_gridEnabled = false;
};
