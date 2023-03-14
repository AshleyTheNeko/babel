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

        public slots:
            void login_click();
            void call_click(const std::string &caller, int id);
            void hangup_click();

        private:
            static constexpr std::tuple<int, int> WIN_SIZE = {640, 564};

            void init_callbacks();

            QTimer time;
            QPointer<QStackedWidget> list;
            babel::LoginPage log_page;
            babel::ContactsPage contact_page;
            babel::CallPage call_page;
    };
}