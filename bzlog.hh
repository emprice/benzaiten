#ifndef _BZLOG_HH_
#define _BZLOG_HH_

#include "bzexpression.hh"
#include "bzfunction.hh"
#include "bzquo.hh"

#include <cmath>
#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <typename E>
    struct FnLog;

    template <typename Wrt, size_t Order, typename E>
    struct FnLogDerivativeType
    {
        static constexpr bool zero =
            std::is_same<typename E::template deriv_type<Wrt, 1>, ZeroFn>::value;

        using type = typename std::conditional<zero, ZeroFn,
            typename FnQuo<typename E::template deriv_type<Wrt, 1>, E>::template deriv_type<Wrt, Order - 1>>::type;
    };

    template <typename Wrt, typename E>
    struct FnLogDerivativeType<Wrt, 0, E>
    {
        using type = FnLog<E>;
    };

    template <typename E>
    struct FnLog : public FnExpression<FnLog<E>>
    {
        template <typename Wrt, size_t Order>
        using deriv_type = typename FnLogDerivativeType<Wrt, Order, E>::type;

        template <typename Src, typename Dest>
        using replace_type = FnLog<typename E::template replace_type<Src, Dest>>;

        FnLog(const E& fn) : fn(fn) { }

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            if constexpr (Order == 0) return *this;
            else
            {
                return (fn.template derivative<Wrt, 1>() / fn).template derivative<Wrt, Order - 1>();
            }
        }

        template <typename Target>
        FnLog<E>& substitute(double val)
        {
            fn.template substitute<Target>(val);

            return *this;
        }

        template <typename Src, typename Dest>
        replace_type<Src, Dest> replace(Dest dest)
        {
            auto newfn = fn.template replace<Src, Dest>(dest);
            return FnLog<decltype(newfn)>(newfn);
        }

        friend std::ostream& operator<<(std::ostream& os, const FnLog& lg)
        {
            os << "log(" << lg.fn << ")";
            return os;
        }

        double getValue() const { return std::log(fn.getValue()); }

        void resetValue()
        {
            fn.resetValue();
        }

        private:
            E fn;
    };

    template <typename E>
    FnLog<E> log(const FnExpression<E>& fn)
    {
        return FnLog<E>(static_cast<const E&>(fn));
    }
}

#endif      // _BZLOG_HH_

// vim: set ft=cpp.doxygen:
