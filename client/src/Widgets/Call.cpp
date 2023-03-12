#include "Widgets/Call.hpp"
#include <iomanip>
#include <sstream>

babel::CallPage::CallPage() { setup_ui(); }

void babel::CallPage::setup_ui(void)
{
    centralwidget = new QWidget();
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    frame = new QFrame(centralwidget);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    verticalLayout_2 = new QVBoxLayout(frame);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    label = new QLabel(frame);
    label->setObjectName(QString::fromUtf8("label"));
    QFont font;
    font.setPointSize(25);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout_2->addWidget(label);

    label_2 = new QLabel(frame);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QFont font1;
    font1.setPointSize(20);
    label_2->setFont(font1);
    label_2->setScaledContents(false);
    label_2->setAlignment(Qt::AlignCenter);
    label_2->setIndent(0);

    verticalLayout_2->addWidget(label_2);

    verticalLayout->addWidget(frame);

    frame_2 = new QFrame(centralwidget);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame_2);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    pushButton_2 = new QPushButton(frame_2);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    QFont font2;
    font2.setPointSize(10);
    pushButton_2->setFont(font2);

    horizontalLayout->addWidget(pushButton_2);

    pushButton = new QPushButton(frame_2);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    horizontalLayout->addWidget(pushButton);

    verticalLayout->addWidget(frame_2);

    pushButton_2->setText(QString("Mute"));
    pushButton->setText(QString("Hang up"));
}

void babel::CallPage::change_text(void)
{
    std::stringstream str_time;

    str_time << "Call time: " << std::setfill('0') << std::setw(2) << timer.elapsed() / 60000 << ":" << std::setfill('0') << std::setw(2)
             << (timer.elapsed() % 60000) / 1000;
    label_2->setText(QString(str_time.str().c_str()));
}

void babel::CallPage::setup_informations(std::string name, int start_time)
{
    std::stringstream str_name;
    std::stringstream str_time;

    timer.restart();
    str_name << "Call with " << name;
    label->setText(QString(str_name.str().c_str()));
    str_time << "Call time: " << std::setfill('0') << std::setw(2) << timer.elapsed() / 60000 << ":" << std::setfill('0') << std::setw(2)
             << (timer.elapsed() % 60000) / 1000;
    label_2->setText(QString(str_time.str().c_str()));
}

QWidget &babel::CallPage::get_central_widget() { return (*centralwidget); }

QPushButton &babel::CallPage::get_hangup() { return (*pushButton); }

QPushButton &babel::CallPage::get_mute() { return (*pushButton_2); }