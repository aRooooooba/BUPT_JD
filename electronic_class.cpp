#include "electronic_class.h"
#include "commodity_class.h"
#include <qstring.h>

electronic_CLASS::electronic_CLASS(QString _title, QString _id, QString _description,
                                   float _price, discount_CLASS _discount, int _number,
                                   QString _place, QString _URL, QString _voltage,
                                   QString _power, QString _brand, QString _originalPlace)
    :commodity_CLASS(_title, _id, _description, _price, _discount, _number, _place, _URL)
{
    this->voltage = _voltage;
    this->power = _power;
    this->brand = _brand;
    this->originalPlace = _originalPlace;
}

QString electronic_CLASS::get_voltage() {return this->voltage;}

QString electronic_CLASS::get_power() {return this->power;}

QString electronic_CLASS::get_brand()   {return this->brand;}

QString electronic_CLASS::get_originalPlace()   {return this->originalPlace;}

float electronic_CLASS::calcPrice(float originalPrice)
{
    if (1 == this->get_discount().get_discountType())
    {
        if (originalPrice >= this->get_discount().get_limit())
            return originalPrice * this->get_discount().get_multiple();
    }
    else if (2 == this->get_discount().get_discountType())
    {
        if (originalPrice >= this->get_discount().get_limit())
            return originalPrice - this->get_discount().get_minus();
    }
    return originalPrice;
}
