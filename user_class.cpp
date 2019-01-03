#include "user_class.h"
#include "address_class.h"
#include "order_class.h"
#include <qstring.h>
#include <qvector.h>

user_CLASS::user_CLASS(QString _username, QString _password, QString _bankAccount)
{
    this->username = _username;
    this->password = _password;
    this->bankAccount = _bankAccount;
    this->addressNUM = 0;
    this->orderNUM = 0;
}

QString user_CLASS::get_username()  {return this->username;}

QString user_CLASS::get_password()  {return this->password;}

QString user_CLASS::get_bankAccount()  {return this->bankAccount;}

int user_CLASS::get_addressNUM()    {return this->address.size();}

void user_CLASS::set_bankAccount(QString _bankAccount)
{
    this->bankAccount = _bankAccount;
}

address_CLASS * user_CLASS::get_address(int i)    {return &(this->address[i]);}

void user_CLASS::add_address(address_CLASS anotherAddress)
{
    this->address.push_back(anotherAddress);
    this->addressNUM++;
}

void user_CLASS::delete_address(int thisAddress)
{
    QVector <address_CLASS>::iterator iter;
    for (iter = this->address.begin(); iter != this->address.begin() + thisAddress; iter++);
    this->address.erase(iter);
    this->addressNUM--;
}

void user_CLASS::editPassword(QString newPassword)
{
    this->password = newPassword;
}

int user_CLASS::get_orderNUM()  {return this->orderNUM;}

order_CLASS * user_CLASS::get_order(int i)    {return &(this->order[i]);}

void user_CLASS::add_order(order_CLASS anotherOrder)
{
    this->order.push_back(anotherOrder);
    this->orderNUM++;
}

void user_CLASS::delete_order(int thisOrder)
{
    QVector <order_CLASS>::iterator iter;
    for (iter = this->order.begin(); iter != this->order.begin() + thisOrder; iter++);
    this->order.erase(iter);
    this->orderNUM--;
}
