#include "Widgets/Login.hpp"

babel::LoginPage::LoginPage() { setup_ui(); }

void babel::LoginPage::setup_ui()
{
    centralwidget = new QWidget();
    centralwidget->setEnabled(true);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(100, 150, 100, 150);
    horizontalSpacer = new QSpacerItem(625, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    verticalLayout->addItem(horizontalSpacer);

    label_2 = new QLabel(centralwidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label_2);

    lineEdit = new QLineEdit(centralwidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    verticalLayout->addWidget(lineEdit);

    label = new QLabel(centralwidget);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label);

    lineEdit_2 = new QLineEdit(centralwidget);
    lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
    lineEdit_2->setEchoMode(QLineEdit::Password);

    verticalLayout->addWidget(lineEdit_2);

    horizontalSpacer_3 = new QSpacerItem(437, 28, QSizePolicy::Expanding, QSizePolicy::Minimum);

    verticalLayout->addItem(horizontalSpacer_3);

    pushButton = new QPushButton(centralwidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    verticalLayout->addWidget(pushButton);

    pushButton_2 = new QPushButton(centralwidget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    verticalLayout->addWidget(pushButton_2);

    horizontalSpacer_4 = new QSpacerItem(437, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

    verticalLayout->addItem(horizontalSpacer_4);

    label_2->setText(QCoreApplication::translate("MainWindow", "Username", nullptr));
    label->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
    pushButton->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
    pushButton_2->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
}

QPushButton &babel::LoginPage::get_login_button(void)
{
    return (*pushButton);
}

QWidget &babel::LoginPage::get_central_widget(void)
{
    return (*centralwidget);
}