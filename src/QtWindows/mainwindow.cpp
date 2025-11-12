#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <portable-file-dialogs.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, &QAction::triggered, this, []()
    {
       auto filename =  pfd::open_file("Select ROM", "./ROM/", {"*.ch8"}).result();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
