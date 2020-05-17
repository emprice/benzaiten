#ifndef _BZFUNCTION_HH_
#define _BZFUNCTION_HH_

#include "bzexpression.hh"

#include <ostream>
#include <type_traits>

namespace benzaiten
{
    template <char Id, typename... Args>
    struct Fn;

    template <typename Wrt, size_t Order, typename Arg>
    struct IncrementDerivative
    {
        using type = typename std::conditional<Wrt::id == Arg::id,
            Variable<Arg::id, Arg::order + Order>, Arg>::type;
    };

    template <char Id, typename Wrt, size_t Order, typename First, typename... Rest>
    struct FnDerivativeType
    {
        static constexpr bool equal = (Wrt::id == First::id) ||
            FnDerivativeType<Id, Wrt, Order, Rest...>::equal;;

        using type = typename std::conditional<equal,
            Fn<Id, typename IncrementDerivative<Wrt, Order, First>::type,
                typename IncrementDerivative<Wrt, Order, Rest>::type...>,
            ZeroFn>::type;
    };

    template <char Id, typename Wrt, size_t Order, typename Only>
    struct FnDerivativeType<Id, Wrt, Order, Only>
    {
        static constexpr bool equal = (Wrt::id == Only::id);

        using type = typename std::conditional<equal,
            Fn<Id, typename IncrementDerivative<Wrt, Order, Only>::type>,
                ZeroFn>::type;
    };

    template <char Id, typename Wrt, typename First, typename... Rest>
    struct FnDerivativeType<Id, Wrt, 0, First, Rest...>
    {
        using type = Fn<Id, First, Rest...>;
    };

    template <char Id, typename... Args>
    struct Fn : public FnExpression<Fn<Id, Args...>>
    {
        static constexpr char id = Id;

        template <typename Wrt, size_t Order>
        using deriv_type = FnDerivativeType<Id, Wrt, Order, Args...>::type;

        template <auto Wrt, size_t Order = 1>
        deriv_type<decltype(Wrt), Order> derivative() const
        {
            return deriv_type<decltype(Wrt), Order>();
        }

        friend std::ostream& operator<<(std::ostream& os, const Fn& fn)
        {
            os << Fn::id << "(";
            print_args<Args...>(os);
            os << ")";
            return os;
        }

        private:
            template <typename First, typename... Rest>
            static void print_args(std::ostream& os)
            {
                os << "{" << First::id << ", " << First::order << "}";
                if constexpr (sizeof...(Rest) > 0)
                {
                    os << ", ";
                    print_args<Rest...>(os);
                }
            }
    };

    template <char Id, auto... Args>
    Fn<Id, decltype(Args)...> Function()
    {
        return Fn<Id, decltype(Args)...>();
    }
}

#endif      // _BZFUNCTION_HH_

// vim: set ft=cpp.doxygen:
