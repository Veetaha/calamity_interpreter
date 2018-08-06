#pragma once
#include "vtem.h"

namespace Cala {

    // Sentinel class for null value (contains no data members)
    class Null {
    private:
        constexpr Null() = default;
        static const Null singleton;
    public:
        DECL_NO_COPY_AND_MOVE(Null)
        static constexpr inline const Null & instance() {
            return singleton;
        }
        template<typename TSource>
        static inline const Null & cast(const TSource) {
            return singleton;
        }
    };


    inline const Null Null::singleton;
    inline constexpr const Null & null = Null::instance();

}