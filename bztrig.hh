#ifndef _BZTRIG_HH_
#define _BZTRIG_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzneg.hh"
#include "bzprod.hh"
#include "bzpow.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E>
    struct FnSin;

    template <typename E>
    struct FnCos;

    template <typename Wrt, size_t Order, typename E>
    struct FnSinDerivativeType
    {
        static constexpr bool zero =
            std::is_same<typename E::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type = typename std::conditional<zero, ZeroFn,
            typename FnProd<FnCos<E>, typename E::template deriv_type<Wrt, 1>>::template deriv_type<Wrt, Order - 1>>::type;
    };

    template <typename Wrt, size_t Order, typename E>
    struct FnCosDerivativeType
    {
        static constexpr bool zero =
            std::is_same<typename E::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type = typename std::conditional<zero, ZeroFn,
            typename FnProd<FnNeg<FnSin<E>>, typename E::template deriv_type<Wrt, 1>>::template deriv_type<Wrt, Order - 1>>::type;
    };

    template <typename Wrt, typename E>
    struct FnSinDerivativeType<Wrt, 0, E>
    {
        using type = FnSin<E>;
    };

    template <typename Wrt, typename E>
    struct FnCosDerivativeType<Wrt, 0, E>
    {
        using type = FnCos<E>;
    };

    template <typename E>
    struct FnSin : public FnExpression<FnSin<E>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnSinDerivativeType<Wrt, Order, E>::type;

        FnSin(const E& fn) : fn(fn) { }

        template <auto Wrt, size_t Order = 1>
        deriv_type<decltype(Wrt), Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (cos(fn) * fn.template derivative<Wrt, 1>()).template derivative<Wrt, Order - 1>();
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const FnSin& sn)
        {
            os << "sin(" << sn.fn << ")";
            return os;
        }

        double getValue() const { return std::sin(fn.getValue()); }

        private:
            E fn;
    };

    template <typename E>
    struct FnCos : public FnExpression<FnCos<E>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = FnCosDerivativeType<Wrt, Order, E>::type;

        FnCos(const E& fn) : fn(fn) { }

        template <auto Wrt, size_t Order = 1>
        deriv_type<decltype(Wrt), Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (-sin(fn) * fn.template derivative<Wrt, 1>()).template derivative<Wrt, Order - 1>();
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const FnCos& cs)
        {
            os << "cos(" << cs.fn << ")";
            return os;
        }

        double getValue() const { return std::cos(fn.getValue()); }

        private:
            E fn;
    };

    template <typename E>
    FnSin<E> sin(const FnExpression<E>& fn)
    {
        return FnSin<E>(static_cast<const E&>(fn));
    }

    template <typename E>
    FnCos<E> cos(const FnExpression<E>& fn)
    {
        return FnCos<E>(static_cast<const E&>(fn));
    }
}

#endif      // _BZTRIG_HH_

// vim: set ft=cpp.doxygen:
