#include "Integrator.h"

Integrator::Integrator()
    : integral(0.0), delta_time(1.0)
{
}

void Integrator::set_delta_time(double delta_time)
{
    this->delta_time = delta_time;
}

void Integrator::update(double v)
{
    integral += delta_time * v;
}

double Integrator::value()
{
    return integral;
}
