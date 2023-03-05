// filter.h
#ifndef FILTER_H
#define FILTER_H
#include "elma/elma.h"
using namespace elma;

class Filter : public Process {
  private:
    int _count;
    double _sum;
    double _average;
    std::deque<double> _vals;
  public:
    Filter(std::string name) : Process(name), _count(0), _sum(0), _average(0) {}
    void init();
    void start();
    void update();
    void stop();
    double value();
};

#endif

