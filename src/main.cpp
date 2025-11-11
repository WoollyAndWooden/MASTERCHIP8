//
// Created by Gaming on 10/11/2025.
//
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label("Hello, Qt World!");
    label.resize(200, 100);
    label.show();

    return app.exec();
}
