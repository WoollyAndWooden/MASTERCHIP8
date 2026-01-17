#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include "../EmulatorSrc/Display.h" // Include the Display header

class EmulatorDisplay : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit EmulatorDisplay(QWidget *parent = nullptr);
    void setDisplay(SuperChip8::Display* display); // Method to set the display source

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    SuperChip8::Display* m_display = nullptr; // Pointer to the emulator's display
};
