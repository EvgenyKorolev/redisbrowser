#include "redis.h"
#include <cstring>
#include <algorithm>
#include <QEventLoop>
#include <QDataStream>
redis::redis() : QObject(nullptr), host_ip{"127.0.0.1"}, host_port{6379}, passwd{""}, status{false}
{
    sock = new QTcpSocket();
    QObject::connect(sock, SIGNAL(connected()), this, SLOT(slot_connected()));
    QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    QObject::connect(sock, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    QObject::connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_error(QAbstractSocket::SocketError)));
}
redis::redis(const std::string &addr, unsigned short port, const std::string &pass) :  QObject(nullptr), host_ip{addr}, host_port{port}, passwd{pass}, status{false}
{
    sock = new QTcpSocket();
    QObject::connect(sock, SIGNAL(connected()), this, SLOT(slot_connected()));
    QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    QObject::connect(sock, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    QObject::connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_error(QAbstractSocket::SocketError)));
    srv_connect();
}
redis::~redis()
{
    sock->close();
    delete sock;
}
bool redis::srv_connect()
{
    sock->close();
    QEventLoop loop0;
        connect(sock, SIGNAL(connected()), &loop0, SLOT(quit()));
        connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), &loop0, SLOT(quit()));
        sock->connectToHost(QString(host_ip.c_str()), host_port);
    loop0.exec();
    if (sock->state() != QAbstractSocket::ConnectedState){
      status = false;
      buffer = "Connected filed\n";
      return false;
    }
    status = true;
    buffer = "Connected OK\n";
    if (passwd != ""){
        auth();
    }
    return true;
}
void redis::change_host(const std::string& addr, unsigned short port, const std::string& pass)
{
    host_ip = addr;
    host_port = port;
    passwd = pass;
    srv_connect();
}
void redis::slot_connected()
{
    if (sock->state() != QAbstractSocket::ConnectedState){
      status = false;
      buffer = "Connected filed\n";
    } else {
        buffer = "Connected OK\n";
        status = true;
    }
    emit data_ready();
}
void redis::slot_readyRead()
{
    buffer = QString(sock->readAll()).toStdString();
    if (buffer.front() == '+'){
        buffer = buffer.substr(1, std::string::npos);
        if (strcom == "QUIT\n" || strcom == "quit\n") {
            my_closed = true;
        }
        emit data_ready();
    }
    if (buffer.front() == '-'){
         buffer = buffer.substr(1, std::string::npos);
         emit data_ready();
    }
    if (buffer.front() == ':'){
         buffer = buffer.substr(1, std::string::npos);
         emit data_ready();
    }
    if (buffer.front() == '$'){
         auto tmpp = buffer.find('\r');
         if (tmpp != std::string::npos){
            buffer = buffer.substr(tmpp, std::string::npos);
         }
         emit data_ready();
    }
    if (buffer.front() == '*'){
         buffer = buffer.substr(1, std::string::npos);
         emit data_ready();
    }
}
void redis::slot_disconnected()
{
    buffer = "Connected close\n";
    status = false;
    emit data_ready();
}
void redis::slot_error(QAbstractSocket::SocketError)
{
    if (my_closed) {
        status = false;
        my_closed = false;
        return;
    }
    buffer = "Connection error\n";
    status = false;
    emit data_ready();
}
bool redis::command(const std::string& arg)
{
    if (!status) {
        return false;
    }
    strcom = arg;
    if (strcom.back() != '\n'){
       strcom.push_back('\n');
    }
    sock->write(strcom.c_str());
    QEventLoop loop0;
        connect(sock, SIGNAL(readyRead()), &loop0, SLOT(quit()));
        connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), &loop0, SLOT(quit()));
    loop0.exec();
    QEventLoop loopX;
        connect(this, SIGNAL(data_ready()), &loopX, SLOT(quit()));
    loopX.exec();
    return true;
}
bool redis::auth()
{
    if (!status) {
        return false;
    }
    strcom = "AUTH " + passwd;
    sock->write(strcom.c_str());
    QEventLoop loop0;
        connect(sock, SIGNAL(readyRead()), &loop0, SLOT(quit()));
        connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), &loop0, SLOT(quit()));
    loop0.exec();
    QEventLoop loopX;
        connect(this, SIGNAL(data_ready()), &loopX, SLOT(quit()));
    loopX.exec();
    return true;
}
