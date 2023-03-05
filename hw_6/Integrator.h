#ifndef INTEGRATOR_H
#define INTEGRATOR_H

class Integrator {
public:
    // Constructor
    Integrator();

    // Public methods
    void set_delta_time(double delta_time);
    void update(double v);
    double value();

private:
    // Private variables
    double integral;
    double delta_time;
};

#endif // INTEGRATOR_H
