#ifndef LOCATION
#define LOCATION
#include <iostream>
#include <string>
#include "std_ext.h"

template <typename TDistance = int>
class Point2D{
public:
    TDistance y;
    TDistance x;

    explicit Point2D(const TDistance & y = 0, const TDistance & x = 0);
    std::string toStdString();
    bool operator==(const Point2D & other) const ;

    // https://stackoverflow.com/a/44712138/9259330
    template <typename TTDistance>
    friend std::ostream & operator <<(std::ostream &stream, const Point2D<TTDistance> & self);
};




template <typename TDistance>
Point2D<TDistance>::Point2D(const TDistance & y, const TDistance & x){
    this->x = x;
    this->y = y;
}

template <typename TDistance>
std::ostream & operator <<(std::ostream & stream, const Point2D<TDistance> & self){
    return stream << '(' << self.y << ", " << self.x << ')';
}

template <typename TDistance>
std::string Point2D<TDistance>::toStdString(){
    return "(" + std::to_string(this->y) + " : " + std::to_string(this->x) + ")";
}

template<typename TDistance>
bool Point2D<TDistance>::operator==(const Point2D<TDistance> & other) const {
    return std_ext::isEqual(x, other.x) && std_ext::isEqual(y, other.y);
}


#endif // LOCATION

