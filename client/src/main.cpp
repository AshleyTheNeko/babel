#include "Error.hpp"
#include "Widgets/Login.hpp"
#include "Widgets/MainWindow.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QMainWindow>
#include <iostream>

int main(int argc, char **argv)
{
    try {
        QApplication app(argc, argv);
        QCommandLineParser parser;

        QCommandLineOption host_opt({"i", "host"}, "The server address.", "host");
        QCommandLineOption port_opt({"p", "port"}, "The server port.", "port");

        parser.addHelpOption();
        parser.addVersionOption();
        parser.addOption(host_opt);
        parser.addOption(port_opt);
        parser.process(QCoreApplication::arguments());
        QStringList args = parser.positionalArguments();

        int port = parser.value("port").toInt();
        QString host = parser.value("host");

        if (!parser.isSet(host_opt) || !parser.isSet(port_opt)) {
            std::cerr << "Ip and port are required." << std::endl;
            parser.showHelp();
        }

        babel::MainWindow window(host, port);
        window.show();

        return app.exec();
    } catch (babel::Error &e) {
        std::cerr << "Error during start: " << e.what() << std::endl;
    }
    return (1);
}