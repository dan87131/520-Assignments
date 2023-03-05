#include "Filter.h"
#include <iostream>

void Filter::init() {}

void Filter::start() {}

void Filter::update() {
    double r = channel("link").latest();
    std::cout << "Received value: " << r << std::endl;
    _sum += r;
    _count++;

    if (_count > 10) {
        if (_vals.empty()) {
            return;
        }
        _sum -= _vals.front();
        _vals.pop_front();
        _count -= 1;
    }
    else {
        _vals.push_back(r);
    }
    _average = _sum / _count;
}

void Filter::stop() {}

double Filter::value() {
    return _average;
}
