#ifndef _BZEXPRESSION_HH_
#define _BZEXPRESSION_HH_

#include <ostream>

namespace benzaiten
{
    template <typename E>
    struct FnExpression
    {
    };

    struct ZeroFn : public FnExpression<ZeroFn>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = ZeroFn;

        double getValue() const { return 0; }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            return *this;
        }

        template <typename Target>
        ZeroFn& substitute(double val) { return *this; }

        friend std::ostream& operator<<(std::ostream& os, const ZeroFn& fn)
        {
            os << "0";
            return os;
        }
    };

    struct Constant : public FnExpression<Constant>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = ZeroFn;

        Constant(double value) : value(value) { }

        Constant(const Constant& other) : value(other.value) { }

        Constant(Constant&& other)
        {
            value = other.value;
            other.value = 0;
        }

        double getValue() const { return value; }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            return ZeroFn();
        }

        template <typename Target>
        Constant& substitute(double val) { return *this; }

        friend std::ostream& operator<<(std::ostream& os, const Constant& cnst)
        {
            os << cnst.value;
            return os;
        }

        private:
            double value;
    };
}

#endif      // _BZEXPRESSION_HH_

// vim: set ft=cpp.doxygen:
