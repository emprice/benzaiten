#ifndef _BZEXP_HH_
#define _BZEXP_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzquo.hh"

#include <cmath>
#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E>
    struct FnExp;

    template <typename Wrt, size_t Order, typename E>
    struct FnExpDerivativeType
    {
        static constexpr bool zero =
            std::is_same<typename E::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type = typename std::conditional<zero, ZeroFn,
            typename FnProd<FnExp<E>, typename E::template deriv_type<Wrt, 1>>::template deriv_type<Wrt, Order - 1>>::type;
    };

    template <typename Wrt, typename E>
    struct FnExpDerivativeType<Wrt, 0, E>
    {
        using type = FnExp<E>;
    };

    template <typename E>
    struct FnExp : public FnExpression<FnExp<E>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnExpDerivativeType<Wrt, Order, E>::type;

        template <typename Src, typename Dest>
        using replace_type = FnExp<typename E::template replace_type<Src, Dest>>;

        FnExp(const E& fn) : fn(fn) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (exp(fn) * fn.template derivative<Wrt, 1>()).template derivative<Wrt, Order - 1>();
            }
        }

        template <typename Target>
        FnExp<E>& substitute(double val)
        {
            fn.template substitute<Target>(val);

            return *this;
        }

        template <typename Src, typename Dest>
        replace_type<Src, Dest> replace(Dest dest)
        {
            auto newfn = fn.template replace<Src, Dest>(dest);
            return FnExp<decltype(newfn)>(newfn);
        }

        friend std::ostream& operator<<(std::ostream& os, const FnExp& ex)
        {
            os << "exp(" << ex.fn << ")";
            return os;
        }

        double getValue() const { return std::exp(fn.getValue()); }

        private:
            E fn;
    };

    template <typename E>
    FnExp<E> exp(const FnExpression<E>& fn)
    {
        return FnExp<E>(static_cast<const E&>(fn));
    }
}

#endif      // _BZEXP_HH_

// vim: set ft=cpp.doxygen:
