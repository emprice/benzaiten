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
        using deriv_type = typename FnDerivativeType<Id, Wrt, Order, Args...>::type;

        template <typename Src, typename Dest>
        using replace_type =
            typename std::conditional<std::is_same<Src, Fn<Id, Args...>>::value, Dest, Src>::type;

        template <typename Wrt, size_t Order = 1>
        deriv_type<Wrt, Order> derivative() const
        {
            return deriv_type<Wrt, Order>();
        }

        template <typename Target>
        Fn<Id, Args...>& substitute(double val)
        {
            if constexpr (std::is_same<Target, Fn<Id, Args...>>::value)
            {
                value = val;
            }

            return *this;
        }

        template <typename Src, typename Dest>
        replace_type<Src, Dest> replace(Dest dest)
        {
            if constexpr (std::is_same<Src, Fn<Id, Args...>>::value)
            {
                return dest;
            }
            else
            {
                return *this;
            }
        }

        double getValue() const { return value; }

        friend std::ostream& operator<<(std::ostream& os, const Fn& fn)
        {
            os << Fn::id << "(";
            print_args<Args...>(os);
            os << ")";
            return os;
        }

        private:
            double value;

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

    template <char Id, typename... Args>
    Fn<Id, Args...> Function()
    {
        return Fn<Id, Args...>();
    }
}

#endif      // _BZFUNCTION_HH_

// vim: set ft=cpp.doxygen:
