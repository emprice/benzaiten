#ifndef _BZNEG_HH_
#define _BZNEG_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E>
    struct FnNeg;

    template <typename Wrt, size_t Order, typename E>
    struct FnNegDerivativeType
    {
        static constexpr bool zero =
            std::is_same<typename E::template deriv_type<Wrt, Order>, ZeroFn>::value;

        using type = std::conditional<zero,
            ZeroFn, FnNeg<typename E::template deriv_type<Wrt, Order>>>::type;
    };

    template <typename E>
    struct FnNeg : public FnExpression<FnNeg<E>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnNegDerivativeType<Wrt, Order, E>::type;

        FnNeg(E const& fn) : fn(fn) { }

        template <auto Wrt, size_t Order = 1>
        deriv_type<decltype(Wrt), Order> derivative() const
        {
            return -fn.template derivative<Wrt, Order>();
        }

        friend std::ostream& operator<<(std::ostream& os, const FnNeg& sum)
        {
            os << "(-(" << sum.fn << "))";
            return os;
        }

        double getValue() const { return -fn.getValue(); }

        private:
            E fn;
    };

    template <typename E>
    FnNeg<E> operator-(FnExpression<E> const& fn)
    {
        return FnNeg<E>(static_cast<const E&>(fn));
    }

    ZeroFn operator-(ZeroFn const& zero)
    {
        return zero;
    }
}

#endif      // _BZNEG_HH_

// vim: set ft=cpp.doxygen:
