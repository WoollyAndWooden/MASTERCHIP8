#pragma once
#include <QMainWindow>
#include <memory>
#include <QTimer>
#include <QKeyEvent>
#include <QAudioSink>
#include <QIODevice>
#include "../EmulatorSrc/SuperChip8.h"

namespace Ui {
class MainWindow;
}

// Simple infinite beep generator
class BeepGenerator : public QIODevice
{
    Q_OBJECT
public:
    BeepGenerator(QObject* parent = nullptr);
    void start();
    void stop();
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;

private:
    qint64 m_pos = 0;
};

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

    // Audio
    QAudioSink* m_audioSink = nullptr;
    BeepGenerator* m_beepGen = nullptr;

    void setupDebugTable();
    void updateDebugInfo();
    int mapKey(int qtKey);
    void updateAudio();
};
