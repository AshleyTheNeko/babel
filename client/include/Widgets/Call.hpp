#pragma once

#include <QElapsedTimer>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace babel
{
    class CallPage {
        public:
            CallPage();
            CallPage(CallPage &&) = delete;
            CallPage(const CallPage &) = delete;
            CallPage &operator=(CallPage &&) = delete;
            CallPage &operator=(const CallPage &) = delete;
            ~CallPage() = default;

            QWidget &get_central_widget();
            void setup_informations(std::string name, int start_time);
            QPushButton &get_hangup();
            QPushButton &get_mute();

            void change_text(void);

        private:
            QElapsedTimer timer;
            QWidget *centralwidget;
            QVBoxLayout *verticalLayout;
            QFrame *frame;
            QVBoxLayout *verticalLayout_2;
            QLabel *label;
            QLabel *label_2;
            QFrame *frame_2;
            QHBoxLayout *horizontalLayout;
            QPushButton *pushButton_2;
            QPushButton *pushButton;

            void setup_ui(void);
    };
}