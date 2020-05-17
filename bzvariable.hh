#ifndef _BZVARIABLE_HH_
#define _BZVARIABLE_HH_

#include <cstddef>

namespace benzaiten
{
    template <char Id, size_t Order = 0>
    struct Variable
    {
        static constexpr char id = Id;
        static constexpr size_t order = Order;
    };
}

#endif      // _BZVARIABLE_HH_

// vim: set ft=cpp.doxygen:
