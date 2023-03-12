#include "Widgets/Login.hpp"
#include "Widgets/MainWindow.hpp"
#include <QApplication>
#include <QMainWindow>
#include <iostream>

int main(int argc, char const *argv[])
{
    QApplication app(argc, (char **) argv);
    babel::MainWindow window;

    window.show();

    return app.exec();
}