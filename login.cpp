#include "login.h"
#include "ui_login.h"
#include "user_class.h"
#include "date_class.h"
#include "book_class.h"
#include "varieties.h"
#include "registernewuser.h"
#include <qstring.h>
#include <qvector.h>
#include <QMessageBox>
#include <qnetwork.h>
#include <qtcpsocket.h>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    ui->username->setFocus();
    if (!loadUsers())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入用户信息失败！"), QMessageBox::Yes);
        reject();
    }
    if (!loadBooks())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入图书信息失败！"), QMessageBox::Yes);
        reject();
    }
    if (!loadClothes())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入衣物信息失败！"), QMessageBox::Yes);
        reject();
    }
    if (!loadElectronics())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入电器信息失败！"), QMessageBox::Yes);
        reject();
    }
    initDate();
    tcpSocket = new QTcpSocket(this);
    begin = startTimer(10000);
}

Login::~Login()
{
    delete ui;
    saveUsers();
}

void Login::initDate()
{
    ui->date->setText(QString("%1.%2.%3")
                      .arg(QString::number(date.get_year(), 10))
                      .arg(QString::number(date.get_month(), 10))
                      .arg(QString::number(date.get_day(), 10)));
}

void Login::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == begin && timeFlag)
    {
        date.dateCalculator();
        initDate();
        date.saveDate();
    }
}

void Login::on_acceptBTN_clicked()
{
    if (ui->username->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入用户名！"), QMessageBox::Yes);
        ui->username->setFocus();
        ui->password->clear();
    }
    else if (ui->password->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入密码！"), QMessageBox::Yes);
        ui->password->setFocus();
    }
    else
    {
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
        {
            if (iter->get_username() == ui->username->text())
            {
                if (iter->get_password() == ui->password->text())
                {
                    thisUser = iter;
                    ui->username->clear();
                    ui->username->setFocus();
                    ui->password->clear();
                    timeFlag = false;
                    if ("-1" != thisUser->get_bankAccount())
                    {
                        tcpSocket->abort();
                        tcpSocket->connectToHost("127.0.0.1", 6665);
                        QByteArray block;
                        QDataStream out(&block, QIODevice::WriteOnly);
                        out.setVersion(QDataStream::Qt_5_5);
                        out << (quint16) 0;
                        out << (quint16) 1; //直接登录
                        out << thisUser->get_bankAccount();
                        out.device()->seek(0);
                        out << (quint16) (block.size() - sizeof(quint16));
                        tcpSocket->write(block);
                    }
                    accept();
                }
                else
                {
                    QMessageBox::critical(this, "Error", QString::fromLocal8Bit("密码错误！"), QMessageBox::Yes);
                    ui->password->clear();
                    ui->password->setFocus();
                }
                break;
            }
        }
        if (users.end() == iter)
        {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("用户名不存在！"), QMessageBox::Yes);
            ui->username->clear();
            ui->username->setFocus();
            ui->password->clear();
        }
    }
}

void Login::on_exitBTN_clicked()
{
    reject();
}

void Login::on_registerNewUser_clicked()
{
    killTimer(begin);
    RegisterNewUser r;
    if (Accepted == r.exec())
        saveUsers();
    begin = startTimer(10000);
}
