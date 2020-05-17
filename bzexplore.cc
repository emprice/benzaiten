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

    auto f = Function<'f', x, y>();
    auto g = Function<'g', z>();
    auto h = Function<'h', x>();

    auto df = f.derivative<x, 2>();
    auto dg = g.derivative<x>();
    auto dh = h.derivative<x>();

    std::cout << df << std::endl;
    std::cout << dg << std::endl;
    std::cout << dh << std::endl << std::endl;

    auto sum = f + g;

    std::cout << sum << std::endl;
    std::cout << sum.derivative<x>() << std::endl;
    std::cout << sum.derivative<z>() << std::endl;
    std::cout << sum.derivative<x>().derivative<y>() << std::endl;
    std::cout << sum.derivative<x, 2>() << std::endl << std::endl;

    auto sum2 = f + 1;

    std::cout << sum2 << std::endl;
    std::cout << sum2.derivative<x>() << std::endl;
    std::cout << sum2.derivative<z>() << std::endl << std::endl;

    auto diff = f - g;

    std::cout << diff << std::endl;
    std::cout << diff.derivative<x>() << std::endl;
    std::cout << diff.derivative<z>() << std::endl;
    std::cout << diff.derivative<x>().derivative<y>() << std::endl;
    std::cout << diff.derivative<x, 2>() << std::endl << std::endl;

    auto neg = -f;

    std::cout << neg << std::endl;
    std::cout << neg.derivative<x>() << std::endl;
    std::cout << neg.derivative<x, 2>() << std::endl << std::endl;

    auto prod = f * g;

    std::cout << prod << std::endl;
    std::cout << prod.derivative<x, 2>() << std::endl;
    std::cout << prod.derivative<z, 2>() << std::endl << std::endl;

    auto prod2 = f * h;

    std::cout << prod2 << std::endl;
    std::cout << prod2.derivative<x, 2>() << std::endl << std::endl;

    auto quo = f / g;

    std::cout << quo << std::endl;
    std::cout << quo.derivative<x>() << std::endl;
    std::cout << quo.derivative<z>() << std::endl << std::endl;

    auto quo2 = f / h;

    std::cout << quo2 << std::endl;
    std::cout << quo2.derivative<x>() << std::endl << std::endl;

    auto pwr = f ^ g;

    std::cout << pwr << std::endl;
    std::cout << pwr.derivative<x>() << std::endl;
    std::cout << pwr.derivative<z>() << std::endl << std::endl;

    auto pwr2 = g ^ f;

    std::cout << pwr2 << std::endl;
    std::cout << pwr2.derivative<x>() << std::endl;
    std::cout << pwr2.derivative<z>() << std::endl << std::endl;

    auto pwr3 = f ^ h;

    std::cout << pwr3 << std::endl;
    std::cout << pwr3.derivative<x>() << std::endl;
    std::cout << pwr3.derivative<z>() << std::endl << std::endl;

    auto pwr4 = f ^ 2;

    std::cout << pwr4 << std::endl;
    std::cout << pwr4.derivative<x>() << std::endl;

    auto lg = log(f);

    std::cout << lg << std::endl;
    std::cout << lg.derivative<x>() << std::endl;
    std::cout << lg.derivative<z>() << std::endl << std::endl;

    auto ex = exp(f);

    std::cout << ex << std::endl;
    std::cout << ex.derivative<x>() << std::endl;
    std::cout << ex.derivative<x, 2>() << std::endl;
    std::cout << ex.derivative<z>() << std::endl << std::endl;

    auto trig1 = sin(f);

    std::cout << trig1 << std::endl;
    std::cout << trig1.derivative<x>() << std::endl;
    std::cout << trig1.derivative<y>() << std::endl << std::endl;

    auto trig2 = cos(f);

    std::cout << trig2 << std::endl;
    std::cout << trig2.derivative<x>() << std::endl;
    std::cout << trig2.derivative<y>() << std::endl << std::endl;

    auto expr = x^2;

    std::cout << expr << std::endl;
    std::cout << expr.derivative<x>() << std::endl;
    std::cout << expr.derivative<x, 2>() << std::endl;
    std::cout << expr.derivative<y>() << std::endl << std::endl;

//    auto expr2 = ((sin(x)^2) * cos(x).derivative<x>()).derivative<x>();
    auto expr2 = cos(x).derivative<x>();
    auto expr3 = expr2.derivative<x>();

    std::cout << expr2 << std::endl;
    std::cout << expr3 << std::endl;

    return 0;
}

// vim: set ft=cpp.doxygen:
