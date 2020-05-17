#ifndef _BZSUM_HH_
#define _BZSUM_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E1, typename E2>
    struct FnSum;

    template <typename Wrt, size_t Order, typename E1, typename E2>
    struct FnSumDerivativeType
    {
        static constexpr bool zero1 =
            std::is_same<typename E1::template deriv_type<Wrt, Order>, ZeroFn>::value;
        static constexpr bool zero2 =
            std::is_same<typename E2::template deriv_type<Wrt, Order>, ZeroFn>::value;

        using type = std::conditional<zero1,
            typename E2::template deriv_type<Wrt, Order>,
            typename std::conditional<zero2,
                typename E1::template deriv_type<Wrt, Order>,
                FnSum<typename E1::template deriv_type<Wrt, Order>,
                    typename E2::template deriv_type<Wrt, Order>>>::type>::type;
    };

    template <typename Wrt, typename E1, typename E2>
    struct FnSumDerivativeType<Wrt, 0, E1, E2>
    {
        using type = FnSum<E1, E2>;
    };

    template <typename E1, typename E2>
    struct FnSum : public FnExpression<FnSum<E1, E2>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnSumDerivativeType<Wrt, Order, E1, E2>::type;

        FnSum(const E1 fn1, const E2 &fn2) : fn1(fn1), fn2(fn2) { }

        template <auto Wrt, size_t Order = 1>
        deriv_type<decltype(Wrt), Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else return fn1.template derivative<Wrt, Order>() +
                        fn2.template derivative<Wrt, Order>();
        }

        friend std::ostream& operator<<(std::ostream& os, const FnSum& sum)
        {
            os << "(" << sum.fn1 << " + " << sum.fn2 << ")";
            return os;
        }

        double getValue() const { return fn1.getValue() + fn2.getValue(); }

        private:
            E1 fn1;
            E2 fn2;
    };

    template <typename E1, typename E2>
    FnSum<E1, E2> operator+(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnSum<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    FnSum<E1, Constant> operator+(const FnExpression<E1>& fn1, double cnst)
    {
        return FnSum<E1, Constant>(static_cast<const E1&>(fn1), Constant(cnst));
    }

    template <typename E2>
    FnSum<Constant, E2> operator+(double cnst, const FnExpression<E2>& fn2)
    {
        return FnSum<Constant, E2>(Constant(cnst), static_cast<const E2&>(fn2));
    }

    template <typename E1>
    E1 operator+(const FnExpression<E1>& fn1, const ZeroFn& zero)
    {
        return static_cast<const E1&>(fn1);
    }

    template <typename E2>
    E2 operator+(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return static_cast<const E2&>(fn2);
    }

    ZeroFn operator+(const ZeroFn& zero1, const ZeroFn& zero2)
    {
        return zero1;
    }
}

#endif      // _BZSUM_HH_

// vim: set ft=cpp.doxygen:
