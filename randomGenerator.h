#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <boost/random/mersenne_twister.hpp>

class randomGenerator
{
    using mt = boost::random::mt19937;

public:
    randomGenerator(int);
    mt getGen() const {return gen;}
    int uniform_int(int a=0, int b=1);
    double uniform_real(double a=0, double b=1);
    double rayleigh(double, double, double);
    double triangular(double, double, double);
    double normal(double,double,double,double);
    double weibull(double,double,double,double);

private:
    mt gen;
    int seed;
};

#endif // RANDOMGENERATOR_H
