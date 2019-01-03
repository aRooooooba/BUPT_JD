#ifndef ADDRESS_CLASS_H
#define ADDRESS_CLASS_H

#include <qstring.h>

class address_CLASS
{
private:
    QString city;
    QString receiverName;
    QString receiverPhone;
    QString detailedAddress;
public:
    address_CLASS(){};
    address_CLASS(QString _city, QString _receiverName, QString _receiverPhone, QString _detailedAddress);
    QString get_city();
    QString get_detailedAddress();
    QString get_receiverName();
    QString get_receiverPhone();
};

#endif // ADDRESS_CLASS_H
