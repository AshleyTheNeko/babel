#include "Widgets/MainWindow.hpp"
#include "Packet.hpp"
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
    contact_fetch_timer.setInterval(SECOND * 6);
    contact_fetch_timer.start();
    call_fetch_timer.setInterval(SECOND * 3);
    call_fetch_timer.start();
    sock = new QTcpSocket(this);
    sock->connectToHost("localhost", 2000);
    sock->waitForConnected(3000);
    connect(sock, &QTcpSocket::readyRead, this, &babel::MainWindow::net_response);
    init_callbacks();
}

void babel::MainWindow::hangup_click()
{
    std::string request = "0001/" + std::to_string(babel::RequestType::HANGUP) + "0";
    sock->write(request.c_str());
}

void babel::MainWindow::call_click(const std::string &caller)
{
    std::string header = std::to_string(caller.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::CALL);

    std::string request = header + caller;
    sock->write(request.c_str());
}

void babel::MainWindow::login_click()
{
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::LOGIN);
    std::string request = header + fields;

    sock->write(request.c_str());
}

void babel::MainWindow::register_click()
{
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::REGISTER);
    std::string request = header + fields;

    sock->write(request.c_str());
}

void babel::MainWindow::check_calls()
{
    if (call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::GET_CALLS) + "0";
    sock->write(request.c_str());
}

void babel::MainWindow::in_call()
{
    if (!call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::IN_CALL) + "0";
    sock->write(request.c_str());
}

void babel::MainWindow::fetch_contacts()
{
    if (call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::GET_USERS) + "0";
    sock->write(request.c_str());
}

void babel::MainWindow::net_response()
{
    std::string bytes = sock->readAll().toStdString();
    std::cout << bytes << std::endl;
    std::string body = bytes.substr(2);
    std::string token;
    size_t pos = 0;

    switch (std::strtol(bytes.substr(0, 1).c_str(), nullptr, 10)) {
        case babel::RequestType::LOGIN:
            if (body == "OK") {
                fetch_contacts();
                list->setCurrentWidget(&contact_page.get_central_widget());
            } else {
                log_page.set_error("Login failed");
            }
            break;
        case babel::RequestType::GET_USERS:
            contact_page.clear_contacts();
            while ((pos = body.find(';')) != std::string::npos) {
                token = body.substr(0, pos);
                if (!token.empty()) {
                    connect(&contact_page.add_contact(token), &QPushButton::clicked, this,
                        [this, token] { babel::MainWindow::call_click(token); });
                }
                body.erase(0, pos + 1);
            }
            break;
        case babel::RequestType::REGISTER:
            if (body == "OK") {
                log_page.set_error("Registered new user");
            } else {
                log_page.set_error("Register failed: " + body);
            }
            break;
        case babel::RequestType::GET_CALLS:
            if (body[0] == '1') {
                call = true;
                msgbox = new QMessageBox(this);
                msgbox->setText((body.substr(1) + " is calling you.").c_str());
                msgbox->setInformativeText("Do you accept the call?");
                msgbox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgbox->setDefaultButton(QMessageBox::No);
                int ret = msgbox->exec();
                msgbox.clear();
                if (ret == QMessageBox::Yes) {
                    std::string request = "0001/" + std::to_string(babel::RequestType::ACCEPT_CALL) + "0";
                    sock->write(request.c_str());
                } else {
                    call = false;
                    hangup_click();
                }
            }
            break;
        case babel::RequestType::HANGUP:
            if (body == "OK") {
                call = false;
                list->setCurrentWidget(&contact_page.get_central_widget());
            }
            break;
        case babel::RequestType::CALL:
        case babel::RequestType::ACCEPT_CALL:
            if (body[0] == '0') {
                call = true;
                std::cout << "call" << call << std::endl;
                call_page.setup_informations(body.substr(1), (bytes.substr(0, 1) == std::to_string(CALL)));
                list->setCurrentWidget(&call_page.get_central_widget());
            }
            break;
        case babel::RequestType::IN_CALL:
            switch (body[0]) {
                case '0':
                    if (call) {
                        call = false;
                        list->setCurrentWidget(&contact_page.get_central_widget());
                    }
                    break;
                case '1':
                    break;
                case '2':
                    call_page.set_await(false);
                    break;
                default:
                    break;
            }
        default:
            break;
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
    connect(&call_fetch_timer, &QTimer::timeout, this, &babel::MainWindow::in_call);
}