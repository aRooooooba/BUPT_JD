#include "address_class.h"
#include <qstring.h>

address_CLASS::address_CLASS(QString _city, QString _receiverName, QString _receiverPhone, QString _detailedAddress)
{
    this->city = _city;
    this->receiverName = _receiverName;
    this->receiverPhone = _receiverPhone;
    this->detailedAddress = _detailedAddress;
}

QString address_CLASS::get_city()  {return this->city;}

QString address_CLASS::get_detailedAddress()    {return this->detailedAddress;}

QString address_CLASS::get_receiverName()   {return this->receiverName;}

QString address_CLASS::get_receiverPhone()  {return this->receiverPhone;}
