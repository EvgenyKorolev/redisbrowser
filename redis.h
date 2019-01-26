#ifndef REDIS_H
#define REDIS_H
#include <QTcpSocket>
#include <QObject>
#define R_DEBUG
enum class section{server, clients, memory, persistence, stats, replication, cpu, cluster, keyspace, all};
class redis : public QObject
{
    Q_OBJECT
public:
    redis();
    redis(const std::string& addr, unsigned short port, const std::string& pass = "");
    ~redis();
    // ------------------------------------------------
    // операторы по умолчению с ростом класса придётся возможно реализовать или удалить
    redis(const redis& arg) = default;
    redis(redis&& arg) = default;
    redis& operator=(const redis& arg) = default;
    redis& operator=(redis&& arg) = default;
    // ------------------------------------------------
    // Всякие общие и службные функции
    bool srv_connect();
    void change_host(const std::string& addr, unsigned short port, const std::string& pass = "");
    bool command(const std::string& arg); // выполняет произвольную комманду
    bool is_ready() const {return  status;}
    std::string ret_data() const {return buffer;}
    // ------------------------------------------------
public slots:
    void slot_connected();
    void slot_readyRead();
    void slot_disconnected();
    void slot_error(QAbstractSocket::SocketError);
signals:
    void data_ready();
private:
    QTcpSocket* sock;
    std::string host_ip;
    unsigned short host_port;
    std::string passwd{""};
    bool status{false};
    std::string buffer{""};
    std::string strcom{""};
    bool my_closed{false};
    bool auth();
};

#endif // REDIS_H

