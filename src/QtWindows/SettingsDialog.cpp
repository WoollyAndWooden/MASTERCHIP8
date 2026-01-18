#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <QColorDialog>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent, QColor currentPixel, QColor currentBg, bool gridEnabled) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_pixelColor(currentPixel),
    m_bgColor(currentBg)
{
    ui->setupUi(this);

    ui->chkPixelGrid->setChecked(gridEnabled);
    updateButtonStyles();

    connect(ui->btnPixelColor, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(m_pixelColor, this, "Select Pixel Color");
        if (color.isValid()) {
            m_pixelColor = color;
            updateButtonStyles();
        }
    });

    connect(ui->btnBgColor, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(m_bgColor, this, "Select Background Color");
        if (color.isValid()) {
            m_bgColor = color;
            updateButtonStyles();
        }
    });
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QColor SettingsDialog::getPixelColor() const
{
    return m_pixelColor;
}

QColor SettingsDialog::getBackgroundColor() const
{
    return m_bgColor;
}

bool SettingsDialog::isGridEnabled() const
{
    return ui->chkPixelGrid->isChecked();
}

void SettingsDialog::updateButtonStyles()
{
    ui->btnPixelColor->setStyleSheet(QString("background-color: %1").arg(m_pixelColor.name()));
    ui->btnBgColor->setStyleSheet(QString("background-color: %1").arg(m_bgColor.name()));
}
