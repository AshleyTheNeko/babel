#pragma once

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
            QWidget &get_central_widget();

        private:
            QWidget *centralwidget = nullptr;
            QVBoxLayout *verticalLayout = nullptr;
            QSpacerItem *horizontalSpacer = nullptr;
            QLabel *label_2 = nullptr;
            QLineEdit *lineEdit = nullptr;
            QLabel *label = nullptr;
            QLineEdit *lineEdit_2 = nullptr;
            QSpacerItem *horizontalSpacer_3 = nullptr;
            QPushButton *pushButton = nullptr;
            QPushButton *pushButton_2 = nullptr;
            QSpacerItem *horizontalSpacer_4 = nullptr;

            void setup_ui();
    };
}