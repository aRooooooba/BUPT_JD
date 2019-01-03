#include "cloth_class.h"
#include "commodity_class.h"
#include <qstring.h>

cloth_CLASS::cloth_CLASS(QString _title, QString _id, QString _description,
                         float _price, discount_CLASS _discount, int _number,
                         QString _place, QString _URL, QString _material, QString _brand, QString _clothType)
    :commodity_CLASS(_title, _id, _description, _price, _discount, _number, _place, _URL)
{
    this->material = _material;
    this->brand = _brand;
    this->clothType = _clothType;
}

QString cloth_CLASS::get_material() {return this->material;}

QString cloth_CLASS::get_brand()    {return this->brand;}

QString cloth_CLASS::get_clothType()    {return this->clothType;}

float cloth_CLASS::calcPrice(float originalPrice)
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
