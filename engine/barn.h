#ifndef BARN_H_
#define BARN_H_

// C/C++ headers
#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <typeindex>
#include <typeinfo>

// Engine headers
#include <combinatorial.h>
#include <complex.h>
#include <matrix.h>
#include <operand.h>
#include <operation.h>
#include <rational.h>
#include <token.h>
#include <vector.h>
#include <vtable.h>

#define __add_binary_operation(str, A, B, O)									\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {					\
					typeid(operand <A>),					\
					typeid(operand <B>)					\
				}								\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#A) + ") " + std::string(#str)		\
					+ " $2 (" + std::string(#B) + ")",			\
				2,								\
				[&](const std::vector <token *> &ins) {				\
					operand <A> *a = dynamic_cast <operand <A> *> (ins[0]);	\
					operand <B> *b = dynamic_cast <operand <B> *> (ins[1]);	\
												\
					return new operand <O> (a->get() str b->get());		\
				}								\
			}									\
	});

#define __add_heterogenous_binary_operation(str, A, B, O)									\
	__add_binary_operation(str, A, B, O)									\
	__add_binary_operation(str, B, A, O)

#define __add_binary_operation_ftr(str, A, B, O, ftr)								\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {					\
					typeid(operand <A>),					\
					typeid(operand <B>)					\
				}								\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#A) + ") " + std::string(#str)		\
					+ " $2 (" + std::string(#B) + ")",			\
				2,								\
				[&](const std::vector <token *> &ins) {				\
					operand <A> *a = dynamic_cast <operand <A> *> (ins[0]);	\
					operand <B> *b = dynamic_cast <operand <B> *> (ins[1]);	\
												\
					return new operand <O> (ftr);				\
				}								\
			}									\
	});

#define __add_heterogenous_binary_operation_ftr(str, A, B, O, ftr)								\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {					\
					typeid(operand <A>),					\
					typeid(operand <B>)					\
				}								\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#A) + ") " + std::string(#str)		\
					+ " $2 (" + std::string(#B) + ")",			\
				2,								\
				[&](const std::vector <token *> &ins) {				\
					operand <A> *a = dynamic_cast <operand <A> *> (ins[0]);	\
					operand <B> *b = dynamic_cast <operand <B> *> (ins[1]);	\
												\
					return new operand <O> (ftr);				\
				}								\
			}									\
	});											\
												\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {					\
					typeid(operand <B>),					\
					typeid(operand <A>)					\
				}								\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#B) + ") " + std::string(#str)		\
					+ " $2 (" + std::string(#A) + ")",			\
				2,								\
				[&](const std::vector <token *> &ins) {				\
					operand <A> *a = dynamic_cast <operand <A> *> (ins[1]);	\
					operand <B> *b = dynamic_cast <operand <B> *> (ins[0]);	\
												\
					return new operand <O> (ftr);				\
				}								\
			}									\
	});

#define __add_binary_operation_set(str)										\
	__add_binary_operation(str, Z, Z, Z);									\
	__add_binary_operation(str, R, R, R);									\
	__add_binary_operation(str, Q, Q, Q);									\
	__add_binary_operation(str, CR, CR, CR);									\
	__add_binary_operation(str, CQ, CQ, CQ);									\
												\
	__add_heterogenous_binary_operation(str, R, Z, R);									\
	__add_heterogenous_binary_operation_ftr(str, Z, Q, Q, Q(a->get()) str b->get());					\
	__add_heterogenous_binary_operation(str, R, Q, R);									\
	__add_heterogenous_binary_operation(str, R, CR, CR);									\
	__add_heterogenous_binary_operation_ftr(str, R, CQ, CR, CR(a->get() str R(b->get().real()), R(b->get().imag())));	\
	__add_heterogenous_binary_operation_ftr(str, Z, CR, CR, CR(a->get() str b->get().real(), b->get().imag()));		\
	__add_heterogenous_binary_operation_ftr(str, Z, CQ, CQ, CQ(a->get() str b->get().real(), b->get().imag()));		\
	__add_heterogenous_binary_operation_ftr(str, Q, CR, CR, CR(R(a->get()) str b->get().real(), b->get().imag()));	\
	__add_heterogenous_binary_operation_ftr(str, Q, CQ, CQ, CQ(a->get() str b->get().real(), b->get().imag()));		\
	__add_heterogenous_binary_operation_ftr(str, CR, CQ, CR, CR(a->get().real() str (R) b->get().real(),			\
		a->get().imag() str (R) b->get().imag()));

#define __add_unary_operation(str, I, O)									\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {typeid(operand <I>)}		\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#I) + ")",					\
				1, 								\
				[&](const std::vector <token *> &ins) {				\
					operand <I> *in = dynamic_cast				\
						<operand <I> *> (ins[0]);			\
												\
					return new operand <O> (str(in->get()));		\
				}								\
			}									\
	});

