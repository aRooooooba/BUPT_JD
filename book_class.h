#ifndef BOOK_CLASS_H
#define BOOK_CLASS_H

#include "commodity_class.h"
#include "discount_class.h"
#include <qstring.h>

class book_CLASS : public commodity_CLASS
{
private:
    QString author;
    QString publisher;
public:
    book_CLASS(){};
    book_CLASS(QString _title, QString _id, QString _description,
               float _price, discount_CLASS _discount, int _number,
               QString _place, QString _URL, QString _author, QString _publisher);
    QString get_author();
    QString get_publisher();
    virtual float calcPrice(float originalPrice);
};

#endif // BOOK_CLASS_H
