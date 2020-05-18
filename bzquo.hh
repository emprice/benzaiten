#ifndef _BZQUO_HH_
#define _BZQUO_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzdiff.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E1, typename E2>
    struct FnQuo;

    template <typename Wrt, size_t Order, typename E1, typename E2>
    struct FnQuoDerivativeType
    {
        static constexpr bool zero1 =
            std::is_same<typename E1::template deriv_type<Wrt, 1>, ZeroFn>::value;
        static constexpr bool zero2 =
            std::is_same<typename E2::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type = typename std::conditional<zero1,
            typename FnNeg<FnQuo<FnProd<E1, typename E2::template deriv_type<Wrt, 1>>, FnProd<E2, E2>>>::template deriv_type<Wrt, Order - 1>,
            typename std::conditional<zero2,
                typename FnQuo<typename E1::template deriv_type<Wrt, 1>, E2>::template deriv_type<Wrt, Order - 1>,
                typename FnDiff<FnQuo<typename E1::template deriv_type<Wrt, 1>, E2>,
                    FnQuo<FnProd<E1, typename E2::template deriv_type<Wrt, 1>>, FnProd<E2, E2>>>::template deriv_type<Wrt, Order - 1>>::type>::type;
    };

    template <typename Wrt, typename E1, typename E2>
    struct FnQuoDerivativeType<Wrt, 0, E1, E2>
    {
        using type = FnQuo<E1, E2>;
    };

    template <typename E1, typename E2>
    struct FnQuo : public FnExpression<FnQuo<E1, E2>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnQuoDerivativeType<Wrt, Order, E1, E2>::type;

        FnQuo(const E1& fn1, const E2& fn2) : fn1(fn1), fn2(fn2) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (fn1.template derivative<Wrt, 1>() / fn2 -
                        fn1 * fn2.template derivative<Wrt, 1>() / (fn2 * fn2)).template derivative<Wrt, Order - 1>();
            }
        }

        template <typename Target>
        FnQuo<E1, E2>& substitute(double val)
        {
            fn1.template substitute<Target>(val);
            fn2.template substitute<Target>(val);

            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const FnQuo& quo)
        {
            os << "(" << quo.fn1 << " / " << quo.fn2 << ")";
            return os;
        }

        double getValue() const { return fn1.getValue() / fn2.getValue(); }

        private:
            E1 fn1;
            E2 fn2;
    };

    template <typename E1, typename E2>
    FnQuo<E1, E2> operator/(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnQuo<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    FnQuo<E1, Constant> operator/(const FnExpression<E1>& fn1, double cnst)
    {
        return FnQuo<E1, Constant>(static_cast<const E1&>(fn1), Constant(cnst));
    }

    template <typename E2>
    FnQuo<Constant, E2> operator/(double cnst, const FnExpression<E2>& fn2)
    {
        return FnQuo<Constant, E2>(Constant(cnst), static_cast<const E2&>(fn2));
    }

    template <typename E2>
    ZeroFn operator/(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return zero;
    }
}

#endif      // _BZQUO_HH_

// vim: set ft=cpp.doxygen:
