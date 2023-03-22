#include "Widgets/MainWindow.hpp"
#include "Packet.hpp"
#include "const_expressions.h"
#include <QAbstractSocket>
#include <QMessageBox>
#include <QThread>
#include <array>
#include <iostream>

babel::MainWindow::MainWindow(QString &host, int port) : QMainWindow(nullptr)
{
    if (objectName().isEmpty()) {
        setObjectName(QString::fromUtf8("MainWindow"));
    }

    setFixedSize(std::get<0>(WIN_SIZE), std::get<1>(WIN_SIZE));
    setWindowTitle(QCoreApplication::translate("MainWindow", "Babel", nullptr));

    list = new QStackedWidget(this);
    setCentralWidget(list);
    list->addWidget(&log_page.get_central_widget());
    list->addWidget(&contact_page.get_central_widget());
    list->addWidget(&call_page.get_central_widget());
    list->setCurrentIndex(0);

    time.setInterval(SECOND);
    time.start();

    contact_fetch_timer.setInterval(SECOND * 6);
    contact_fetch_timer.start();

    call_fetch_timer.setInterval(SECOND * 3);
    call_fetch_timer.start();

    sock = new QTcpSocket(this);
    sock->connectToHost(host, port);
    sock->waitForConnected(3000);
    if (sock->state() != QAbstractSocket::ConnectedState) {
        throw Error("Unable to connect to server.");
    }
    connect(sock, &QTcpSocket::readyRead, this, &babel::MainWindow::net_response);

    audio_manager = std::make_unique<AudioBuffer>();

    init_callbacks();
}

void babel::MainWindow::init_callbacks()
{
    connect(&call_page.get_hangup(), &QPushButton::clicked, this, &babel::MainWindow::hangup_click);
    connect(&log_page.get_login_button(), &QPushButton::clicked, this, &babel::MainWindow::login_click);
    connect(&log_page.get_register_button(), &QPushButton::clicked, this, &babel::MainWindow::register_click);
    connect(&time, &QTimer::timeout, this, [this] { call_page.change_text(); });
    connect(&contact_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::fetch_contacts);
    connect(&call_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::check_calls);
    connect(&call_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::in_call);
}