#pragma once

#include <QElapsedTimer>
#include <QPointer>
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
            QPointer<QWidget> central_widget;
            QPointer<QVBoxLayout> main_layout;
            QPointer<QFrame> call_info_frame;
            QPointer<QVBoxLayout> call_info_layout;
            QPointer<QLabel> call_name;
            QPointer<QLabel> call_time;
            QPointer<QFrame> call_buttons_frame;
            QPointer<QHBoxLayout> call_buttons_layout;
            QPointer<QPushButton> mic_button;
            QPointer<QPushButton> hangup_button;

            void setup_ui(void);
    };
}