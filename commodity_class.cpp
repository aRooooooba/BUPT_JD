#include "commodity_class.h"
#include "discount_class.h"

commodity_CLASS::commodity_CLASS(QString _title, QString _id, QString _description,
                                 float _price, discount_CLASS _discount, int _number,
                                 QString _place, QString _URL)
{
    this->title = _title;
    this->id = _id;
    this->description = _description;
    this->price = _price;
    this->discount = _discount;
    this->number = _number;
    this->place = _place;
    this->URL = _URL;
}

QString commodity_CLASS::get_title()    {return this->title;}

QString commodity_CLASS::get_id()   {return this->id;}

QString commodity_CLASS::get_description()  {return this->description;}

float commodity_CLASS::get_price()  {return this->price;}

discount_CLASS commodity_CLASS::get_discount()  {return this->discount;}

int commodity_CLASS::get_number()   {return this->number;}

QString commodity_CLASS::get_place()    {return this->place;}

QString commodity_CLASS::get_URL()  {return this->URL;}

void commodity_CLASS::soldSome(int soldNUM)
{
    this->number -= soldNUM;
}

void commodity_CLASS::reSetNUM(int num)
{
    this->number += num;
}

float commodity_CLASS::calcPrice(float originalPrice)   {return originalPrice;}
