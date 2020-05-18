benzaiten
=========

Automatic differentiation of abstract functions in C++20

<div align="center">
<img src="/assets/bzlogo.svg" width="100%" />
</div>

# Overview

The use case for _benzaiten_ is relatively straightforward. Suppose you are solving a nonlinear
diffusion PDE numerically. You will have derivatives of your unknown function and some functions
of your independent variable(s). You need to evaluate an expression like this, and you have your
function sampled at discrete points.

For a small expression, it suffices to use _Mathematica_ or SymPy to expand out all the terms.
But what happens when your expression is too complex, or it has too many terms to easily
transcribe from another source?

_benzaiten_ strives to solve this problem using pure C++ and metaprogramming. You can easily
program your expression in _benzaiten_'s formalism, and the compiler will determine the tree
structure of your expression. Substituting values has also been optimized to use metaprogramming
techniques, so that you can easily input values for your independent variables and unknown
function derivatives, which might be found by finite differences or other techniques.

## How *not* to take derivatives

Returning to the nonlinear diffusion example, suppose you decide to take your derivatives in
what I'll call the naive way: You take a first derivative of some function _f_, multiply
by the nonlinear term, and then take a first derivative of that result. Both operations are
second order accurate, so how bad could it be?

The answer is, compared to _benzaiten_'s way, fairly bad. We can look at the average error
of computing a nonlinear-diffusion-type term, where _f_ has been set to _cos_(x) and
the nonlinear term is _sin_^2(x) (see [bzbench.cc](/bzbench.cc)). The error of the successive
derivative method is much higher than the error of computing with _benzaiten_, even when
both use second order accurate finite differences, confirmed by a logarithmic plot of 
error as a function of sample points.

<div align="center">
<img src="/assets/absolute_error.svg" width="40%" style="float: left;" />
<img src="/assets/error_scaling.svg" width="40%" style="float: left;" />
</div>

# Is it fast?

The old version of _benzaiten_ took a major performance hit over a baseline case; it was slower
by about a factor of ten. This version, however, was actually faster in the benchmarking test
than the baseline. Whether _benzaiten_ is fast enough for your particular application may
be problem-dependent, but the results so far look promising.

# How do I use it?

To get started, take a look at [bzexplore.cc](/bzexplore.cc), which serves as test suite and
examples for _benzaiten_. [bzbench.cc](/bzbench.cc), which is used for benchmarking, may
also be useful.

# Why the name?

[Benzaiten](https://en.wikipedia.org/wiki/Benzaiten) is the Japanese goddess of "everything
that flows," including water. This code was originally intended to be part of a larger 
hydrodynamics code, so the name seemed appropriate.
