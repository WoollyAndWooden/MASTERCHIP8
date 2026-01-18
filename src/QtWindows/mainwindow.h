#pragma once
#include <QMainWindow>
#include <memory>
#include <QTimer>
#include <QKeyEvent>
#include "../EmulatorSrc/SuperChip8.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    std::unique_ptr<SuperChip8::SuperChip8> emulator;
    QTimer* emulationTimer;
    int currentCycles = 10;

    void setupDebugTable();
    void updateDebugInfo();
    int mapKey(int qtKey);
};
