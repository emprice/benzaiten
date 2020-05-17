#ifndef _BZVARIABLE_HH_
#define _BZVARIABLE_HH_

#include "bzexpression.hh"

#include <cstddef>
#include <ostream>

namespace benzaiten
{
    template <char Id, size_t Order>
    struct Variable;

    template <typename Wrt, size_t Ord, typename Arg>
    struct VarDerivativeType
    {
        static constexpr bool equal = (Ord == 1) && (Wrt::id == Arg::id);

        using type = typename std::conditional<Ord == 0, Arg,
            typename std::conditional<equal, Constant, ZeroFn>::type>::type;
    };

    template <char Id, size_t Order = 0>
    struct Variable : public FnExpression<Variable<Id, Order>>
    {
        template <typename Wrt, size_t Ord>
        using deriv_type = VarDerivativeType<Wrt, Ord, Variable<Id, Order>>::type;

        static constexpr char id = Id;
        static constexpr size_t order = Order;

        friend std::ostream& operator<<(std::ostream& os, const Variable &var)
        {
            os << Id;
            return os;
        }

        template <auto Wrt, size_t Ord = 1>
        deriv_type<decltype(Wrt), Ord> derivative() const
        {
            if constexpr (Ord == 0) return *this;
            else if constexpr ((Ord == 1) && (decltype(Wrt)::id == Id)) return Constant(1);
            else return ZeroFn();
        }
    };
}

#endif      // _BZVARIABLE_HH_

// vim: set ft=cpp.doxygen:
