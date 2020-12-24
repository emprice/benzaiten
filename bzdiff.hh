#ifndef _BZDIFF_HH_
#define _BZDIFF_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzneg.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E1, typename E2>
    struct FnDiff;

    template <typename Wrt, size_t Order, typename E1, typename E2>
    struct FnDiffDerivativeType
    {
        static constexpr bool zero1 =
            std::is_same<typename E1::template deriv_type<Wrt, Order>, ZeroFn>::value;
        static constexpr bool zero2 =
            std::is_same<typename E2::template deriv_type<Wrt, Order>, ZeroFn>::value;

        using type = typename std::conditional<zero1,
            FnNeg<typename E2::template deriv_type<Wrt, Order>>,
            typename std::conditional<zero2,
            typename E1::template deriv_type<Wrt, Order>,
                FnDiff<typename E1::template deriv_type<Wrt, Order>,
                    typename E2::template deriv_type<Wrt, Order>>>::type>::type;
    };

    template <typename E1, typename E2>
    struct FnDiff : public FnExpression<FnDiff<E1, E2>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = typename FnDiffDerivativeType<Wrt, Order, E1, E2>::type;

        template <typename Src, typename Dest>
        using replace_type = FnDiff<typename E1::template replace_type<Src, Dest>,
            typename E2::template replace_type<Src, Dest>>;

        FnDiff(const E1& fn1, const E2& fn2) : fn1(fn1), fn2(fn2) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            return fn1.template derivative<Wrt, Order>() -
                   fn2.template derivative<Wrt, Order>();
        }

        template <typename Target>
        FnDiff<E1, E2>& substitute(double val)
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

            return FnDiff<decltype(newfn1), decltype(newfn2)>(newfn1, newfn2);
        }

        friend std::ostream& operator<<(std::ostream& os, const FnDiff& diff)
        {
            os << "(" << diff.fn1 << " - " << diff.fn2 << ")";
            return os;
        }

        double getValue() const { return fn1.getValue() - fn2.getValue(); }

        private:
            E1 fn1;
            E2 fn2;
    };

    template <typename E1, typename E2>
    FnDiff<E1, E2> operator-(const FnExpression<E1>& fn1,
        const FnExpression<E2>& fn2)
    {
        return FnDiff<E1, E2>(static_cast<const E1&>(fn1),
            static_cast<const E2&>(fn2));
    }

    template <typename E1>
    E1 operator-(const FnExpression<E1>& fn1, const ZeroFn& zero)
    {
        return static_cast<const E1&>(fn1);
    }

    template <typename E2>
    FnNeg<E2> operator-(const ZeroFn& zero, const FnExpression<E2>& fn2)
    {
        return -static_cast<const E2&>(fn2);
    }

    ZeroFn operator-(const ZeroFn& zero1, const ZeroFn& zero2)
    {
        return zero1;
    }

    template <typename E1>
    FnDiff<E1, Constant> operator-(const FnExpression<E1>& fn1, double cnst)
    {
        return FnDiff<E1, Constant>(static_cast<const E1&>(fn1), Constant(cnst));
    }

    template <typename E2>
    FnDiff<Constant, E2> operator-(double cnst, const FnExpression<E2>& fn2)
    {
        return FnDiff<Constant, E2>(Constant(cnst), static_cast<const E2&>(fn2));
    }
}

#endif      // _BZDIFF_HH_

// vim: set ft=cpp.doxygen:
