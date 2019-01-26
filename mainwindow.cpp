#include "mainwindow.h"
#include "QTextEdit"
#include "QDockWidget"
#include "QGridLayout"
#include "QPushButton"
#include "QLineEdit"
#include "QCheckBox"
#include "QLabel"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
MainWindow::MainWindow(QWidget *parent)
    :  QMainWindow(parent)
{
    this->resize(1100, 700);
    this->setWindowTitle("Учёт и хранение");
  //  this->setWindowIcon(QIcon(":/images/w.png"));
//----------------------------------------------------------------
    central_txt = new QTextEdit();
    central_txt->setReadOnly(true);
    this->setCentralWidget(central_txt);
//----------------------------------------------------------------
    QDockWidget* settings_area = new QDockWidget("Настройки", this);
    QGridLayout* settings_lay = new QGridLayout();
    QWidget* proxy_widget = new QWidget();
    proxy_widget->setLayout(settings_lay);
    settings_area->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addDockWidget(Qt::TopDockWidgetArea, settings_area);
    QPushButton* but_apply_set = new QPushButton("Применить");
    QPushButton* but_apply_command = new QPushButton("Выполнить");
    QLabel* url_titl = new QLabel();
    url_titl->setText("По URL");
    use_url = new QCheckBox();
    use_url->setChecked(false);
    host_url = new QLineEdit();
    host_url->setText("localhost");
    host_url->setReadOnly(true);
    host_ip = new QLineEdit();
    host_ip->setInputMask("000.000.000.000");
    host_ip->setText("127.0.0.1");
    host_port = new QLineEdit();
    host_port->setText("6379");
    host_port->setInputMask("0000000");
    host_pass = new QLineEdit();
    host_pass->setEchoMode(QLineEdit::Password);
    command_txt = new QTextEdit();
    command_txt->setMaximumHeight(50);
    QObject::connect(but_apply_command, SIGNAL(clicked()), this, SLOT(slot_apply_command()));
    QObject::connect(but_apply_set, SIGNAL(clicked()), this, SLOT(slot_connect_change()));
    QObject::connect(use_url, SIGNAL(stateChanged(int)), this, SLOT(slot_nametype_change()));
    settings_lay->addWidget(url_titl, 0, 0);
    settings_lay->addWidget(use_url, 0, 1);
    settings_lay->addWidget(host_url, 0, 2);
    settings_lay->addWidget(host_ip, 0, 3);
    settings_lay->addWidget(host_port, 0, 4);
    settings_lay->addWidget(host_pass, 0, 5);
    settings_lay->addWidget(but_apply_set, 0, 6);
    settings_lay->addWidget(command_txt, 1, 0, 1, 7);
    settings_lay->addWidget(but_apply_command, 2, 0, 1, 7);
    settings_area->setWidget(proxy_widget);
//----------------------------------------------------------------
    QDockWidget* buttons_area = new QDockWidget("Быстрые комнды", this);
    QGridLayout* buttons_lay = new QGridLayout();
    settings_area->setLayout(buttons_lay);
    buttons_area->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, buttons_area);
    QWidget* proxy_widget2 = new QWidget();
    buttons_area->setWidget(proxy_widget2);
    proxy_widget2->setLayout(buttons_lay);
    QPushButton* but_ping = new QPushButton("Пинг");
    QPushButton* but_info = new QPushButton("Инфо");
    QPushButton* but_quit = new QPushButton("Отключиться");
    QPushButton* but_bgsave = new QPushButton("Асинхронно на диск");
    QPushButton* but_dbsize = new QPushButton("Размер базы");
    QPushButton* but_multi = new QPushButton("Записать транзакцию");
    QPushButton* but_exec = new QPushButton("Выполнить тразакцию");
    QPushButton* but_randomkey = new QPushButton("Случайный ключ");
    QPushButton* but_keys = new QPushButton("Все ключи");
    QPushButton* but_save = new QPushButton("Синхронно на диск");
    QPushButton* but_time = new QPushButton("Время");
    QPushButton* but_role = new QPushButton("Роли");
//    QPushButton* but_info = new QPushButton("Инфо");
//    QPushButton* but_info = new QPushButton("Инфо");
//    QPushButton* but_info = new QPushButton("Инфо");
//    QPushButton* but_info = new QPushButton("Инфо");
//    QPushButton* but_info = new QPushButton("Инфо");
//    QPushButton* but_info = new QPushButton("Инфо");
    QObject::connect(but_ping, SIGNAL(clicked()), this, SLOT(slot_ping()));
    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
    QObject::connect(but_quit, SIGNAL(clicked()), this, SLOT(slot_quit()));
    QObject::connect(but_bgsave, SIGNAL(clicked()), this, SLOT(slot_bgsave()));
    QObject::connect(but_dbsize, SIGNAL(clicked()), this, SLOT(slot_dbsize()));
    QObject::connect(but_multi, SIGNAL(clicked()), this, SLOT(slot_multi()));
    QObject::connect(but_exec, SIGNAL(clicked()), this, SLOT(slot_exec()));
    QObject::connect(but_randomkey, SIGNAL(clicked()), this, SLOT(slot_randomkey()));
    QObject::connect(but_keys, SIGNAL(clicked()), this, SLOT(slot_keys()));
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(slot_save()));
    QObject::connect(but_time, SIGNAL(clicked()), this, SLOT(slot_time()));
    QObject::connect(but_role, SIGNAL(clicked()), this, SLOT(slot_role()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
//    QObject::connect(but_info, SIGNAL(clicked()), this, SLOT(slot_info()));
    buttons_lay->addWidget(but_ping);
    buttons_lay->addWidget(but_info);
    buttons_lay->addWidget(but_quit);
    buttons_lay->addWidget(but_bgsave);
    buttons_lay->addWidget(but_save);
    buttons_lay->addWidget(but_dbsize);
    buttons_lay->addWidget(but_multi);
    buttons_lay->addWidget(but_exec);
    buttons_lay->addWidget(but_randomkey);
    buttons_lay->addWidget(but_keys);
    buttons_lay->addWidget(but_time);
    buttons_lay->addWidget(but_role);
//    buttons_lay->addWidget(but_info);
//    buttons_lay->addWidget(but_info);
//    buttons_lay->addWidget(but_info);
//    buttons_lay->addWidget(but_info);
//    buttons_lay->addWidget(but_info);
//    buttons_lay->addWidget(but_info);
    db = new redis();
    QObject::connect(db, SIGNAL(data_ready()), this, SLOT(slot_data_show()));
    if (!db->is_ready()) db->srv_connect();
}

MainWindow::~MainWindow()
{
    delete central_txt;
    delete command_txt;
    delete host_ip;
    delete host_url;
    delete host_port;
    delete host_pass;
    delete use_url;
    delete db;
}
void MainWindow::slot_connect_change()
{
    if (use_url->isChecked()){
        hostent* retad;
        retad = gethostbyname(host_url->text().toStdString().c_str());
        if (retad == nullptr){
            central_txt->insertPlainText("Host " + host_url->text() + " not available\n");
            host_ip->setText("127.0.0.1");
            host_url->setText("localhost");
            return;
        }
        host_ip->setText(inet_ntoa(*((struct in_addr*)retad->h_addr_list[0])));
    } else {
        if (host_ip->text().mid(0, 3).toInt() > 255 ||
            host_ip->text().mid(4, 3).toInt() > 255 ||
            host_ip->text().mid(8, 3).toInt() > 255 ||
            host_ip->text().mid(13, 3).toInt() > 255) {
            central_txt->insertPlainText("Uncorrect ip: " + host_ip->text() + ". use default ip: 127.0.0.1\n");
            host_ip->setText("127.0.0.1");
        }
        host_url->setReadOnly(true);
      }
        db->change_host(host_ip->text().toStdString(), host_port->text().toUShort());
}
void MainWindow::slot_nametype_change()
{
    if (use_url->isChecked()){
        host_url->setReadOnly(false);
    } else {
        host_url->setReadOnly(true);
    }
}
void MainWindow::slot_data_show()
{
    central_txt->insertPlainText(QString(db->ret_data().c_str()));
}
void MainWindow::slot_apply_command()
{
    if (!db->is_ready()){
        central_txt->insertPlainText("Connection filed\n");
        return;
    }
    if (db->command(command_txt->toPlainText().toStdString())){
        return;
    }
    central_txt->insertPlainText("Command error\n");
}
void MainWindow::apply_command(const std::string& arg)
{
    if (!db->is_ready()){
        central_txt->insertPlainText("Connection filed\n");
        return;
    }
    if (db->command(arg)){
        return;
    }
    central_txt->insertPlainText("Command error\n");
}
void MainWindow::slot_ping(){apply_command("PING\n");}
void MainWindow::slot_info(){apply_command("INFO\n");}
void MainWindow::slot_quit(){apply_command("QUIT\n");}
void MainWindow::slot_bgsave(){apply_command("BGSAVE\n");}
void MainWindow::slot_save(){apply_command("SAVE\n");}
void MainWindow::slot_dbsize(){apply_command("DBSIZE\n");}
void MainWindow::slot_multi(){apply_command("MULTI\n");}
void MainWindow::slot_exec(){apply_command("EXEC\n");}
void MainWindow::slot_randomkey(){apply_command("RANDOMKEY\n");}
void MainWindow::slot_keys(){apply_command("KEYS *\n");}
void MainWindow::slot_time(){apply_command("TIME\n");}
void MainWindow::slot_role(){apply_command("ROLE\n");}
