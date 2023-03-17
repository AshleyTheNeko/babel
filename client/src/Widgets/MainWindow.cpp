#include "Widgets/MainWindow.hpp"
#include "const_expressions.h"
#include <QMessageBox>
#include <QThread>
#include <array>
#include <iostream>

babel::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
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
    sock = new QTcpSocket(this);
    sock->connectToHost("localhost", 2000);
    sock->waitForConnected(3000);
    init_callbacks();
}

void babel::MainWindow::hangup_click() { list->setCurrentWidget(&contact_page.get_central_widget()); }

void babel::MainWindow::call_click(const std::string &caller, int id)
{
    call_page.setup_informations(caller, id);
    list->setCurrentWidget(&call_page.get_central_widget());
}

void babel::MainWindow::login_click()
{
    network_thread = new NetThread("0001/50", byte_arr, *sock, this);
    connect(network_thread.data(), &QThread::finished, this, &babel::MainWindow::net_response);
    network_thread->start();
    list->setCurrentWidget(&contact_page.get_central_widget());
}

void babel::MainWindow::login_done() {

}

void babel::MainWindow::register_click() {
    network_thread = new NetThread("001/00050", byte_arr, *sock, this);
    connect(network_thread.data(), &QThread::finished, this, &babel::MainWindow::net_response);
    network_thread->start();
    list->setCurrentWidget(&contact_page.get_central_widget());
}

void babel::MainWindow::register_done() {}

void babel::MainWindow::check_calls() {}

void babel::MainWindow::fetch_contacts() {}

void babel::MainWindow::net_response()
{
    std::string response = byte_arr.toStdString();
    std::string token;
    size_t pos = 0;

    std::cout << response << std::endl;
    while ((pos = response.find(';')) != std::string::npos) {
        token = response.substr(0, pos);
        if (!token.empty()) {
            connect(&contact_page.add_contact(token, 30), &QPushButton::clicked, this,
                [this, token] { babel::MainWindow::call_click(token, 30); });
        }
        std::cout << token << std::endl;
        response.erase(0, pos + 1);
    }
}

void babel::MainWindow::init_callbacks()
{
    connect(&call_page.get_hangup(), &QPushButton::clicked, this, &babel::MainWindow::hangup_click);
    connect(&log_page.get_login_button(), &QPushButton::clicked, this, &babel::MainWindow::login_click);
    connect(&time, &QTimer::timeout, this, [this] { call_page.change_text(); });
}