#ifndef _BZVARIABLE_HH_
#define _BZVARIABLE_HH_

#include "bzexpression.hh"

#include <cstddef>
#include <ostream>
#include <sstream>
#include <optional>

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
        using deriv_type = typename VarDerivativeType<Wrt, Ord, Variable<Id, Order>>::type;

        template <typename Src, typename Dest>
        using replace_type = Variable<Id, Order>;

        static constexpr char id = Id;
        static constexpr size_t order = Order;

        template <typename Wrt, size_t Ord = 1>
        deriv_type<Wrt, Ord> derivative() const
        {
            if constexpr (Ord == 0) return *this;
            else if constexpr ((Ord == 1) && (Wrt::id == Id)) return Constant(1);
            else return ZeroFn();
        }

        template <typename Target>
        Variable<Id, Order>& substitute(double val)
        {
            if constexpr (std::is_same<Target, Variable<Id, Order>>::value)
            {
                value = val;
            }

            return *this;
        }

        template <typename Src, typename Dest>
        replace_type<Src, Dest> replace(Dest dest)
        {
            return *this;
        }

        double getValue() const
        {
#ifdef NDEBUG
            return value.value_or(0.);
#else
            if (!value.has_value())
            {
                std::ostringstream oss;
                oss << "forgot to set value: " << typeid(this).name();
                throw std::runtime_error(oss.str());
            }

            return *value;
#endif
        }

        void resetValue()
        {
            value.reset();
        }

        friend std::ostream& operator<<(std::ostream& os, const Variable &var)
        {
            os << Id;
            return os;
        }

        private:
            std::optional<double> value;
    };
}

#endif      // _BZVARIABLE_HH_

// vim: set ft=cpp.doxygen:
