/**
 * @file bzexplore.cc
 * @brief Lots of examples and tests for benzaiten
 */

#include "benzaiten.hh"

#include <iostream>
#include <type_traits>

using namespace benzaiten;

int main(int argc, char **argv)
{
    auto x = Variable<'x'>();
    auto y = Variable<'y'>();
    auto z = Variable<'z'>();

    auto f = Function<'f', decltype(x), decltype(y)>();
    auto g = Function<'g', decltype(z)>();
    auto h = Function<'h', decltype(x)>();

    auto df = f.derivative<decltype(x), 2>();
    auto dg = g.derivative<decltype(x)>();
    auto dh = h.derivative<decltype(x)>();

    std::cout << df << std::endl;
    std::cout << dg << std::endl;
    std::cout << dh << std::endl << std::endl;

    auto sum = f + g;

    std::cout << sum << std::endl;
    std::cout << sum.derivative<decltype(x)>() << std::endl;
    std::cout << sum.derivative<decltype(z)>() << std::endl;
    std::cout << sum.derivative<decltype(x)>().derivative<decltype(y)>() << std::endl;
    std::cout << sum.derivative<decltype(x), 2>() << std::endl << std::endl;

    auto sum2 = f + 1;

    std::cout << sum2 << std::endl;
    std::cout << sum2.derivative<decltype(x)>() << std::endl;
    std::cout << sum2.derivative<decltype(z)>() << std::endl << std::endl;

    auto diff = f - g;

    std::cout << diff << std::endl;
    std::cout << diff.derivative<decltype(x)>() << std::endl;
    std::cout << diff.derivative<decltype(z)>() << std::endl;
    std::cout << diff.derivative<decltype(x)>().derivative<decltype(y)>() << std::endl;
    std::cout << diff.derivative<decltype(x), 2>() << std::endl << std::endl;

    auto neg = -f;

    std::cout << neg << std::endl;
    std::cout << neg.derivative<decltype(x)>() << std::endl;
    std::cout << neg.derivative<decltype(x), 2>() << std::endl << std::endl;

    auto prod = f * g;

    std::cout << prod << std::endl;
    std::cout << prod.derivative<decltype(x), 2>() << std::endl;
    std::cout << prod.derivative<decltype(z), 2>() << std::endl << std::endl;

    auto prod2 = f * h;

    std::cout << prod2 << std::endl;
    std::cout << prod2.derivative<decltype(x), 2>() << std::endl << std::endl;

    auto quo = f / g;

    std::cout << quo << std::endl;
    std::cout << quo.derivative<decltype(x)>() << std::endl;
    std::cout << quo.derivative<decltype(z)>() << std::endl << std::endl;

    auto quo2 = f / h;

    std::cout << quo2 << std::endl;
    std::cout << quo2.derivative<decltype(x)>() << std::endl << std::endl;

    auto pwr = f ^ g;

    std::cout << pwr << std::endl;
    std::cout << pwr.derivative<decltype(x)>() << std::endl;
    std::cout << pwr.derivative<decltype(z)>() << std::endl << std::endl;

    auto pwr2 = g ^ f;

    std::cout << pwr2 << std::endl;
    std::cout << pwr2.derivative<decltype(x)>() << std::endl;
    std::cout << pwr2.derivative<decltype(z)>() << std::endl << std::endl;

    auto pwr3 = f ^ h;

    std::cout << pwr3 << std::endl;
    std::cout << pwr3.derivative<decltype(x)>() << std::endl;
    std::cout << pwr3.derivative<decltype(z)>() << std::endl << std::endl;

    auto pwr4 = f ^ 2;

    std::cout << pwr4 << std::endl;
    std::cout << pwr4.derivative<decltype(x)>() << std::endl;

    auto lg = log(f);

    std::cout << lg << std::endl;
    std::cout << lg.derivative<decltype(x)>() << std::endl;
    std::cout << lg.derivative<decltype(z)>() << std::endl << std::endl;

    auto ex = exp(f);

    std::cout << ex << std::endl;
    std::cout << ex.derivative<decltype(x)>() << std::endl;
    std::cout << ex.derivative<decltype(x), 2>() << std::endl;
    std::cout << ex.derivative<decltype(z)>() << std::endl << std::endl;

    auto trig1 = sin(f);

    std::cout << trig1 << std::endl;
    std::cout << trig1.derivative<decltype(x)>() << std::endl;
    std::cout << trig1.derivative<decltype(y)>() << std::endl << std::endl;

    auto trig2 = cos(f);

    std::cout << trig2 << std::endl;
    std::cout << trig2.derivative<decltype(x)>() << std::endl;
    std::cout << trig2.derivative<decltype(y)>() << std::endl << std::endl;

    auto expr = x^2;

    std::cout << expr << std::endl;
    std::cout << expr.derivative<decltype(x)>() << std::endl;
    std::cout << expr.derivative<decltype(x), 2>() << std::endl;
    std::cout << expr.derivative<decltype(y)>() << std::endl << std::endl;

    auto expr2 = ((sin(x)^2) * cos(x).derivative<decltype(x)>()).derivative<decltype(x)>();

    std::cout << expr2 << std::endl << std::endl;
    std::cout << expr2.substitute<decltype(x)>(2).getValue() << std::endl;

    return 0;
}

// vim: set ft=cpp.doxygen:
