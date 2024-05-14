#pragma once

#include <QPointer>
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
#include <iostream>

namespace babel
{
    class ContactButton {
        public:
            ContactButton(std::string const &name, QWidget *parent, QVBoxLayout *main_layout);
            ContactButton(ContactButton &&) = delete;
            ContactButton(const ContactButton &) = delete;
            ContactButton &operator=(ContactButton &&) = delete;
            ContactButton &operator=(const ContactButton &) = delete;
            ~ContactButton();

            std::string &get_name(void);
            QPushButton &get_button(void);

        private:
            std::string name;
            QPointer<QFrame> frame;
            QPointer<QHBoxLayout> layout;
            QPointer<QLabel> label;
            QPointer<QPushButton> button;
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
            QPushButton &add_contact(std::string const &name);
            void clear_contacts(void);

        private:
            QPointer<QWidget> central_widget;
            QPointer<QHBoxLayout> main_layout;
            QPointer<QScrollArea> contact_scroll_area;
            QPointer<QWidget> contacts_holder;
            QPointer<QVBoxLayout> contacts_layout;
            std::vector<std::unique_ptr<ContactButton>> contacts;

            void setup_ui();
    };
}