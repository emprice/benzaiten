#ifndef _BZPROD_HH_
#define _BZPROD_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzsum.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E1, typename E2>
    struct FnProd;

    template <typename Wrt, size_t Order, typename E1, typename E2>
    struct FnProdDerivativeType
    {
        static constexpr bool zero1 = std::is_same<E1, ZeroFn>::value;
        static constexpr bool zero2 = std::is_same<E2, ZeroFn>::value;

        static constexpr bool dzero1 =
            std::is_same<typename E1::template deriv_type<Wrt, 1>, ZeroFn>::value;
        static constexpr bool dzero2 =
            std::is_same<typename E2::template deriv_type<Wrt, 1>, ZeroFn>::value;

        static constexpr bool zero = (zero1 || zero2) ||
            ((dzero1 && zero2) || (dzero2 && zero1)) || (dzero1 && dzero2);

        using type1 = FnProd<E1, typename E2::template deriv_type<Wrt, 1>>;
        using type2 = FnProd<typename E1::template deriv_type<Wrt, 1>, E2>;

        using type = typename std::conditional<zero, ZeroFn,
            typename std::conditional<dzero1, typename type1::template deriv_type<Wrt, Order - 1>,
                typename std::conditional<dzero2, typename type2::template deriv_type<Wrt, Order - 1>,
                    typename FnSum<type2, type1>::template deriv_type<Wrt, Order - 1>>::type>::type>::type;
    };

    template <typename Wrt, typename E1, typename E2>
    struct FnProdDerivativeType<Wrt, 0, E1, E2>
    {
        using type = FnProd<E1, E2>;
    };

    template <typename E1, typename E2>
    struct FnProd : public FnExpression<FnProd<E1, E2>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnProdDerivativeType<Wrt, Order, E1, E2>::type;

        FnProd(const E1& fn1, const E2& fn2) : fn1(fn1), fn2(fn2) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (fn1.template derivative<Wrt, 1>() * fn2 +
                        fn1 * fn2.template derivative<Wrt, 1>()).template derivative<Wrt, Order - 1>();
            }
        }

        template <typename Target>
        FnProd<E1, E2>& substitute(double val)
        {
            fn1.template substitute<Target>(val);
            fn2.template substitute<Target>(val);

            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const FnProd& prod)
        {
            os << "(" << prod.fn1 << " * " << prod.fn2 << ")";
            return os;
        }

        double getValue() const { return fn1.getValue() * fn2.getValue(); }

        private:
            E1 fn1;
            E2 fn2;
    };

    template <typename E1, typename E2>
    FnProd<E1, E2> operator*(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnProd<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    FnProd<E1, Constant> operator*(const FnExpression<E1>& fn1, double cnst)
    {
        return FnProd<E1, Constant>(static_cast<const E1&>(fn1),
            Constant(cnst));
    }

    template <typename E2>
    FnProd<Constant, E2> operator*(double cnst, const FnExpression<E2>& fn2)
    {
        return FnProd<Constant, E2>(Constant(cnst),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    ZeroFn operator*(const FnExpression<E1>& fn1, const ZeroFn& zero)
    {
        return zero;
    }

    template <typename E2>
    ZeroFn operator*(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return zero;
    }

    ZeroFn operator*(const ZeroFn& zero1, const ZeroFn& zero2)
    {
        return zero1;
    }
}

#endif      // _BZPROD_HH_

// vim: set ft=cpp.doxygen:
