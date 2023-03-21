#include "Packet.hpp"
#include "Widgets/MainWindow.hpp"

void babel::MainWindow::hangup_click()
{
    std::string request = "0001/" + std::to_string(babel::RequestType::HANGUP) + "0";
    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::call_click(const std::string &caller)
{
    std::string header = std::to_string(caller.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::CALL);

    std::string request = header + caller;
    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::login_click()
{
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::LOGIN);
    std::string request = header + fields;

    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::register_click()
{
    std::string fields = log_page.get_username() + ";" + log_page.get_password();
    std::string header = std::to_string(fields.size());
    header =
        std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" + std::to_string(babel::RequestType::REGISTER);
    std::string request = header + fields;

    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::check_calls()
{
    if (call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::GET_CALLS) + "0";
    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::in_call()
{
    if (!call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::IN_CALL) + "0";
    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::fetch_contacts()
{
    if (call) {
        return;
    }
    std::string request = "0001/" + std::to_string(babel::RequestType::GET_USERS) + "0";
    sock->write(request.c_str());
    sock->waitForBytesWritten(500);
}

void babel::MainWindow::send_audio()
{
    if (!call) {
        return;
    }
    std::string request;
    while (!audio_manager->get_bins(source_type::MIC).empty()) {
        auto temp = audio_manager->get_bins(source_type::MIC).front();
        std::string fields = {temp.begin(), temp.end()};
        std::string header = std::to_string(fields.size());
        request.append(std::string(4 - std::min(static_cast<size_t>(4), header.size()), '0') + header + "/" +
            std::to_string(babel::RequestType::CALL_PACKET));
        request.append(fields);

        audio_manager->get_bins(source_type::MIC).erase(audio_manager->get_bins(source_type::MIC).begin());
    }
    sock->write(request.c_str(), static_cast<qint64>(request.length()));
    sock->waitForBytesWritten(500);
}