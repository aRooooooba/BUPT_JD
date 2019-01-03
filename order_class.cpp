#include "order_class.h"
#include "date_class.h"
#include "varieties.h"
#include <qstring.h>

order_CLASS::order_CLASS(int _state, QString _id, int _number, date_CLASS _refreshDate)
{
    this->state = _state;
    this->id = _id;
    this->number = _number;
    this->refreshDate = _refreshDate;
}

int order_CLASS::get_state()    {return this->state;}

QString order_CLASS::get_id()  {return this->id;}

int order_CLASS::get_number()   {return this->number;}

date_CLASS order_CLASS::get_refreshDate()   {return this->refreshDate;}

void order_CLASS::set_number(int _number)
{
    this->number = _number;
}

void order_CLASS::refreshState(int newState)
{
    this->state = newState;
    this->refreshDate = date;
}
