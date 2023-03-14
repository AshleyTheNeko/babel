#include "Widgets/Login.hpp"

babel::LoginPage::LoginPage()
    : central_widget(new QWidget()), login_box(new QVBoxLayout(central_widget)),
      spacer_1(new QSpacerItem(625, 10, QSizePolicy::Expanding, QSizePolicy::Minimum)), username_label(new QLabel(central_widget)),
      username_field(new QLineEdit(central_widget)), password_label(new QLabel(central_widget)),
      password_field(new QLineEdit(central_widget)), spacer_2(new QSpacerItem(437, 28, QSizePolicy::Expanding, QSizePolicy::Minimum)),
      login_button(new QPushButton(central_widget)), register_button(new QPushButton(central_widget)),
      spacer_3(new QSpacerItem(437, 10, QSizePolicy::Fixed, QSizePolicy::Minimum))
{
    setup_ui();
}

void babel::LoginPage::setup_ui()
{
    central_widget->setEnabled(true);
    central_widget->setObjectName(QString::fromUtf8("central_widget"));
    login_box->setObjectName(QString::fromUtf8("login_box"));
    login_box->setContentsMargins(100, 150, 100, 150);

    login_box->addItem(spacer_1);

    username_label->setObjectName(QString::fromUtf8("username_label"));
    username_label->setAlignment(Qt::AlignCenter);

    login_box->addWidget(username_label);

    username_field->setObjectName(QString::fromUtf8("username_field"));

    login_box->addWidget(username_field);

    password_label->setObjectName(QString::fromUtf8("password_label"));
    password_label->setAlignment(Qt::AlignCenter);

    login_box->addWidget(password_label);

    password_field->setObjectName(QString::fromUtf8("password_field"));
    password_field->setEchoMode(QLineEdit::Password);

    login_box->addWidget(password_field);

    login_box->addItem(spacer_2);

    login_button->setObjectName(QString::fromUtf8("login_button"));

    login_box->addWidget(login_button);

    register_button->setObjectName(QString::fromUtf8("register_button"));

    login_box->addWidget(register_button);

    login_box->addItem(spacer_3);

    username_label->setText(QString("Username"));
    password_label->setText(QString("Password"));
    login_button->setText(QString("Login"));
    register_button->setText(QString("Register"));
}

QPushButton &babel::LoginPage::get_login_button() { return (*login_button); }

QWidget &babel::LoginPage::get_central_widget() { return (*central_widget); }