#ifndef COMMODITY_CLASS_H
#define COMMODITY_CLASS_H

#include "discount_class.h"
#include <qstring.h>

class commodity_CLASS
{
private:
    QString title;
    QString id;
    QString description;
    float price;
    discount_CLASS discount;
    int number;
    QString place;
    QString URL;
public:
    commodity_CLASS(){};
    commodity_CLASS(QString _title, QString _id, QString _description,
                    float _price, discount_CLASS _discount, int _number,
                    QString _place, QString _URL);
    QString get_title();
    QString get_id();
    QString get_description();
    float get_price();
    discount_CLASS get_discount();
    int get_number();
    QString get_place();
    QString get_URL();
    void soldSome(int soldNUM);
    void reSetNUM(int num);
    virtual float calcPrice(float originalPrice);
};

#endif // COMMODITY_CLASS_H
