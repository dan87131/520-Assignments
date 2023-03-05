#include "RandomProcess.h"
#include <cstdlib>

void RandomProcess::init() {}

void RandomProcess::start() {}

void RandomProcess::update() {
    double r = (double)rand() / RAND_MAX;
    channel("link").send(r);
}

void RandomProcess::stop() {}

