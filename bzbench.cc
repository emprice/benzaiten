#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include "benzaiten.hh"

using namespace benzaiten;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "error: expected 1 argument, got " << argc - 1 << std::endl;
        return -1;
    }

    auto x = Variable<'x'>();
    auto f = Function<'f', decltype(x)>();
    auto df = f.derivative<decltype(x)>();
    auto d2f = f.derivative<decltype(x), 2>();

    size_t N = atoi(argv[1]);
    double xmin = 0., xmax = 2. * M_PI;
    double h = (xmax - xmin) / (N - 1);

    double *xx = new double[N];
    double *ff = new double[N];
    double *d1 = new double[N];
    double *d2 = new double[N];

    for (size_t i = 0; i < N; ++i)
    {
        xx[i] = xmin + h * i;
        ff[i] = cos(xx[i]);
    }

    // first way
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();
    for (size_t i = 2; i < N - 2; ++i)
    {
        double fim2 = ff[i-2];
        double fi   = ff[i];
        double fip2 = ff[i+2];

        double xim1 = xx[i-1];
        double xi   = xx[i];
        double xip1 = xx[i+1];

        double dfdxim1 = sin(xim1) * sin(xim1) * (-fim2 + fi)   / (2 * h);
        double dfdxip1 = sin(xip1) * sin(xip1) * (-fi   + fip2) / (2 * h);

        d1[i] = (-dfdxim1 + dfdxip1) / (2 * h);
    }
    std::chrono::high_resolution_clock::time_point t2 =
        std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> span1 =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    // second way
    auto expr = ((sin(x) ^ 2) * f.derivative<decltype(x)>()).derivative<decltype(x)>();

    std::chrono::high_resolution_clock::time_point t3 =
        std::chrono::high_resolution_clock::now();
    for (size_t i = 2; i < N - 2; ++i)
    {
        double fim1 = ff[i-1];
        double fi   = ff[i];
        double fip1 = ff[i+1];

        double xi   = xx[i];

        double dfdx   = (-fim1 + fip1) / (2 * h);
        double d2fdx2 = (fim1 - 2 * fi + fip1) / (h * h);

        expr.substitute<decltype(x)>(xi);
        expr.substitute<decltype(f)>(fi);
        expr.substitute<decltype(df)>(dfdx);
        expr.substitute<decltype(d2f)>(d2fdx2);

        d2[i] = expr.getValue();
    }
    std::chrono::high_resolution_clock::time_point t4 =
        std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> span2 =
        std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);

    std::ofstream ofs;
    std::ostringstream name;
    name << "bzbench_" << N << ".dat";
    ofs.open(name.str());
    ofs << std::scientific;

    for (size_t i = 2; i < N - 2; ++i)
    {
        ofs << xx[i] << " " << d1[i] << " " << d2[i] << " "
            << -3. * cos(xx[i]) * sin(xx[i]) * sin(xx[i]) << std::endl;
    }

    ofs.close();

    std::cout << span1.count() / (N - 4) << " " << span2.count() / (N - 4) << std::endl;

    delete[] xx;
    delete[] ff;
    delete[] d1;
    delete[] d2;
}

// vim: set ft=cpp.doxygen:
