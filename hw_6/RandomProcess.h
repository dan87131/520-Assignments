// random_process.h
#ifndef RANDOM_PROCESS_H
#define RANDOM_PROCESS_H

#include "elma/elma.h"
using namespace elma;

class RandomProcess : public Process {
  public:
    RandomProcess(std::string name) : Process(name) {}
    void init();
    void start();
    void update();
    void stop();
};

#endif