#define __add_unary_operation_ftr(str, I, O, ftr)								\
	ops.push_back({										\
			{									\
				std::string(#str),						\
				std::vector <std::type_index> {typeid(operand <I>)}		\
			},									\
												\
			new operation <O> {							\
				std::string(#str),						\
				"$1 (" + std::string(#I) + ")",					\
				1, 								\
				[&](const std::vector <token *> &ins) {				\
					operand <I> *in = dynamic_cast				\
						<operand <I> *> (ins[0]);			\
												\
					return new operand <O> (ftr);				\
				}								\
			}									\
	});

/**
 * @brief Represents the working
 * space of a [zhetapi] function
 * or application; the sets of
 * integer, real, complex, Rational,
 * vector and Matrix fields.
 *
 * @tparam R The type of a scalar
 * real value. Defaults to
 * [long double].
 *
 * @tparam Z The type of a scalar
 * integer value (used in Rationals).
 * Defaults to [long long int].
 */
template <class R = long double, class Z = long long int>
class barn {
public:
	using Q = Rational <Z>;

	using CR = Complex <R>;
	using CQ = Complex <Q>;

	using MR = Matrix <R>;
	using MQ = Matrix <Q>;

	using MCR = Matrix <CR>;
	using MCQ = Matrix <CQ>;

	using VR = Vector <R>;
	using VQ = Vector <Q>;

	using VCR = Vector <CR>;
	using VCQ = Vector <CQ>;

	using opd_z = operand <Z>;
	using opd_r = operand <R>;
	using opd_q = operand <Q>;
	using opd_cr = operand <CR>;
	using opd_cq = operand <CQ>;

	using ID = std::pair <std::string, std::vector <std::type_index>>;
private:
	vtable <Z> z;
	vtable <R> r;
	vtable <Q> q;

	vtable <CR> rc;
	vtable <CQ> qc;

	vtable <MR> rm;
	vtable <MQ> qm;

	// ftable <R, Z> maps;

	std::vector <std::pair <ID, token *>> ops;
public:
	barn();

	void put_z(const Z &);
	void put_r(const R &);
	void put_q(const Q &);
	void put_rc(const CR &);
	void put_qc(const CQ &);
	void put_rm(const MR &);
	void put_qm(const MQ &);

	token *mkop(const std::string &, const std::vector <std::type_index> &);
	token *value(const std::string &, const std::vector <std::type_index> &,
			const std::vector <token *> &) const;

	void print();
};

//////////////////////////////////////////
// Constructors
//////////////////////////////////////////

template <class R, class Z>
barn <R, Z> ::barn() : z(), r(), q(), rc(), qc(), rm(), qm()
{
	//////////////////////////////////////////
	// Real Scalar Arithemtic
	//////////////////////////////////////////
	
	// Addition and Subtraction
	__add_binary_operation_set(+);
	__add_binary_operation_set(-);

	// Multiplication
	__add_binary_operation(*, Z, Z, Z);
	__add_binary_operation(*, R, R, R);
	__add_binary_operation(*, Q, Q, Q);
	__add_binary_operation(*, CR, CR, CR);
	__add_binary_operation(*, CQ, CQ, CQ);

	__add_heterogenous_binary_operation(*, R, Z, R);
	__add_heterogenous_binary_operation(*, Z, Q, Q);
	__add_heterogenous_binary_operation_ftr(*, R, Q, R, a->get() * (R) b->get());
	__add_heterogenous_binary_operation(*, R, CR, CR);
	__add_heterogenous_binary_operation_ftr(*, R, CQ, CR, CR(a->get() * (R) b->get().real(), a->get() * (R) b->get().imag()));
	__add_heterogenous_binary_operation_ftr(*, Z, CR, CR, CR(a->get() * (R) b->get().real(), a->get() * (R) b->get().imag()));
	__add_heterogenous_binary_operation_ftr(*, Z, CQ, CQ, CQ((Q) a->get() * b->get().real(), (Q) a->get() * b->get().imag()));
	__add_heterogenous_binary_operation_ftr(*, Q, CR, CR, CR((R) a->get() * b->get().real(), (R) a->get() * b->get().imag()));
	__add_heterogenous_binary_operation(*, Q, CQ, CQ);

	// Division
	__add_binary_operation(/, R, R, R);
	__add_binary_operation(/, Q, Q, Q);
	__add_binary_operation(/, CR, CR, CR);
	__add_binary_operation(/, CQ, CQ, CQ);

	__add_heterogenous_binary_operation(/, R, Z, R);
	__add_heterogenous_binary_operation(/, Z, Q, Q);
	__add_heterogenous_binary_operation_ftr(/, R, Q, R, a->get() * (R) b->get());
	__add_heterogenous_binary_operation(/, R, CR, CR);
	__add_heterogenous_binary_operation_ftr(/, R, CQ, CR, CR(a->get() * (R) b->get().real(), a->get() * (R) b->get().imag()));
	__add_heterogenous_binary_operation_ftr(/, Z, CR, CR, CR(a->get() * (R) b->get().real(), a->get() * (R) b->get().imag()));
	__add_heterogenous_binary_operation_ftr(/, Z, CQ, CQ, CQ((Q) a->get() * b->get().real(), (Q) a->get() * b->get().imag()));
	__add_heterogenous_binary_operation_ftr(/, Q, CR, CR, CR((R) a->get() * b->get().real(), (R) a->get() * b->get().imag()));
	__add_heterogenous_binary_operation(/, Q, CQ, CQ);

	// Exponentiation
	__add_binary_operation_ftr(^, Z, Z, Z, (Z) pow(a->get(), b->get()));
	__add_binary_operation_ftr(^, R, R, R, pow(a->get(), b->get()));
	__add_binary_operation_ftr(^, Q, Q, R, pow((R) a->get(), (R) b->get()));
	__add_binary_operation_ftr(^, CR, CR, CR, 1);
	__add_binary_operation_ftr(^, CQ, CQ, CQ, Q(1));

	// Trigonometry
	__add_unary_operation(sin, Z, R);
	__add_unary_operation_ftr(sin, Q, R, sin(R (in->get())));
	__add_unary_operation(sin, R, R);
	
	__add_unary_operation(cos, Z, R);
	__add_unary_operation_ftr(cos, Q, R, cos(R (in->get())));
	__add_unary_operation(cos, R, R);
	
	__add_unary_operation(tan, Z, R);
	__add_unary_operation_ftr(tan, Q, R, tan(R (in->get())));
	__add_unary_operation(tan, R, R);
	
	__add_unary_operation_ftr(csc, Z, R, 1/sin(in->get()));
	__add_unary_operation_ftr(csc, Q, R, 1/sin(R (in->get())));
	__add_unary_operation_ftr(csc, R, R, 1/sin(in->get()));
	
	__add_unary_operation_ftr(sec, Z, R, 1/cos(in->get()));
	__add_unary_operation_ftr(sec, Q, R, 1/cos(R (in->get())));
	__add_unary_operation_ftr(sec, R, R, 1/cos(in->get()));
	
	__add_unary_operation_ftr(cot, Z, R, 1/tan(in->get()));
	__add_unary_operation_ftr(cot, Q, R, 1/tan(R (in->get())));
	__add_unary_operation_ftr(cot, R, R, 1/tan(in->get()));

	// Hyperbolic Functions
	__add_unary_operation(sinh, Z, R);
	__add_unary_operation_ftr(sinh, Q, R, sinh(R (in->get())));
	__add_unary_operation(sinh, R, R);
	
	__add_unary_operation(cosh, Z, R);
	__add_unary_operation_ftr(cosh, Q, R, cosh(R (in->get())));
	__add_unary_operation(cosh, R, R);
	
	__add_unary_operation(tanh, Z, R);
	__add_unary_operation_ftr(tanh, Q, R, tanh(R (in->get())));
	__add_unary_operation(tanh, R, R);
	
	__add_unary_operation_ftr(csch, Z, R, 1/sinh(in->get()));
	__add_unary_operation_ftr(csch, Q, R, 1/sinh(R (in->get())));
	__add_unary_operation_ftr(csch, R, R, 1/sinh(in->get()));
	
	__add_unary_operation_ftr(sech, Z, R, 1/cosh(in->get()));
	__add_unary_operation_ftr(sech, Q, R, 1/cosh(R (in->get())));
	__add_unary_operation_ftr(sech, R, R, 1/cosh(in->get()));
	
	__add_unary_operation_ftr(coth, Z, R, 1/tanh(in->get()));
	__add_unary_operation_ftr(coth, Q, R, 1/tanh(R (in->get())));
	__add_unary_operation_ftr(coth, R, R, 1/tanh(in->get()));

	// Vector
	__add_binary_operation(+, VQ, VQ, VQ);
	__add_binary_operation(+, VR, VR, VR);
	
	__add_binary_operation(-, VQ, VQ, VQ);
	__add_binary_operation(-, VR, VR, VR);
	
	__add_binary_operation_ftr(., VQ, VQ, Q, inner(a->get(), b->get()));
	__add_binary_operation_ftr(., VR, VR, R, inner(a->get(), b->get()));
	
	__add_binary_operation_ftr(shur, VQ, VQ, VQ, shur(a->get(), b->get()));
	__add_binary_operation_ftr(shur, VR, VR, VR, shur(a->get(), b->get()));

	__add_unary_operation_ftr(transpose, VQ, MQ, in->get().transpose());
	__add_unary_operation_ftr(transpose, VR, MR, in->get().transpose());

	// Matrix
	__add_binary_operation(+, MQ, MQ, MQ);
	__add_binary_operation(+, MR, MR, MR);
	
	__add_binary_operation(-, MQ, MQ, MQ);
	__add_binary_operation(-, MR, MR, MR);
	
	__add_binary_operation_ftr(shur, MQ, MQ, MQ, shur(a->get(), b->get()));
	__add_binary_operation_ftr(shur, MR, MR, MR, shur(a->get(), b->get()));

	__add_unary_operation_ftr(transpose, MQ, MQ, in->get().transpose());
	__add_unary_operation_ftr(transpose, MR, MR, in->get().transpose());

	// Other Linear Algebra
	__add_binary_operation(*, MQ, VQ, MQ);
	__add_binary_operation(*, VQ, MQ, MQ);

	// Combinatorics
	__add_unary_operation_ftr(!, Z, Z, utility::integral_factorial(in->get())); 
	
	__add_binary_operation_ftr(binom, Z, Z, Z, utility::integral_binom(a->get(), b->get()));

	// Add API functions
}

template <class R, class Z>
token *barn <R, Z> ::value(const std::string &str,
		const std::vector <std::type_index> &types,
		const std::vector <token *> &vals) const
{
	auto it = ops.end();

	for (auto itr = ops.begin(); itr != ops.end(); itr++) {
		if (itr->first.first == str &&
			itr->first.second.size() == types.size()) {
			bool ps = true;

			for (size_t i = 0; i < types.size(); i++) {
				if (types[i] != itr->first.second[i]) {
					ps = false;
					break;
				}
			}

			if (ps) {
				it = itr;
				break;
			}
		}
	}

	if (it != ops.end()) {
		token *tptr = it->second;

		// cout << "Token Established: " << tptr->str() << endl;
		
		// Constants
		operation <R> *opn_r = dynamic_cast <operation <R> *> (tptr);
		if (opn_r != nullptr)
			return (*opn_r)(vals);

		operation <Z> *opn_z = dynamic_cast <operation <Z> *> (tptr);
		if (opn_z != nullptr)
			return (*opn_z)(vals);

		operation <Q> *opn_q = dynamic_cast <operation <Q> *> (tptr);
		if (opn_q != nullptr)
			return (*opn_q)(vals);

		operation <CR> *opn_cr = dynamic_cast <operation <CR> *> (tptr);
		if (opn_cr != nullptr)
			return (*opn_cr)(vals);

		operation <CQ> *opn_cq = dynamic_cast <operation <CQ> *> (tptr);
		if (opn_cq != nullptr)
			return (*opn_cq)(vals);

		// Vectors
		operation <VQ> *opn_vq = dynamic_cast <operation <VQ> *> (tptr);
		if (opn_vq != nullptr)
			return (*opn_vq)(vals);

		operation <VR> *opn_vr = dynamic_cast <operation <VR> *> (tptr);
		if (opn_vr != nullptr)
			return (*opn_vr)(vals);

		operation <VCQ> *opn_vcq = dynamic_cast <operation <VCQ> *> (tptr);
		if (opn_vcq != nullptr)
			return (*opn_vcq)(vals);

		operation <VCR> *opn_vcr = dynamic_cast <operation <VCR> *> (tptr);
		if (opn_vcr != nullptr)
			return (*opn_vcr)(vals);

		// Matrices
		operation <MQ> *opn_mq = dynamic_cast <operation <MQ> *> (tptr);
		if (opn_mq != nullptr)
			return (*opn_mq)(vals);

		operation <MR> *opn_mr = dynamic_cast <operation <MR> *> (tptr);
		if (opn_mr != nullptr)
			return (*opn_mr)(vals);

		operation <MCQ> *opn_mcq = dynamic_cast <operation <MCQ> *> (tptr);
		if (opn_mcq != nullptr)
			return (*opn_mcq)(vals);

		operation <MCR> *opn_mcr = dynamic_cast <operation <MCR> *> (tptr);
		if (opn_mcr != nullptr)
			return (*opn_mcr)(vals);
	} else {
		std::cout << "Invalid Token (Not Found)" << std::endl;
	}

	return nullptr;
}

template <class R, class Z>
void barn <R, Z> ::print()
{
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "INTEGERS:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	z.print();

	std::cout << std::string(50, '=') << std::endl;
	std::cout << "REALS:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	r.print();
	
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "RATIONALS:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	q.print();
	
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "REAL COMPLEX:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	rc.print();

	std::cout << std::string(50, '=') << std::endl;
	std::cout << "RATIONAL COMPLEX:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	qc.print();
	
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "REAL MATRICES:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	rm.print();
	
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "RATIONAL MATRICES:" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	qm.print();
}

#endif