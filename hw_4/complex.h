class Complex {
public:
    Complex(double real, double imaginary);
    double real() const;
    double imaginary() const;
    Complex conjugate() const;
    Complex operator+(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    bool operator==(const Complex& other) const;

private:
    double real_part;
    double imaginary_part;
};
