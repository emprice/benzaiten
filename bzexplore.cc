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
    std::cout << quo2.derivative<x>() << std::endl;

    return 0;
}

// vim: set ft=cpp.doxygen:
