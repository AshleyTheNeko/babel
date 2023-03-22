#include "Packet.hpp"
#include "Widgets/MainWindow.hpp"

void babel::MainWindow::net_response()
{
    Packet temp;
    while (sock->read(temp.get_header(), HEADER_MAX_SIZE) > 0) {
        try {
            temp.set_body_size_from_header();
        } catch (const Error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        if (sock->read(temp.get_body(), temp.get_size()) < 1) {
            std::cerr << "Read Failure" << std::endl;
            break;
        }
        try {
            switch (temp.get_type()) {
                case babel::RequestType::LOGIN:
                    login_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::GET_USERS:
                    contacts_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::REGISTER:
                    register_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::GET_CALLS:
                    get_call_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::HANGUP:
                    hangup_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::CALL:
                case babel::RequestType::ACCEPT_CALL:
                    call_res({temp.get_body(), temp.get_body() + temp.get_size()}, temp.get_type() == RequestType::CALL);
                    break;
                case babel::RequestType::IN_CALL:
                    in_call_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                case babel::RequestType::CALL_PACKET:
                    call_packet_res({temp.get_body(), temp.get_body() + temp.get_size()});
                    break;
                default:
                    break;
            }
        } catch (babel::Error const &error) {
            std::cerr << error.what() << std::endl;
        }
    }
}

void babel::MainWindow::login_res(std::string &&body)
{
    if (body == "OK") {
        fetch_contacts();
        list->setCurrentWidget(&contact_page.get_central_widget());
    } else {
        log_page.set_error("Login failed");
    }
}

void babel::MainWindow::register_res(std::string &&body)
{
    if (body == "OK") {
        log_page.set_error("Registered new user");
    } else {
        log_page.set_error("Register failed: " + body);
    }
}

void babel::MainWindow::in_call_res(std::string &&body)
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

void babel::MainWindow::call_res(std::string &&body, bool type)
{
    if (body[0] == '0') {
        call = true;
        call_page.setup_informations(body.substr(1), type);
        list->setCurrentWidget(&call_page.get_central_widget());
    }
}

void babel::MainWindow::get_call_res(std::string &&body)
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

void babel::MainWindow::hangup_res(std::string &&body)
{
    if (body == "OK") {
        call = false;
        audio_manager->stop_streaming();
        list->setCurrentWidget(&contact_page.get_central_widget());
    }
}

void babel::MainWindow::contacts_res(std::string &&body)
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

void babel::MainWindow::call_packet_res(std::string &&body)
{
    audio_manager->get_bins(source_type::SPEAKER).push_back({body.begin(), body.end()});
}