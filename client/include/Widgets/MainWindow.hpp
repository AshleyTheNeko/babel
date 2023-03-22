#pragma once
#include "Audio/AudioBuffer.hpp"
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
            MainWindow(QString &host, int port);
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
            void send_audio();

        private:
            static constexpr std::tuple<int, int> WIN_SIZE = {640, 564};

            void init_callbacks();
            void login_res(std::string &&body);
            void register_res(std::string &&body);
            void in_call_res(std::string &&body);
            void call_res(std::string &&body, bool type);
            void get_call_res(std::string &&body);
            void hangup_res(std::string &&body);
            void contacts_res(std::string &&body);
            void call_packet_res(std::string &&body);

            bool call = false;
            std::unique_ptr<AudioBuffer> audio_manager = nullptr;
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