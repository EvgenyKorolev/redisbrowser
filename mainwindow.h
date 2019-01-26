#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "redis.h"
#include <QMainWindow>
class QTextEdit;
class QLineEdit;
class QCheckBox;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void apply_command(const std::string& arg);
public slots:
    void slot_nametype_change();
    void slot_connect_change();
    void slot_apply_command();
    void slot_data_show();
// Слоты для быстрых комманд
    void slot_ping();
    void slot_info();
    void slot_quit();
    void slot_bgsave();
    void slot_dbsize();
    void slot_multi();
    void slot_exec();
    void slot_randomkey();
    void slot_keys();
    void slot_save();
    void slot_time();
    void slot_role();
private:
    redis* db;
    QTextEdit* central_txt;
    QTextEdit* command_txt;
    QLineEdit* host_ip;
    QLineEdit* host_url;
    QLineEdit* host_port;
    QLineEdit* host_pass;
    QCheckBox* use_url;
};

#endif // MAINWINDOW_H
