#include "book_class.h"
#include "commodity_class.h"
#include <qstring.h>

book_CLASS::book_CLASS(QString _title, QString _id, QString _description,
                       float _price, discount_CLASS _discount, int _number,
                       QString _place, QString _URL, QString _author, QString _publisher)
    :commodity_CLASS(_title, _id, _description, _price, _discount, _number, _place, _URL)
{
    this->author = _author;
    this->publisher = _publisher;
}

QString book_CLASS::get_author()    {return this->author;}

QString book_CLASS::get_publisher() {return this->publisher;}

float book_CLASS::calcPrice(float originalPrice)
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
