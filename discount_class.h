#ifndef DISCOUNT_CLASS_H
#define DISCOUNT_CLASS_H


class discount_CLASS
{
private:
    int discountType;   //0:�ޣ�1:�ۿۣ�2:����
    float multiple;
    int minus;
    int limit;
public:
    discount_CLASS(){};
    discount_CLASS(int _discountType, float _multiple, int _minus, int _limit);
    int get_discountType();
    float get_multiple();
    int get_minus();
    int get_limit();
};

#endif // DISCOUNT_CLASS_H
