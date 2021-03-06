#include <std/interval.hpp>

namespace zhetapi {

namespace utility {

dre Interval <1> ::gen;
udb Interval <1> ::distro = udb(0, 1);

Interval <1> operator|(const Interval <1> &a, const Interval <1> &b)
{
	Interval <1> out = a;

	return out |= b;
}

std::ostream &operator<<(std::ostream &os, const Interval <1> &itv)
{
	size_t sz = itv.__union.size();

	for (size_t i = 0; i < sz; i++) {
		auto itr = itv.__union.begin();

		std::advance(itr, i);

		if (itr->closed)
			os << "[";
		else
			os << "(";

		os << itr->left << ", " << itr->right;
		
		if (itr->closed)
			os << "]";
		else
			os << ")";

		if (i < sz - 1)
			os << " U ";
	}

	return os;
}

// Literal constructor
Interval <1> operator""_I(unsigned long long int x)
{
	return Interval <1> (x);
}

Interval <1> operator""_I(long double x)
{
	return Interval <1> (x);
}

/*
template <size_t N, size_t M>
Interval <N + M> operator*(const Interval <N> &, const Interval <M> &)
{
} */

}

}