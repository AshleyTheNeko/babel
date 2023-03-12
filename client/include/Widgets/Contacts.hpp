#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QPointer>

namespace babel
{
    class ContactButton {
        public:
            ContactButton(int id, std::string name, QWidget *parent, QVBoxLayout *main_layout);
            ContactButton(ContactButton &&) = delete;
            ContactButton(const ContactButton &) = delete;
            ContactButton &operator=(ContactButton &&) = delete;
            ContactButton &operator=(const ContactButton &) = delete;
            ~ContactButton() = default;

            std::string &get_name(void);
            int &get_id(void);
            QPushButton &get_button(void);

        private:
            std::string name;
            int id;
            QFrame *frame;
            QHBoxLayout *layout;
            QLabel *label;
            QPushButton *button;
    };

    class ContactsPage {
        public:
            ContactsPage();
            ContactsPage(ContactsPage &&) = delete;
            ContactsPage(const ContactsPage &) = delete;
            ContactsPage &operator=(ContactsPage &&) = delete;
            ContactsPage &operator=(const ContactsPage &) = delete;
            ~ContactsPage() = default;

            QWidget &get_central_widget();
            QPushButton &add_contact(std::string name, int id);
            void clear_contacts(void);

        private:
            QPointer<QWidget> centralwidget;
            QHBoxLayout *horizontalLayout_10;
            QScrollArea *scrollArea;
            QWidget *scrollAreaWidgetContents;
            QVBoxLayout *verticalLayout;
            std::vector<std::shared_ptr<babel::ContactButton>> contacts;

            void setup_ui();
    };
}