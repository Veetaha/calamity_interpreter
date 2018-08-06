#ifndef LOCATION
#define LOCATION
#include <iostream>
#include <string>
#include <array>
#include "vtem.h"

namespace Vtem {

    template<typename TDistance = int>
    class Point2D {
    public:
        DECL_DEFAULT_COPY_AND_MOVE(Point2D)

        TDistance y;
        TDistance x;

        explicit Point2D(const TDistance & y = TDistance(0),
                         const TDistance & x = TDistance(0));


        TDistance * data() noexcept;
        const TDistance * data() const noexcept;

        std::string toStdString();

        bool operator==(const Point2D & other) const noexcept;



        friend std::ostream & operator<<(std::ostream & stream, const Point2D & self){
            return stream << '(' << self.y << ", " << self.x << ')';
        }
    };


    template<typename TDistance>
    Point2D<TDistance>::Point2D(const TDistance & y, const TDistance & x) {
        this->x = x;
        this->y = y;
    }


    template<typename TDistance>
    std::string Point2D<TDistance>::toStdString() {
        return "(" + std::to_string(this->y) + " : " + std::to_string(this->x) + ")";
    }

    template<typename TDistance>
    bool Point2D<TDistance>::operator==(const Point2D<TDistance> & other) const noexcept{
        return Vtem::isEqual(x, other.x) && Vtem::isEqual(y, other.y);
    }

    template<typename TDistance>
    TDistance * Point2D<TDistance>::data() noexcept {
        return reinterpret_cast<TDistance *>(this);
    }

    template<typename TDistance>
    const TDistance * Point2D<TDistance>::data() const noexcept  {
        return reinterpret_cast<TDistance *>(this);
    }
}

#endif // LOCATION

