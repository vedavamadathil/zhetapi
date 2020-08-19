#ifndef ZCOMPLEX_H_
#define ZCOMPLEX_H_

#include <complex>
#include <initializer_list>
#include <iostream>

/**
 * @brief The Complex class is an
 * extension of the std::complex
 * class which includes a more convenient
 * set of methods, such as normalization.
 */
template <class T>
class Complex : public std::complex <T> {
public:
	// Constructor
	Complex(const T & = 0);
	Complex(const T &, const T &);

	Complex(const std::complex <T> &);

	// Getters
	T magnitude() const;

	bool is_real() const;

	operator double() const;

	// Functional Methods
	Complex normalize() const;

	// Output Methods
	template <class U>
	friend std::string std::to_string(const Complex <U> &);

	template <class U>
	friend std::ostream &operator<<(std::ostream &, const Complex <U> &);
};

//////////////////////////////////////////
// Constructors
//////////////////////////////////////////

template <class T>
Complex <T> ::Complex(const T &re)
	: std::complex <T> (re) {}

template <class T>
Complex <T> ::Complex(const T &re, const T &im)
	: std::complex <T> (re, im) {}

template <class T>
Complex <T> ::Complex(const std::complex <T> &z)
	: std::complex <T> (z) {}

//////////////////////////////////////////
// Getters
//////////////////////////////////////////

template <class T>
T Complex <T> ::magnitude() const
{
	return sqrt(norm(*this));
}

template <class T>
bool Complex <T> ::is_real() const
{
	return this->imag() == 0;
}

template <class T>
Complex <T> ::operator double() const
{
	// std::cout << "Zcompex conv: Here" << std::endl;
	return (double) this->real();
}

//////////////////////////////////////////
// Functional Methods
//////////////////////////////////////////

template <class T>
Complex <T> Complex <T> ::normalize() const
{
	return *this/magnitude();
}

bool operator<(const Complex <long double> &a, const Complex <long double> &b)
{
	return norm(a) < norm(b);
}

bool operator<=(const Complex <long double> &a, const Complex <long double> &b)
{
	return norm(a) <= norm(b);
}

//////////////////////////////////////////
// Output Methods
//////////////////////////////////////////
namespace std {

	template <class T>
	std::string to_string(const Complex <T> &z)
	{
		std::string str;

		bool pl = false;

		if (z.real()) {
			pl = true;
			str += to_string(z.real());
		}

		if (z.imag()) {
			if (pl)
				str += " + ";
			str += to_string(z.imag()) + "i";
		}

		return str;
	}

}

template <class T>
std::ostream &operator<<(std::ostream &os, const Complex <T> &z)
{
	bool pl = false;

	if (!(z.real() || z.imag())) {
		os << "0";
		return os;
	}

	if (z.real()) {
		pl = true;
		os << z.real();
	}

	if (z.imag()) {
		if (pl)
			os << " + ";
		os << z.imag() << "i";
	}

	return os;
}

#endif