#pragma once

#include "Widgets/Call.hpp"
#include "Widgets/Contacts.hpp"
#include "Widgets/Login.hpp"
#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>

namespace babel
{
    class MainWindow : public QMainWindow {
        public:
            MainWindow(QWidget *parent = nullptr);
            MainWindow(MainWindow &&) = delete;
            MainWindow(const MainWindow &) = delete;
            MainWindow &operator=(MainWindow &&) = delete;
            MainWindow &operator=(const MainWindow &) = delete;
            ~MainWindow() = default;

        signals:
        public slots:
            void loginButton();
            void callButton(std::string caller, int id);
            void hangupButton();
            void updateClock();

        private:
            void initCallbacks();

        private:
            QTimer time;
            QPointer<QStackedWidget> list;
            babel::LoginPage log_page;
            babel::ContactsPage contact_page;
            babel::CallPage call_page;
    };
}