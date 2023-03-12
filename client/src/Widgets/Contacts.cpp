#include "Widgets/Contacts.hpp"

babel::ContactsPage::ContactsPage() { setup_ui(); }

void babel::ContactsPage::setup_ui()
{
    centralwidget = new QWidget();
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    horizontalLayout_10 = new QHBoxLayout(centralwidget);
    horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
    scrollArea = new QScrollArea(centralwidget);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 624, 497));
    verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    scrollArea->setWidget(scrollAreaWidgetContents);
    horizontalLayout_10->addWidget(scrollArea);
}

QWidget &babel::ContactsPage::get_central_widget() { return (*centralwidget); }

QPushButton &babel::ContactsPage::add_contact(std::string name, int id)
{
    contacts.push_back(std::make_shared<babel::ContactButton>(id, name, scrollAreaWidgetContents, verticalLayout));
    return (contacts.back()->get_button());
}
void babel::ContactsPage::clear_contacts(void) {}

babel::ContactButton::ContactButton(int id, std::string name, QWidget *parent, QVBoxLayout *main_layout) : name(name), id(id)
{
    frame = new QFrame(parent);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setMaximumSize(QSize(16777215, 50));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    layout = new QHBoxLayout(frame);
    layout->setObjectName(QString::fromUtf8("layout"));

    label = new QLabel(frame);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);
    label->setText(QString::fromUtf8(name.c_str()));

    button = new QPushButton(frame);
    button->setObjectName(QString::fromUtf8("button"));
    button->setText(QString::fromUtf8("Call"));

    layout->addWidget(label);
    layout->addWidget(button);

    main_layout->addWidget(frame);
}

std::string &babel::ContactButton::get_name(void) { return (name); }

int &babel::ContactButton::get_id(void) { return (id); }

QPushButton &babel::ContactButton::get_button(void) { return (*button); }