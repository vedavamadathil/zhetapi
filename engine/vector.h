#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <matrix.h>

/**
 * @brief Representative
 * of a general Vector,
 * with components of type
 * T. In relation to a Matrix,
 * these are only column vectors:
 * transpose them to get the
 * corresponding row vectors.
 */
template <class T>
class Vector : public Matrix <T> {
public:
	Vector(T *);
	Vector(const Vector &);
	Vector(const Matrix <T> &);

	Vector(const std::vector <T> &);
	Vector(const std::initializer_list <T> &);

	Vector(size_t, T *);
	Vector(size_t = 0, T = T());
	
	Vector(size_t, std::function <T (size_t)>);
	Vector(size_t, std::function <T *(size_t)>);

	const Vector <T> &operator=(const Matrix <T> &);

	size_t size() const;

	T &operator[](size_t);
	const T &operator[](size_t) const;

	// Concatenating vectors
	Vector append_above(const T &);
	Vector append_below(const T &);

	T norm() const;

	void normalize();

	Vector normalized();

	template <class U>
	friend U inner(const Vector <U> &, const Vector <U> &);

	template <class U>
	friend Vector <U> cross(const Vector <U> &, const Vector <U> &);
};

template <class T>
Vector <T> ::Vector(T *ref)
{
}

template <class T>
Vector <T> ::Vector(const Vector &other) : Matrix <T> (other) {}

template <class T>
Vector <T> ::Vector(const Matrix <T> &other)
{
	*this = other;
}

template <class T>
Vector <T> ::Vector(const std::vector <T> &ref) : Matrix <T> (ref)
{
	/* std::vector <std::vector <T>> pass(ref.size());
	for (auto t : ref)
		pass.push_back({t});
	*this = Matrix <T> (pass); */
}

template <class T>
Vector <T> ::Vector(const std::initializer_list <T> &ref)
	: Vector(std::vector <T> (ref)) {}

template <class T>
Vector <T> ::Vector(size_t rs, T *ref)
{

}

template <class T>
Vector <T> ::Vector(size_t rs, T def) : Matrix <T> (rs, 1, def) {}

template <class T>
Vector <T> ::Vector(size_t rs, std::function <T (size_t)> gen)
	: Matrix <T> (rs, 1, gen) {}

template <class T>
Vector <T> ::Vector(size_t rs, std::function <T *(size_t)> gen)
	: Matrix <T> (rs, 1, gen) {}

template <class T>
const Vector <T> &Vector <T> ::operator=(const Matrix <T> &other)
{
	Vector <T> *out = new Vector <T> (other.get_rows(),
		[&](size_t i) {
			return other[i][0];
	});

	*this = *out;

	return *this;
}

template <class T>
size_t Vector <T> ::size() const
{
	return this->rows;
}

template <class T>
T &Vector <T> ::operator[](size_t i)
{
	return this->m_array[i][0];
}

template <class T>
const T &Vector <T> ::operator[](size_t i) const
{
	return this->m_array[i][0];
}

template <class T>
Vector <T> Vector <T> ::append_above(const T &x)
{
	size_t t_sz = size();

	std::vector <T> total {x};

	for (size_t i = 0; i < t_sz; i++)
		total.push_back((*this)[i]);

	return Vector(total);
}

template <class T>
Vector <T> Vector <T> ::append_below(const T &x)
{
	size_t t_sz = size();

	std::vector <T> total;

	for (size_t i = 0; i < t_sz; i++)
		total.push_back((*this)[i]);

	total.push_back(x);

	return Vector(total);
}

template <class T>
T Vector <T> ::norm() const
{
	return sqrt(inner(*this, *this));
}

template <class T>
void Vector <T> ::normalize()
{
	T dt = norm();

	for (size_t i = 0; i < size(); i++)
		(*this)[i] /= dt;
}

template <class T>
Vector <T> Vector <T> ::normalized()
{
	std::vector <T> out;

	T dt = norm();

	for (size_t i = 0; i < size(); i++)
		out.push_back((*this)[i]/dt);

	return Vector(out);
}

template <class T>
T inner(const Vector <T> &a, const Vector <T> &b)
{
	T acc = 0;

	assert(a.size() == b.size());
	for (size_t i = 0; i < a.rows; i++)
		acc += a[i] * b[i];

	return acc;
}

template <class T>
T cross(const Vector <T> &a, const Vector <T> &b)
{
	assert(a.size() == b.size() == 3);

	return {
		a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0]
	};
}

#endif