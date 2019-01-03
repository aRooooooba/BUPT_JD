#include "varieties.h"
#include "user_class.h"
#include "date_class.h"
#include "address_class.h"
#include "discount_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"
#include "order_class.h"
#include <qvector.h>
#include <qstring.h>
#include <QFile>
#include <QTextStream>

QString usersFileAddress = "D:\\BUPT_JD\\txt\\usersFile.txt";
QString dateFileAddress = "D:\\BUPT_BANK\\txt\\dateFile.txt";
QString booksFileAddress = "D:\\BUPT_JD\\txt\\booksFile.txt";
QString clothesFileAddress = "D:\\BUPT_JD\\txt\\clothesFile.txt";
QString electronicsFileAddress = "D:\\BUPT_JD\\txt\\electronicsFile.txt";
QString deleteBtnURL = "{border-image:url(:/delete.png)}";
QVector <user_CLASS> users;
QVector <book_CLASS> books;
QVector <cloth_CLASS> clothes;
QVector <electronic_CLASS> electronics;
date_CLASS date = date_CLASS();
user_CLASS * thisUser = NULL;
bool timeFlag = true;

bool loadUsers()
{
    QFile usersFile(usersFileAddress);
    if (!usersFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&usersFile);
    QString username, password, bankAccount, city, receiverName, receiverPhone, detailedAddress, id, head;
    int addressNUM, orderNUM, state, number, year, month, day;
    input >> head >> username;
    while (head != "$")
    {
        input >> head >> password
              >> head >> bankAccount
              >> head >> addressNUM;
        user_CLASS anotherUser = user_CLASS(username, password, bankAccount);
        for (int i = 0; i < addressNUM; i++)
        {
            input >> head >> city
                  >> head >> receiverName
                  >> head >> receiverPhone
                  >> head >> detailedAddress;
            anotherUser.add_address(address_CLASS(city, receiverName, receiverPhone, detailedAddress));
        }
        order_CLASS anotherOrder;
        input >> head >> orderNUM;
        for (int i = 0; i < orderNUM; i++)
        {
            input >> head >> state
                  >> head >> id
                  >> head >> number
                  >> head >> year >> month >> day;
            anotherOrder = order_CLASS(state, id, number, date_CLASS(year, month, day));
            anotherUser.add_order(anotherOrder);
        }
        users.push_back(anotherUser);
        input >> head >> head >> username;
    }
    usersFile.close();
    return true;
}

void saveUsers()
{
    QFile usersFile(usersFileAddress);
    usersFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&usersFile);
    QVector <user_CLASS>::iterator iter;
    for (iter = users.begin(); iter != users.end(); iter++)
    {
        output << QString::fromLocal8Bit("用户名：") << "         " << iter->get_username() << endl
               << QString::fromLocal8Bit("密码：") << "           " << iter->get_password() << endl
               << QString::fromLocal8Bit("关联的银行账户：") << " " << iter->get_bankAccount() << endl
               << QString::fromLocal8Bit("地址数量：") << "       " << iter->get_addressNUM() << endl;
        for (int i = 0; i < iter->get_addressNUM(); i++)
            output << QString::fromLocal8Bit("城市：") << "           " << iter->get_address(i)->get_city() << endl
                   << QString::fromLocal8Bit("收件人姓名：") << "     " << iter->get_address(i)->get_receiverName() << endl
                   << QString::fromLocal8Bit("收件人手机：") << "     " << iter->get_address(i)->get_receiverPhone() << endl
                   << QString::fromLocal8Bit("详细地址：") << "       " << iter->get_address(i)->get_detailedAddress() << endl;
        output << QString::fromLocal8Bit("订单数量：") << "       " << iter->get_orderNUM() << endl;
        for (int i = 0; i < iter->get_orderNUM(); i++)
            output << QString::fromLocal8Bit("订单状态：") << "       " << iter->get_order(i)->get_state() << endl
                   << QString::fromLocal8Bit("商品编号：") << "       " << iter->get_order(i)->get_id() << endl
                   << QString::fromLocal8Bit("物品数量：") << "       " << iter->get_order(i)->get_number() << endl
                   << QString::fromLocal8Bit("更新日期：") << "       " << iter->get_order(i)->get_refreshDate().get_year() << " "
                                                                    << iter->get_order(i)->get_refreshDate().get_month() << " "
                                                                    << iter->get_order(i)->get_refreshDate().get_day() << endl;
        output << "#" << endl;
    }
    output << "$";
    usersFile.close();
}

