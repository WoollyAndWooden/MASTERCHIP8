#pragma once

#include <QDialog>
#include <QColor>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr, QColor currentPixel = Qt::yellow, QColor currentBg = Qt::black, bool gridEnabled = false);
    ~SettingsDialog();

    QColor getPixelColor() const;
    QColor getBackgroundColor() const;
    bool isGridEnabled() const;

private:
    Ui::SettingsDialog *ui;
    QColor m_pixelColor;
    QColor m_bgColor;

    void updateButtonStyles();
};
