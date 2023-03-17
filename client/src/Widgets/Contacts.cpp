#include "Widgets/Contacts.hpp"

babel::ContactsPage::ContactsPage()
    : central_widget(new QWidget()), main_layout(new QHBoxLayout(central_widget)), contact_scroll_area(new QScrollArea(central_widget)),
      contacts_holder(new QWidget()), contacts_layout(new QVBoxLayout(contacts_holder))
{
    setup_ui();
}

void babel::ContactsPage::setup_ui()
{
    central_widget->setObjectName(QString::fromUtf8("central_widget"));

    main_layout->setObjectName(QString::fromUtf8("main_layout"));

    contact_scroll_area->setObjectName(QString::fromUtf8("contact_scroll_area"));
    contact_scroll_area->setWidgetResizable(true);

    contacts_holder->setObjectName(QString::fromUtf8("contacts_holder"));
    contacts_holder->setGeometry(QRect(0, 0, 624, 497));

    contacts_layout->setObjectName(QString::fromUtf8("contacts_layout"));

    contact_scroll_area->setWidget(contacts_holder);

    main_layout->addWidget(contact_scroll_area);
}

QWidget &babel::ContactsPage::get_central_widget() { return (*central_widget); }

QPushButton &babel::ContactsPage::add_contact(std::string const &name, int id)
{
    contacts.push_back(std::make_unique<babel::ContactButton>(id, name, contacts_holder, contacts_layout));
    return (contacts.back()->get_button());
}
void babel::ContactsPage::clear_contacts() { contacts.clear(); }

babel::ContactButton::ContactButton(int id, std::string const &name, QWidget *parent, QVBoxLayout *main_layout)
    : name(name), id(id), frame(new QFrame(parent)), layout(new QHBoxLayout(frame)), label(new QLabel(frame)),
      button(new QPushButton(frame))
{
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setMaximumSize(QSize(16777215, 50));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    layout->setObjectName(QString::fromUtf8("layout"));

    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);
    label->setText(QString::fromUtf8(name.c_str()));

    button->setObjectName(QString::fromUtf8("button"));
    button->setText(QString::fromUtf8("Call"));

    layout->addWidget(label);
    layout->addWidget(button);

    main_layout->addWidget(frame);
}

std::string &babel::ContactButton::get_name() { return (name); }

int &babel::ContactButton::get_id() { return (id); }

QPushButton &babel::ContactButton::get_button() { return (*button); }