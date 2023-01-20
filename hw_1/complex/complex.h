struct Complex {
    double real;
    double im;
};

struct Complex add(struct Complex a, struct Complex b);
struct Complex negate(struct Complex a);
struct Complex multiply(struct Complex a, struct Complex b);
double magnitude(struct Complex a);
