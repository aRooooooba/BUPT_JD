#ifndef CLOTH_CLASS_H
#define CLOTH_CLASS_H

#include "commodity_class.h"
#include "discount_class.h"
#include <qstring.h>

class cloth_CLASS : public commodity_CLASS
{
private:
    QString material;
    QString brand;  //Æ·ÅÆ
    QString clothType;
public:
    cloth_CLASS(){};
    cloth_CLASS(QString _title, QString _id, QString _description,
                float _price, discount_CLASS _discount, int _number,
                QString _place, QString _URL, QString _material, QString _brand, QString _clothType);
    QString get_material();
    QString get_brand();
    QString get_clothType();
    virtual float calcPrice(float originalPrice);
};

#endif // CLOTH_CLASS_H
