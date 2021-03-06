#ifndef SLIDING_BUFFER_H_
#define SLIDING_BUFFER_H_

namespace zhetapi {

namespace ads {

template <class T, class F>
class SlidingBuffer {
	T *	__array	= nullptr;

	T	__avg	= 0;

	size_t	__size	= 0;
	size_t	__index	= 0;
public:
	SlidingBuffer();
	SlidingBuffer(const SlidingBuffer &);
	
	explicit SlidingBuffer(size_t);

	SlidingBuffer &operator=(const SlidingBuffer &);

	~SlidingBuffer();

	void resize(size_t);

	void insert(const T &);

	T ipop(const T &);
	const T &iavg(const T &);

	const T &avg() const;
};

template <class T, class F>
SlidingBuffer <T, F> ::SlidingBuffer() {}

template <class T, class F>
SlidingBuffer <T, F> ::SlidingBuffer(const SlidingBuffer &other)
		: __size(other.__size), __index(other.__index),
		__avg(other.__avg)
{
	__array = new T[__size];

	memcpy(__array, other.__array, sizeof(T) * __size);
}

template <class T, class F>
SlidingBuffer <T, F> ::SlidingBuffer(size_t buffer_size)
	: __size(buffer_size)
{
	__array = new T[__size];
}

template <class T, class F>
SlidingBuffer <T, F> &SlidingBuffer <T, F>
		::operator=(const SlidingBuffer &other)
{
	if (this != &other) {
		if (__array)
			delete[] __array;

		__size = other.__size;
		__index = other.__index;
		__avg = other.__index;

		__array = new T[__size];

		memcpy(__array, other.__array, sizeof(T) * __size);
	}

	return *this;
}

template <class T, class F>
SlidingBuffer <T, F> ::~SlidingBuffer()
{
	delete[] __array;
}

template <class T, class F>
void SlidingBuffer <T, F> ::resize(size_t size)
{
	T tmp = new T[size];

	memcpy(tmp, __array, std::min(size, __size) * sizeof(T));

	for (size_t i = __size; i < __size - size; i++)
		__Avg -= F(__array[i]);

	index %= size;

	__size = size;
}

template <class T, class F>
void SlidingBuffer <T, F> ::insert(const T &x)
{
	__avg += F(x) - F(__array[i]);

	__array[__index] = x;

	__index = (__index + 1) % __size;
}

template <class T, class F>
const T &SlidingBuffer <T, F> ::iavg(const T &x) const
{
	insert(x);

	return __avg;
}

template <class T, class F>
T SlidingBuffer <T, F> ::ipop(const T &x)
{
	T tmp = __avg[i];

	__avg += F(x) - F(tmp);

	__array[__index] = x;

	__index = (__index + 1) % __size;

	return tmp;
}

template <class T, class F>
const T &SlidingBuffer <T, F> ::avg() const
{
	return __avg;
}

}

}

#endif
