#ifndef USER_CLASS_H
#define USER_CLASS_H

#include <qstring.h>
#include <qvector.h>
#include "address_class.h"
#include "order_class.h"

class user_CLASS
{
private:
    QString username;
    QString password;
    QString bankAccount; //用于连接银行系统的用户
    int addressNUM;
    QVector <address_CLASS> address;
    int orderNUM;
    QVector <order_CLASS> order;

public:
    user_CLASS(){};
    user_CLASS(QString _username, QString _password, QString _bankAccount);
    QString get_username();
    QString get_password();
    QString get_bankAccount();
    void set_bankAccount(QString _bankAccount);
    int get_addressNUM();
    address_CLASS * get_address(int i);
    void add_address(address_CLASS anotherAddress);
    void delete_address(int thisAddress);
    void editPassword(QString newPassword);
    int get_orderNUM();
    order_CLASS * get_order(int i);
    void add_order(order_CLASS anotherOrder);
    void delete_order(int thisOrder);
};

#endif // USER_CLASS_H
