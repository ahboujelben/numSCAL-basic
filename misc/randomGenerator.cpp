/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "randomGenerator.h"

#include <cmath>
#include <libs/boost/random.hpp>
#include <libs/boost/random/uniform_int_distribution.hpp>
#include <libs/boost/random/uniform_real_distribution.hpp>
#include <libs/boost/random/normal_distribution.hpp>

randomGenerator::randomGenerator(int seed)
{
    gen.seed(seed);
}

randomGenerator::mt randomGenerator::getGen() const
{
    return gen;
}

int randomGenerator::uniform_int(int a, int b)
{
    if (a == b)
        return a;
    boost::random::uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

double randomGenerator::uniform_real(double a, double b)
{
    if (a == b || a > b)
        return a;
    boost::random::uniform_real_distribution<> dist(a, b);
    return dist(gen);
}

double randomGenerator::rayleigh(double min, double max, double ryParam)
{
    if (min == max)
    {
        return min;
    }
    auto value = min + sqrt(-pow(ryParam, 2) * log(1 - uniform_real() * (1 - exp(-pow((max - min), 2) / pow(ryParam, 2)))));
    return value;
}

double randomGenerator::triangular(double a, double b, double c)
{
    if (a == b || c < a || c > b)
    {
        return a;
    }

    auto fc = (c - a) / (b - a);
    auto u = uniform_real();

    if (u < fc)
        return a + sqrt(u * (b - a) * (c - a));
    else
        return b - sqrt((1 - u) * (b - a) * (b - c));
}

double randomGenerator::normal(double min, double max, double mu, double sigma)
{
    if (min == max || mu < min || mu > max)
    {
        return min;
    }

    boost::normal_distribution<> nd(mu, sigma);
    boost::variate_generator<boost::mt19937 &, boost::normal_distribution<>> var_nor(gen, nd);

    auto value = var_nor();
    while (value < min || value > max)
        value = var_nor();

    return value;
}

double randomGenerator::weibull(double min, double max, double alpha, double beta)
{
    if (min == max)
    {
        return min;
    }

    auto u = uniform_real();
    auto value = (max - min) * pow(-beta * log(u * (1 - exp(-1 / beta)) + exp(-1 / beta)), 1 / alpha) + min;

    return value;
}
