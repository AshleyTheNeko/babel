#include "Error.hpp"
#include "Widgets/Login.hpp"
#include "Widgets/MainWindow.hpp"
#include <QApplication>
#include <QMainWindow>
#include <iostream>

int main(int argc, char **argv)
{
    try {
        QApplication app(argc, argv);

        babel::MainWindow window(nullptr);
        window.show();

        return app.exec();
    } catch (babel::Error &e) {
        std::cerr << "Error during start: " << e.what() << std::endl;
    }
    return (1);
}