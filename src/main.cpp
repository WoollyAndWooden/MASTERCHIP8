//
// Created by Gaming on 10/11/2025.
//
#include <QApplication>
#include <mainwindow.h>
#include <utils.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}

