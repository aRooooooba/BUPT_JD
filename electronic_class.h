#ifndef ELECTRONIC_CLASS_H
#define ELECTRONIC_CLASS_H

#include "commodity_class.h"
#include "discount_class.h"
#include <qstring.h>

class electronic_CLASS : public commodity_CLASS
{
private:
    QString voltage;
    QString power;
    QString brand;
    QString originalPlace;
public:
    electronic_CLASS(){};
    electronic_CLASS(QString _title, QString _id, QString _description,
                     float _price, discount_CLASS _discount, int _number,
                     QString _place, QString _URL, QString _voltage,
                     QString _power, QString _brand, QString _originalPlace);
    QString get_voltage();
    QString get_power();
    QString get_brand();
    QString get_originalPlace();
    virtual float calcPrice(float originalPrice);
};

#endif // ELECTRONIC_CLASS_H
