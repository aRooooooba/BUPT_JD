#ifndef DATE_CLASS_H
#define DATE_CLASS_H


class date_CLASS
{
private:
    int year, month, day;

public:
    date_CLASS();
    date_CLASS(int _year, int _month, int _day);
    int get_year();
    int get_month();
    int get_day();
    void saveDate();
    void dateCalculator();  //�������ڼ�һ
};

#endif // DATE_CLASS_H
