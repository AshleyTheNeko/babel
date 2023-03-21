#include "Packet.hpp"
#include "Widgets/MainWindow.hpp"

void babel::MainWindow::net_response()
{
    std::string bytes = sock->readAll().toStdString();
    // std::cout << bytes << std::endl;
    std::string body = bytes.substr(2);

    switch (std::strtol(bytes.substr(0, 1).c_str(), nullptr, 10)) {
        case babel::RequestType::LOGIN:
            login_res(body);
            break;
        case babel::RequestType::GET_USERS:
            contacts_res(body);
            break;
        case babel::RequestType::REGISTER:
            register_res(body);
            break;
        case babel::RequestType::GET_CALLS:
            get_call_res(body);
            break;
        case babel::RequestType::HANGUP:
            hangup_res(body);
            break;
        case babel::RequestType::CALL:
        case babel::RequestType::ACCEPT_CALL:
            call_res(body, (bytes.substr(0, 1) == std::to_string(CALL)));
            break;
        case babel::RequestType::IN_CALL:
            in_call_res(body);
            break;
        case babel::RequestType::CALL_PACKET:
            call_packet_res(body);
            break;
        default:
            break;
    }
}

void babel::MainWindow::login_res(std::string &body)
{
    if (body == "OK") {
        fetch_contacts();
        list->setCurrentWidget(&contact_page.get_central_widget());
    } else {
        log_page.set_error("Login failed");
    }
}

void babel::MainWindow::register_res(std::string &body)
{
    if (body == "OK") {
        log_page.set_error("Registered new user");
    } else {
        log_page.set_error("Register failed: " + body);
    }
}

void babel::MainWindow::in_call_res(std::string &body)
{
    switch (body[0]) {
        case '0':
            if (call) {
                call = false;
                list->setCurrentWidget(&contact_page.get_central_widget());
            }
            break;
        case '1':
            break;
        case '2':
            if (!audio_manager->streaming()) {
                audio_manager->start_call_threads(*this);
            }
            call_page.set_await(false);
            break;
        default:
            break;
    }
}

void babel::MainWindow::call_res(std::string &body, bool type)
{
    if (body[0] == '0') {
        call = true;
        // std::cout << "call" << call << std::endl;
        call_page.setup_informations(body.substr(1), type);
        list->setCurrentWidget(&call_page.get_central_widget());
    }
}

void babel::MainWindow::get_call_res(std::string &body)
{
    if (body[0] == '1') {
        call = true;
        msgbox = new QMessageBox(this);
        msgbox->setText((body.substr(1) + " is calling you.").c_str());
        msgbox->setInformativeText("Do you accept the call?");
        msgbox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox->setDefaultButton(QMessageBox::No);
        int ret = msgbox->exec();
        msgbox.clear();
        if (ret == QMessageBox::Yes) {
            std::string request = "0001/" + std::to_string(babel::RequestType::ACCEPT_CALL) + "0";
            sock->write(request.c_str());
        } else {
            call = false;
            hangup_click();
        }
    }
}

void babel::MainWindow::hangup_res(std::string &body)
{
    if (body == "OK") {
        call = false;
        audio_manager->stop_streaming();
        list->setCurrentWidget(&contact_page.get_central_widget());
    }
}

void babel::MainWindow::contacts_res(std::string &body)
{
    std::string token;
    size_t pos = 0;

    contact_page.clear_contacts();
    while ((pos = body.find(';')) != std::string::npos) {
        token = body.substr(0, pos);
        if (!token.empty()) {
            connect(&contact_page.add_contact(token), &QPushButton::clicked, this, [this, token] { babel::MainWindow::call_click(token); });
        }
        body.erase(0, pos + 1);
    }
}

void babel::MainWindow::call_packet_res(std::string &body)
{
    audio_manager->get_bins(source_type::SPEAKER).push_back({body.begin(), body.end()});
}