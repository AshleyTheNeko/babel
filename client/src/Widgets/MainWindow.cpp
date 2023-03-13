#include "Widgets/MainWindow.hpp"
#include <QMessageBox>
#include <iostream>

babel::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), log_page(), contact_page(), call_page()
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("MainWindow"));
    setFixedSize(640, 564);
    init_callbacks();
    setWindowTitle(QCoreApplication::translate("MainWindow", "Babel", nullptr));
    list = new QStackedWidget(this);
    setCentralWidget(list);
    list->addWidget(&log_page.get_central_widget());
    list->addWidget(&contact_page.get_central_widget());
    list->addWidget(&call_page.get_central_widget());
    list->setCurrentIndex(0);
    time.setInterval(1000);
    time.start();
}

void babel::MainWindow::hangup_click(void) { list->setCurrentWidget(&contact_page.get_central_widget()); }

void babel::MainWindow::call_click(std::string caller, int id)
{
    call_page.setup_informations(caller, id);
    list->setCurrentWidget(&call_page.get_central_widget());
}

void babel::MainWindow::login_click()
{
    std::string fortnite[] = {
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
    for (int i = 0; i < 12; i++) {
        connect(&contact_page.add_contact(fortnite[i], i), &QPushButton::clicked, this,
            [this, i, fortnite] { babel::MainWindow::call_click(fortnite[i], i); });
    }
    list->setCurrentWidget(&contact_page.get_central_widget());
}

void babel::MainWindow::init_callbacks()
{
    connect(&call_page.get_hangup(), &QPushButton::clicked, this, &babel::MainWindow::hangup_click);
    connect(&log_page.get_login_button(), &QPushButton::clicked, this, &babel::MainWindow::login_click);
    connect(&time, &QTimer::timeout, this, [this] { this->call_page.change_text(); });
}