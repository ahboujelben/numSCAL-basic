/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <libs/boost/random/mersenne_twister.hpp>

class randomGenerator {
  using mt = boost::random::mt19937;

 public:
  randomGenerator(int seed);
  ~randomGenerator() {}
  randomGenerator(const randomGenerator &) = delete;
  randomGenerator(randomGenerator &&) = delete;
  auto operator=(const randomGenerator &) -> randomGenerator & = delete;
  auto operator=(randomGenerator &&) -> randomGenerator & = delete;
  mt getGen() const;
  int uniform_int(int a = 0, int b = 1);
  double uniform_real(double a = 0, double b = 1);
  double rayleigh(double, double, double);
  double triangular(double, double, double);
  double normal(double, double, double, double);
  double weibull(double, double, double, double);

 private:
  mt gen;
};

#endif  // RANDOMGENERATOR_H
