class Complex {
public:
    Complex(double real, double imaginary);
    double real();
    double imaginary();
    Complex conjugate();

    // Operator overloading
    Complex operator+(const Complex& other);
    Complex operator*(const Complex& other);
    bool operator==(const Complex& other);

private:
    double real_part;
    double imaginary_part;
};