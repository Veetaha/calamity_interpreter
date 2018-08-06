#pragma once
#include "vtem.h"


namespace Cala {

    // Sentinel class for null value (contains no data members)
    class Undefined {
        Undefined() = default;
        static const Undefined singleton;
    public:
        DECL_NO_COPY_AND_MOVE(Undefined)
        static constexpr inline const Undefined & instance() {
            return singleton;
        }

        template<typename TSource>
        static constexpr inline const Undefined & cast(const TSource) {
            return singleton;
        }
    };

    inline const Undefined Undefined::singleton;
    inline constexpr const Undefined & undefined = Undefined::instance();


}