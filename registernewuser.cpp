#include "registernewuser.h"
#include "ui_registernewuser.h"
#include "user_class.h"
#include "varieties.h"
#include <QMessageBox>
#include <qstring.h>
#include <qvector.h>

RegisterNewUser::RegisterNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterNewUser)
{
    ui->setupUi(this);
}

RegisterNewUser::~RegisterNewUser()
{
    delete ui;
}

void RegisterNewUser::on_ensured_clicked()
{
    if (ui->username->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("�������û�����"), QMessageBox::Yes);
        ui->username->setFocus();
    }
    else if (ui->password->text().isEmpty() || ui->password_2->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("���������룡"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else if (ui->password->text().size() < 6)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("������̣�����Ҫ6λ��"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else if (ui->password->text() != ui->password_2->text())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("�������벻һ�£�"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else
    {
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
            if (iter->get_username() == ui->username->text())
                break;
        if (users.end() != iter)
        {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("�û�����ע�����"), QMessageBox::Yes);
            ui->username->clear();
            ui->username->setFocus();
        }
        else if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("ȷ�Ϻ󽫲��ɸ��ģ�\nȷ�������"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {
            if (ui->bankAccount->text().isEmpty())
            {
                users.append(user_CLASS(ui->username->text(), ui->password->text(), "-1"));
                QMessageBox::information(this, "OK", QString::fromLocal8Bit("ע��ɹ���"), QMessageBox::Yes);
                accept();
            }
        }
    }
}

void RegisterNewUser::on_cancel_clicked()
{
    if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("ȷ���˳���"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        reject();
}
