#include "Network/Thread.hpp"

babel::NetThread::NetThread(std::string &&request, QByteArray &bytes, QTcpSocket &sock, QObject *parent)
    : request(request), bytes(bytes), sock(sock), QThread(parent)
{
}

void babel::NetThread::run()
{
    sock.write(request.c_str());
    sock.waitForBytesWritten(1000);
    sock.waitForReadyRead(9000);
    bytes = sock.readAll();
}
