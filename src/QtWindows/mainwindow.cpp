#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SettingsDialog.h"
#include <portable-file-dialogs.h>
#include <QMediaDevices>
#include <QAudioDevice>

// --- BeepGenerator Implementation ---
BeepGenerator::BeepGenerator(QObject* parent) : QIODevice(parent)
{
}

void BeepGenerator::start()
{
    open(QIODevice::ReadOnly);
}

void BeepGenerator::stop()
{
    close();
}

qint64 BeepGenerator::readData(char *data, qint64 maxlen)
{
    // Generate square wave (440Hz at 44100Hz sample rate)
    // Period = 100 samples
    const int period = 100;

    for (qint64 i = 0; i < maxlen; ++i) {
        if ((m_pos % period) < (period / 2)) {
            data[i] = 127; // High
        } else {
            data[i] = -128; // Low
        }
        m_pos++;
    }
    return maxlen;
}

qint64 BeepGenerator::writeData(const char *data, qint64 len)
{
    return 0;
}

qint64 BeepGenerator::bytesAvailable() const
{
    return 1024 + QIODevice::bytesAvailable(); // Always have data
}

// --- MainWindow Implementation ---

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDebugTable();

    // Initialize Audio
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);

    QAudioDevice device = QMediaDevices::defaultAudioOutput();
    if (!device.isFormatSupported(format)) {
        format = device.preferredFormat();
    }

    m_audioSink = new QAudioSink(device, format, this);
    m_beepGen = new BeepGenerator(this);
    m_beepGen->start();

    // Do NOT start audio here to prevent initial beep
    // It will be started in updateAudio() when needed

    // Initialize the timer
    emulationTimer = new QTimer(this);
    connect(emulationTimer, &QTimer::timeout, this, [this]() {
        if (emulator) {
            emulator->Tick();
            ui->MasterChip8Display->update();
            updateDebugInfo();
            updateAudio();
        }
    });

    // Button Connections
    connect(ui->btnPause, &QPushButton::clicked, this, [this]() {
        if (emulationTimer->isActive()) {
            emulationTimer->stop();
            if (m_audioSink->state() == QAudio::ActiveState) {
                m_audioSink->suspend();
            }
            ui->btnDecreaseCycles->setEnabled(true);
            ui->btnIncreaseCycles->setEnabled(true);
        }
    });

    connect(ui->btnResume, &QPushButton::clicked, this, [this]() {
        if (!emulationTimer->isActive() && emulator) {
            emulationTimer->start(16);
            ui->btnDecreaseCycles->setEnabled(false);
            ui->btnIncreaseCycles->setEnabled(false);
        }
    });

    connect(ui->btnNext, &QPushButton::clicked, this, [this]() {
        if (emulator) {
            emulator->Tick();
            ui->MasterChip8Display->update();
            updateDebugInfo();
        }
    });

    // Cycle Control Buttons
    connect(ui->btnIncreaseCycles, &QPushButton::clicked, this, [this]() {
        currentCycles++;
        ui->lblCycles->setText(QString::number(currentCycles));
        if (emulator) {
            emulator->change_cycles_per_tick(currentCycles);
        }
    });

    connect(ui->btnDecreaseCycles, &QPushButton::clicked, this, [this]() {
        if (currentCycles > 1) {
            currentCycles--;
            ui->lblCycles->setText(QString::number(currentCycles));
            if (emulator) {
                emulator->change_cycles_per_tick(currentCycles);
            }
        }
    });

    connect(ui->actionOpen, &QAction::triggered, this, [this]()
    {
       auto selection = pfd::open_file("Select ROM", "./ROM/", {"CHIP-8 ROMs", "*.ch8"}).result();
       if (!selection.empty()) {
           emulationTimer->stop();
           if (m_audioSink->state() == QAudio::ActiveState) {
               m_audioSink->suspend();
           }

           std::string filename = selection[0];
           emulator = std::make_unique<SuperChip8::SuperChip8>(filename);
           emulator->change_cycles_per_tick(currentCycles);

           ui->MasterChip8Display->setDisplay(&emulator->display);

           // Start in PAUSED state
           // emulationTimer->start(16);

           // Enable controls since we are paused
           ui->btnDecreaseCycles->setEnabled(true);
           ui->btnIncreaseCycles->setEnabled(true);

           updateDebugInfo();
       }
    });

    // Settings Dialog
    connect(ui->actionPreferences, &QAction::triggered, this, [this]() {
        SettingsDialog dialog(this,
            ui->MasterChip8Display->getPixelColor(),
            ui->MasterChip8Display->getBackgroundColor(),
            ui->MasterChip8Display->isGridEnabled()
        );
        if (dialog.exec() == QDialog::Accepted) {
            ui->MasterChip8Display->setColors(dialog.getPixelColor(), dialog.getBackgroundColor());
            ui->MasterChip8Display->setGridEnabled(dialog.isGridEnabled());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateAudio()
{
    if (!emulator) return;

    if (emulator->registers.get_sound_timer() > 0) {
        if (m_audioSink->state() == QAudio::StoppedState) {
            m_audioSink->start(m_beepGen);
        } else if (m_audioSink->state() == QAudio::SuspendedState) {
            m_audioSink->resume();
        }
    } else {
        if (m_audioSink->state() == QAudio::ActiveState) {
            m_audioSink->suspend();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (emulator) {
        int key = mapKey(event->key());
        if (key != -1) {
            emulator->keyboard.press_key(key);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (emulator) {
        int key = mapKey(event->key());
        if (key != -1) {
            emulator->keyboard.release_key(key);
        }
    }
}

int MainWindow::mapKey(int qtKey)
{
    switch (qtKey) {
        case Qt::Key_1: return 0x1;
        case Qt::Key_2: return 0x2;
        case Qt::Key_3: return 0x3;
        case Qt::Key_4: return 0xC;

        case Qt::Key_Q: return 0x4;
        case Qt::Key_W: return 0x5;
        case Qt::Key_E: return 0x6;
        case Qt::Key_R: return 0xD;

        case Qt::Key_A: return 0x7;
        case Qt::Key_S: return 0x8;
        case Qt::Key_D: return 0x9;
        case Qt::Key_F: return 0xE;

        case Qt::Key_Z: return 0xA;
        case Qt::Key_X: return 0x0;
        case Qt::Key_C: return 0xB;
        case Qt::Key_V: return 0xF;

        default: return -1;
    }
}

void MainWindow::setupDebugTable()
{
    ui->tblDebug->setRowCount(19); // V0-VF (16) + PC + I + Opcode

    QStringList headers;
    headers << "Reg" << "Value";
    ui->tblDebug->setHorizontalHeaderLabels(headers);

    // Initialize labels
    for(int i=0; i<16; ++i) {
        ui->tblDebug->setItem(i, 0, new QTableWidgetItem(QString("V%1").arg(i, 1, 16).toUpper()));
        ui->tblDebug->setItem(i, 1, new QTableWidgetItem("00"));
    }
    ui->tblDebug->setItem(16, 0, new QTableWidgetItem("PC"));
    ui->tblDebug->setItem(16, 1, new QTableWidgetItem("0000"));

    ui->tblDebug->setItem(17, 0, new QTableWidgetItem("I"));
    ui->tblDebug->setItem(17, 1, new QTableWidgetItem("0000"));

    ui->tblDebug->setItem(18, 0, new QTableWidgetItem("Opcode"));
    ui->tblDebug->setItem(18, 1, new QTableWidgetItem("0000"));
}

void MainWindow::updateDebugInfo()
{
    if (!emulator) return;

    // Update V Registers
    for(int i=0; i<16; ++i) {
        uint8_t val = emulator->registers.get_V(i);
        ui->tblDebug->item(i, 1)->setText(QString("%1").arg(val, 2, 16, QChar('0')).toUpper());
    }

    // Update PC
    uint16_t pc = emulator->registers.get_PC();
    ui->tblDebug->item(16, 1)->setText(QString("%1").arg(pc, 4, 16, QChar('0')).toUpper());

    // Update I
    uint16_t I = emulator->registers.get_I();
    ui->tblDebug->item(17, 1)->setText(QString("%1").arg(I, 4, 16, QChar('0')).toUpper());

    // Update Opcode (Read from memory at PC)
    // Note: This shows the NEXT opcode to be executed
    uint16_t opcode = emulator->memory.read_opcode(pc);
    ui->tblDebug->item(18, 1)->setText(QString("%1").arg(opcode, 4, 16, QChar('0')).toUpper());
}
