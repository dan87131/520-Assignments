#include "Complex.h"

Complex::Complex(double real, double imaginary) : real_part(real), imaginary_part(imaginary) {}

double Complex::real() const {
    return real_part;
}

double Complex::imaginary() const {
    return imaginary_part;
}

Complex Complex::conjugate() const {
    return Complex(real_part, -imaginary_part);
}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real_part + other.real_part, imaginary_part + other.imaginary_part);
}

Complex Complex::operator*(const Complex& other) const {
    double r = real_part * other.real_part - imaginary_part * other.imaginary_part;
    double i = real_part * other.imaginary_part + imaginary_part * other.real_part;
    return Complex(r, i);
}

bool Complex::operator==(const Complex& other) const {
    return (real_part == other.real_part) && (imaginary_part == other.imaginary_part);
}
