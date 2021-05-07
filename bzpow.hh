#ifndef _BZPOW_HH_
#define _BZPOW_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzsum.hh"
#include "bzprod.hh"
#include "bzlog.hh"

#include <cmath>
#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E1, typename E2>
    struct FnPow;

    template <typename Wrt, size_t Order, typename E1, typename E2>
    struct FnPowDerivativeType
    {
        static constexpr bool zero1 =
            std::is_same<typename E1::template deriv_type<Wrt, 1>, ZeroFn>::value;
        static constexpr bool zero2 =
            std::is_same<typename E2::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type1 = FnProd<FnProd<FnPow<E1, FnDiff<E2, Constant>>, E2>, typename E1::template deriv_type<Wrt, 1>>;
        using type2 = FnProd<FnProd<FnPow<E1, E2>, FnLog<E1>>, typename E2::template deriv_type<Wrt, 1>>;

        using type = typename std::conditional<zero1 && zero2, ZeroFn,
            typename std::conditional<zero1, typename type2::template deriv_type<Wrt, Order - 1>,
                typename std::conditional<zero2, typename type1::template deriv_type<Wrt, Order - 1>,
                    typename FnSum<type1, type2>::template deriv_type<Wrt, Order - 1>>::type>::type>::type;
    };

    template <typename Wrt, typename E1, typename E2>
    struct FnPowDerivativeType<Wrt, 0, E1, E2>
    {
        using type = FnPow<E1, E2>;
    };

    template <typename E1, typename E2>
    struct FnPow : public FnExpression<FnPow<E1, E2>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = typename FnPowDerivativeType<Wrt, Order, E1, E2>::type;

        template <typename Src, typename Dest>
        using replace_type = FnPow<typename E1::template replace_type<Src, Dest>,
            typename E2::template replace_type<Src, Dest>>;

        FnPow(const E1& fn1, const E2& fn2) : fn1(fn1), fn2(fn2) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (pow(fn1, fn2 - 1) * fn2 * fn1.template derivative<Wrt, 1>() +
                        pow(fn1, fn2) * log(fn1) * fn2.template derivative<Wrt, 1>()).template derivative<Wrt, Order - 1>();
            }
        }

        template <typename Target>
        FnPow<E1, E2>& substitute(double val)
        {
            fn1.template substitute<Target>(val);
            fn2.template substitute<Target>(val);

            return *this;
        }

        template <typename Src, typename Dest>
        replace_type<Src, Dest> replace(Dest dest)
        {
            auto newfn1 = fn1.template replace<Src, Dest>(dest);
            auto newfn2 = fn2.template replace<Src, Dest>(dest);

            return FnPow<decltype(newfn1), decltype(newfn2)>(newfn1, newfn2);
        }

        friend std::ostream& operator<<(std::ostream& os, const FnPow& pwr)
        {
            os << "(" << pwr.fn1 << " ^ " << pwr.fn2 << ")";
            return os;
        }

        double getValue() const { return std::pow(fn1.getValue(), fn2.getValue()); }

        void resetValue()
        {
            fn1.resetValue();
            fn2.resetValue();
        }

        private:
            E1 fn1;
            E2 fn2;
    };

    template <typename E1, typename E2>
    FnPow<E1, E2> pow(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnPow<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1, typename E2>
    FnPow<E1, E2> operator^(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnPow<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    FnPow<E1, Constant> pow(const FnExpression<E1>& fn1, double cnst)
    {
        return FnPow<E1, Constant>(static_cast<const E1&>(fn1), Constant(cnst));
    }

    template <typename E2>
    FnPow<Constant, E2> pow(double cnst, const FnExpression<E2>& fn2)
    {
        return FnPow<Constant, E2>(Constant(cnst), static_cast<const E2&>(fn2));
    }

    template <typename E1>
    FnPow<E1, Constant> operator^(const FnExpression<E1>& fn1, double cnst)
    {
        return FnPow<E1, Constant>(static_cast<const E1&>(fn1), Constant(cnst));
    }

    template <typename E2>
    FnPow<Constant, E2> operator^(double cnst, const FnExpression<E2>& fn2)
    {
        return FnPow<Constant, E2>(Constant(cnst), static_cast<const E2&>(fn2));
    }

    template <typename E>
    FnPow<E, Constant> sqrt(const FnExpression<E>& fn)
    {
        return FnPow<E, Constant>(static_cast<const E&>(fn), Constant(0.5));
    }

    template <typename E1>
    Constant pow(const FnExpression<E1>& fn1, const ZeroFn& zero)
    {
        return Constant(1);
    }

    template <typename E1>
    Constant operator^(const FnExpression<E1>& fn1, const ZeroFn& zero)
    {
        return Constant(1);
    }

    template <typename E2>
    ZeroFn pow(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return zero;
    }

    template <typename E2>
    ZeroFn operator^(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return zero;
    }
}

#endif      // _BZPOW_HH_

// vim: set ft=cpp.doxygen:
