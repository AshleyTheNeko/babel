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
    contact_fetch_timer.setInterval(SECOND * 15);
    contact_fetch_timer.start();
    call_fetch_timer.setInterval(SECOND * 3);
    call_fetch_timer.start();
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
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header = std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/1";
    std::string request = header + fields;

    network_thread = new NetThread(request.c_str(), reg_log_bytes, *sock, this);
    connect(network_thread.data(), &QThread::finished, this, &babel::MainWindow::register_done);
    network_thread->start();
}

void babel::MainWindow::login_done()
{
    if (reg_log_bytes.toStdString() == "OK") {
        list->setCurrentWidget(&contact_page.get_central_widget());
    } else {
        std::cout << "FUUUUUUUCK" << std::endl;
    }
}

void babel::MainWindow::register_click()
{
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header = std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/6";
    std::string request = header + fields;

    network_thread = new NetThread(request.c_str(), reg_log_bytes, *sock, this);
    connect(network_thread.data(), &QThread::finished, this, &babel::MainWindow::register_done);
    network_thread->start();
}

void babel::MainWindow::register_done() {
    if (reg_log_bytes.toStdString() == "OK") {
        std::cout << "GOOOOOOOOD" << std::endl;
    } else {
        std::cout << "FUUUUUUUCK" << std::endl;
    }
}

void babel::MainWindow::check_calls() {}
void babel::MainWindow::calls_checked() {}

void babel::MainWindow::fetch_contacts()
{
    network_thread = new NetThread("0001/50", contacts_bytes, *sock, this);
    connect(network_thread.data(), &QThread::finished, this, &babel::MainWindow::contacts_fetched);
    network_thread->start();
}
void babel::MainWindow::contacts_fetched()
{
    std::string response = contacts_bytes.toStdString();
    std::string token;
    size_t pos = 0;

    std::cout << response << std::endl;
    contact_page.clear_contacts();
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
    connect(&log_page.get_register_button(), &QPushButton::clicked, this, &babel::MainWindow::register_click);
    connect(&time, &QTimer::timeout, this, [this] { call_page.change_text(); });
    connect(&contact_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::fetch_contacts);
    connect(&call_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::check_calls);
}