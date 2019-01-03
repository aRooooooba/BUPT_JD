#include "discount_class.h"

discount_CLASS::discount_CLASS(int _discountType, float _multiple, int _minus, int _limit)
{
    this->discountType = _discountType;
    this->multiple = _multiple;
    this->minus = _minus;
    this->limit = _limit;
}

int discount_CLASS::get_discountType()  {return this->discountType;}

float discount_CLASS::get_multiple()    {return this->multiple;}

int discount_CLASS::get_minus() {return this->minus;}

int discount_CLASS::get_limit() {return this->limit;}
