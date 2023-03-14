#include "Widgets/MainWindow.hpp"
#include "const_expressions.h"
#include <QMessageBox>
#include <array>
#include <iostream>

babel::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    if (objectName().isEmpty()) {
        setObjectName(QString::fromUtf8("MainWindow"));
    }
    setFixedSize(std::get<0>(WIN_SIZE), std::get<1>(WIN_SIZE));
    init_callbacks();
    setWindowTitle(QCoreApplication::translate("MainWindow", "Babel", nullptr));
    list = new QStackedWidget(this);
    setCentralWidget(list);
    list->addWidget(&log_page.get_central_widget());
    list->addWidget(&contact_page.get_central_widget());
    list->addWidget(&call_page.get_central_widget());
    list->setCurrentIndex(0);
    time.setInterval(SECOND);
    time.start();
}

void babel::MainWindow::hangup_click() { list->setCurrentWidget(&contact_page.get_central_widget()); }

void babel::MainWindow::call_click(const std::string &caller, int id)
{
    call_page.setup_informations(caller, id);
    list->setCurrentWidget(&call_page.get_central_widget());
}

void babel::MainWindow::login_click()
{
    std::array<std::string, 13> fortnite = {
        "amogus",
        "assert_perror",
        "drand48_datax",
        "csc",
        "cxc",
        "d_ptr",
        "grdfgrs",
        "be32toh",
        "sdfd",
        "fdsf",
        "vvv",
        "XATTR_LIST_MAX",
        "ccc",
    };
    for (int i = 0; i < fortnite.size(); i++) {
        connect(&contact_page.add_contact(fortnite.at(i), i), &QPushButton::clicked, this,
            [this, i, fortnite] { babel::MainWindow::call_click(fortnite.at(i), i); });
    }
    list->setCurrentWidget(&contact_page.get_central_widget());
}

void babel::MainWindow::init_callbacks()
{
    connect(&call_page.get_hangup(), &QPushButton::clicked, this, &babel::MainWindow::hangup_click);
    connect(&log_page.get_login_button(), &QPushButton::clicked, this, &babel::MainWindow::login_click);
    connect(&time, &QTimer::timeout, this, [this] { this->call_page.change_text(); });
}