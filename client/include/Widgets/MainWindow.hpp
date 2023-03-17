#pragma once

#include "Widgets/Call.hpp"
#include "Widgets/Contacts.hpp"
#include "Widgets/Login.hpp"
#include "Network/Thread.hpp"
#include <QMainWindow>
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
            void login_done();
            void register_click();
            void register_done();
            void fetch_contacts();
            void check_calls();
            void call_click(const std::string &caller, int id);
            void hangup_click();
            void net_response();

        private:
            static constexpr std::tuple<int, int> WIN_SIZE = {640, 564};

            void init_callbacks();

            QPointer<babel::NetThread> network_thread = nullptr;
            QPointer<QTcpSocket> sock;
            QByteArray byte_arr;
            QTimer time;
            QPointer<QStackedWidget> list;
            babel::LoginPage log_page;
            babel::ContactsPage contact_page;
            babel::CallPage call_page;
    };
}