#pragma once

#include "Widgets/Call.hpp"
#include "Widgets/Contacts.hpp"
#include "Widgets/Login.hpp"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTcpSocket>
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

        public slots:
            void login_click();
            void register_click();
            void fetch_contacts();
            void check_calls();
            void in_call();
            void call_click(const std::string &caller);
            void hangup_click();
            void net_response();

        private:
            static constexpr std::tuple<int, int> WIN_SIZE = {640, 564};

            void init_callbacks();

            bool call = false;
            QPointer<QMessageBox> msgbox = nullptr;
            QPointer<QTcpSocket> sock;
            QTimer time;
            QTimer contact_fetch_timer;
            QTimer call_fetch_timer;
            QPointer<QStackedWidget> list;
            babel::LoginPage log_page;
            babel::ContactsPage contact_page;
            babel::CallPage call_page;
    };
}