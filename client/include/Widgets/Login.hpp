#pragma once

#include <QPointer>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace babel
{
    class LoginPage {
        public:
            LoginPage();
            LoginPage(LoginPage &&) = delete;
            LoginPage(const LoginPage &) = delete;
            LoginPage &operator=(LoginPage &&) = delete;
            LoginPage &operator=(const LoginPage &) = delete;
            ~LoginPage() = default;

            QPushButton &get_login_button();
            QPushButton &get_register_button();
            QWidget &get_central_widget();
            std::string get_username();
            std::string get_password();
            void set_error(std::string &&err);

        private:
            QPointer<QWidget> central_widget;
            QPointer<QVBoxLayout> login_box;
            QPointer<QLabel> error_label;
            QPointer<QLabel> username_label;
            QPointer<QLineEdit> username_field;
            QPointer<QLabel> password_label;
            QPointer<QLineEdit> password_field;
            QPointer<QPushButton> login_button;
            QPointer<QPushButton> register_button;
            QSpacerItem *spacer_1;
            QSpacerItem *spacer_2;
            QSpacerItem *spacer_3;

            void setup_ui();
    };
}