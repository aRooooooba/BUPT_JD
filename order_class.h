#ifndef ORDER_CLASS_H
#define ORDER_CLASS_H

#include <qstring.h>
#include "date_class.h"

class order_CLASS
{
private:
    int state;  //状态，0：购物车，1：代付款，2：已完成
    QString id;
    int number;
    date_CLASS refreshDate;
public:
    order_CLASS(){};
    order_CLASS(int _state, QString _id, int _number, date_CLASS _refreshDate);
    int get_state();
    QString get_id();
    int get_number();
    date_CLASS get_refreshDate();
    void set_number(int _number);
    void refreshState(int newState);
};

#endif // ORDER_CLASS_H