bool loadBooks()
{
    QFile booksFile(booksFileAddress);
    if (!booksFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&booksFile);
    QString title, id, description, place, URL, author, publisher, head;
    float price, multiple;
    int discountType, minus, limit, number;
    discount_CLASS discount;
    input >> head >> title;
    while (head != "$")
    {
        input >> head >> id
              >> head >> description
              >> head >> price
              >> head >> discountType
              >> head >> multiple
              >> head >> minus
              >> head >> limit
              >> head >> number
              >> head >> place
              >> head >> URL
              >> head >> author
              >> head >> publisher;
        discount = discount_CLASS(discountType, multiple, minus, limit);
        books.push_back(book_CLASS(title, id, description, price, discount, number, place, URL, author, publisher));
        input >> head >> head >> title;
    }
    booksFile.close();
    return true;
}

bool loadClothes()
{
    QFile clothesFile(clothesFileAddress);
    if (!clothesFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&clothesFile);
    QString title, id, description, place, URL, material, brand, clothType, head;
    float price, multiple;
    int discountType, minus, limit, number;
    discount_CLASS discount;
    input >> head >> title;
    while (head != "$")
    {
        input >> head >> id
              >> head >> description
              >> head >> price
              >> head >> discountType
              >> head >> multiple
              >> head >> minus
              >> head >> limit
              >> head >> number
              >> head >> place
              >> head >> URL
              >> head >> material
              >> head >> brand
              >> head >> clothType;
        discount = discount_CLASS(discountType, multiple, minus, limit);
        clothes.push_back(cloth_CLASS(title, id, description, price, discount, number, place, URL, material, brand, clothType));
        input >> head >> head >> title;
    }
    clothesFile.close();
    return true;
}

bool loadElectronics()
{
    QFile electronicsFile(electronicsFileAddress);
    if (!electronicsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&electronicsFile);
    QString title, id, description, place, URL, voltage, power, brand, originalPlace, head;
    float price, multiple;
    int discountType, minus, limit, number;
    discount_CLASS discount;
    input >> head >> title;
    while (head != "$")
    {
        input >> head >> id
              >> head >> description
              >> head >> price
              >> head >> discountType
              >> head >> multiple
              >> head >> minus
              >> head >> limit
              >> head >> number
              >> head >> place
              >> head >> URL
              >> head >> voltage
              >> head >> power
              >> head >> brand
              >> head >> originalPlace;
        discount = discount_CLASS(discountType, multiple, minus, limit);
        electronics.push_back(electronic_CLASS(title, id, description, price, discount, number, place, URL, voltage, power, brand, originalPlace));
        input >> head >> head >> title;
    }
    electronicsFile.close();
    return true;
}

bool findItem(QString id, book_CLASS * &thisBook, cloth_CLASS * &thisCloth, electronic_CLASS * &thisElectronic)
{
    QVector <book_CLASS>::iterator iterBook;
    for (iterBook = books.begin(); iterBook != books.end(); iterBook++)
        if (id == iterBook->get_id())
            break;
    if (books.end() != iterBook)
    {
        thisBook = iterBook;
        thisCloth = NULL;
        thisElectronic = NULL;
        return true;
    }
    QVector <cloth_CLASS>::iterator iterCloth;
    for (iterCloth = clothes.begin(); iterCloth != clothes.end(); iterCloth++)
        if (id == iterCloth->get_id())
            break;
    if (clothes.end() != iterCloth)
    {
        thisBook = NULL;
        thisCloth = iterCloth;
        thisElectronic = NULL;
        return true;
    }
    QVector <electronic_CLASS>::iterator iterElectronic;
    for (iterElectronic = electronics.begin(); iterElectronic != electronics.end(); iterElectronic++)
        if (id == iterElectronic->get_id())
            break;
    if (electronics.end() != iterElectronic)
    {
        thisBook = NULL;
        thisCloth = NULL;
        thisElectronic = iterElectronic;
        return true;
    }
    return false;
}
