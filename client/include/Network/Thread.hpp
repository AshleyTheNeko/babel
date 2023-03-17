#pragma once

#include <string>
#include <QThread>
#include <QTcpSocket>

namespace babel
{
    class NetThread : public QThread {
        public:
            NetThread(std::string &&request, QByteArray &bytes, QTcpSocket &sock, QObject *parent);
            NetThread(NetThread &&) = delete;
            NetThread(const NetThread &) = delete;
            NetThread &operator=(NetThread &&) = delete;
            NetThread &operator=(const NetThread &) = delete;
            ~NetThread() = default;

            void run() override;

        private:
            std::string request;
            QByteArray &bytes;
            QTcpSocket &sock;
    };
}