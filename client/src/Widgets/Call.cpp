#include "Widgets/Call.hpp"
#include "const_expressions.h"
#include <iomanip>
#include <sstream>

babel::CallPage::CallPage()
    : central_widget(new QWidget()), main_layout(new QVBoxLayout(central_widget)), call_info_frame(new QFrame(central_widget)),
      call_info_layout(new QVBoxLayout(call_info_frame)), call_name(new QLabel(call_info_frame)), call_time(new QLabel(call_info_frame)),
      call_buttons_frame(new QFrame(central_widget)), call_buttons_layout(new QHBoxLayout(call_buttons_frame)),
      mic_button(new QPushButton(call_buttons_frame)), hangup_button(new QPushButton(call_buttons_frame))

{
    setup_ui();
}

constexpr int NAME_SIZE = 25;
constexpr int TIME_SIZE = 20;
constexpr int TEXT_SIZE = 10;

void babel::CallPage::setup_ui()
{
    central_widget->setObjectName(QString::fromUtf8("central_widget"));

    main_layout->setObjectName(QString::fromUtf8("main_layout"));

    call_info_frame->setObjectName(QString::fromUtf8("call_info_frame"));
    call_info_frame->setFrameShape(QFrame::StyledPanel);
    call_info_frame->setFrameShadow(QFrame::Raised);
    call_info_layout->setObjectName(QString::fromUtf8("call_info_layout"));

    call_name->setObjectName(QString::fromUtf8("call_name"));
    QFont font_name;
    font_name.setPointSize(NAME_SIZE);
    call_name->setFont(font_name);
    call_name->setAlignment(Qt::AlignCenter);

    call_info_layout->addWidget(call_name);

    call_time->setObjectName(QString::fromUtf8("call_time"));
    QFont font_time;
    font_time.setPointSize(TIME_SIZE);
    call_time->setFont(font_time);
    call_time->setScaledContents(false);
    call_time->setAlignment(Qt::AlignCenter);
    call_time->setIndent(0);

    call_info_layout->addWidget(call_time);

    main_layout->addWidget(call_info_frame);

    call_buttons_frame->setObjectName(QString::fromUtf8("call_buttons_frame"));
    call_buttons_frame->setFrameShape(QFrame::StyledPanel);
    call_buttons_frame->setFrameShadow(QFrame::Raised);
    call_buttons_layout->setObjectName(QString::fromUtf8("call_buttons_layout"));
    mic_button->setObjectName(QString::fromUtf8("mic_button"));
    QFont font_button;
    font_button.setPointSize(TEXT_SIZE);
    mic_button->setFont(font_button);

    call_buttons_layout->addWidget(mic_button);

    hangup_button->setObjectName(QString::fromUtf8("hangup_button"));

    call_buttons_layout->addWidget(hangup_button);

    main_layout->addWidget(call_buttons_frame);

    mic_button->setText(QString("Mute"));
    hangup_button->setText(QString("Hang up"));
}

void babel::CallPage::change_text()
{
    std::stringstream str_time;

    call_name->setText(QString(("Call with " + caller + static_cast<char const *>(awaiting ? " (Awaiting)" : "")).c_str()));
    str_time << "Call time: " << std::setfill('0') << std::setw(2) << timer.elapsed() / MINUTE << ":" << std::setfill('0') << std::setw(2)
             << (timer.elapsed() % MINUTE) / SECOND;
    call_time->setText(QString(str_time.str().c_str()));
}

void babel::CallPage::setup_informations(std::string const &name, bool await)
{
    awaiting = await;
    timer.restart();
    caller = name;
    call_name->setText(QString(("Call with " + caller + static_cast<char const *>(awaiting ? " (Awaiting)" : "")).c_str()));
    call_time->setText(QString("Call time: 00:00"));
}

void babel::CallPage::set_await(bool await) { awaiting = await; }

bool babel::CallPage::get_await() const { return (awaiting); }

QWidget &babel::CallPage::get_central_widget() { return (*central_widget); }

QPushButton &babel::CallPage::get_hangup() { return (*hangup_button); }

QPushButton &babel::CallPage::get_mute() { return (*mic_button); }