#ifndef NODE_H_
#define NODE_H_

/* C++ Standard Libraries */
#include <cmath>
#include <cassert>
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

/* Engine Headers */
// #include "config.h"
#include <rational.h>
#include <complex.h>
#include <stree.h>
#include <variable.h>
#include <vtable.h>
#include <barn.h>
#include <operation_holder.h>
#include <config.h>

template <class T>
class vtable;

template <class T, class U>
class Function;

/**
 * @brief The enumeration
 * label is used to label nodes
 * of an expression tree.
 *
 * Clarifications:
 *  - l_logarithmic implies a
 *  logarithm of a variable base.
 *  - l_constant_logarithmic implies
 *  a logarithm of a constant base.
 */
enum nd_label {
	l_none,
	l_power,
	l_divided,
	l_variable,

	l_constant,		// keep to prevent errors
	l_constant_integer,
	l_constant_rational,
	l_constant_real,
	l_constant_complex_rational,
	l_constant_complex_real,
	l_constant_vector_rational,	// Convert integers to Rationals for vec/mat
	l_constant_vector_real,
	l_constant_vector_complex_rational,
	l_constant_vector_complex_real,
	l_constant_matrix_rational,
	l_constant_matrix_real,
	l_constant_matrix_complex_rational,
	l_constant_matrix_complex_real,

	l_matrix_uncoded,	// leave martix as nodes
	l_vector_uncoded,	// leave vector as nodes, then decode once substituion is performed

	l_function,
	l_exp,
	l_polynomial,
	l_separable,
	l_multiplied,
	l_exponential,
	l_logarithmic,
	l_trigonometric,
	l_power_uncertain,
	l_function_constant,
	l_operation_constant,
	l_constant_logarithmic,
	l_summation,
	l_summation_variable,
	l_summation_function,
	l_factorial
};

/**
 * @brief String representations
 * of the corresponding labels
 * in the label enumeration.
 */
std::string strlabs[] = {
	"none",
	"power",
	"divided",
	"variable",

	"constant",
	"constant integer",
	"constant rational",
	"constant real",
	"constant complex rational",
	"constant complex real",
	"constant vector rational",
	"constant vector real",
	"constant vector complex rational",
	"constant vector complex real",
	"constant matrix rational",
	"constant matrix real",
	"constant matrix complex rational",
	"constant matrix complex real",

	"function",
	"exponent",
	"polynomic",
	"separable",
	"multiplied",
	"exponential",
	"logarithmic",
	"trigonometric",
	"power uncertain",
	"function constant",
	"operation constant",
	"constant logarithmic",
	"summation",
	"summation variable",
	"summation function",
	"factorial"
};

enum nd_class {
	c_none,
	c_polynomial,
	c_rational,
	c_exponential
};

std::string strclass[] = {
	"none",
	"polynomial",
	"Rational",
	"exponential"
};

/**
 * @brief Singular node class,
 * used in expression trees.
 */
template <class T, class U>
class node {
public:
	// Friends
	friend class Function <T, int>;
	
	// Aliases
	using params = std::vector <variable <T>>;
	
	using opd_r = operand <T>;
	using opd_cr = operand <Complex <T>>;

	using opd_z = operand <U>;
	using opd_q = operand <Rational <U>>;
	using opd_cq = operand <Complex <Rational <U>>>;
	
	using opd_v_r = operand <Vector <T>>;
	using opd_v_cr = operand <Vector <Complex <T>>>;	
	using opd_v_q = operand <Vector <Rational <U>>>;
	using opd_v_cq = operand <Vector <Complex <Rational <U>>>>;
	
	using opd_m_r = operand <Matrix <T>>;
	using opd_m_cr = operand <Matrix <Complex <T>>>;	
	using opd_m_q = operand <Matrix <Rational <U>>>;
	using opd_m_cq = operand <Matrix <Complex <Rational <U>>>>;

	using opn = operation <T>;
	using opd = operand <T>;
	using var = variable <T>;
	// using ftr = Function <T, int>;
	// using cfg = config <T>;
private:
	// Members
	token *tok;			// Main unit being stored

	nd_label type;			// Auxiliary labeling
					// for compression and differentiation

	nd_class cls = c_none;		// Auxiliary labeling
					// for function specifics

	params pars;			// List of parameters that should be
					// considered in every process

	// shared_ptr <cfg> cfg_ptr;	// Ptr to the operation and numeric
					// configurations

	std::vector <node *> leaves;	// Leaves of the current tree node

	barn <T, U> brn = barn <T, U> ();
public:
	// Constructors
	node(std::string, vtable <T> = vtable <T> (), params = params());
			// shared_ptr <cfg> = shared_ptr <cfg> (new cfg()));
	node(stree *, vtable <T> = vtable <T> (), params = params()/*, shared_ptr
			<cfg> = shared_ptr <cfg> (new cfg()) */);
	node(token *t, nd_label l, std::vector <node *> lv/* shared_ptr <cfg> */);
	node(token *t, std::vector <node *> lv/* shared_ptr <cfg> */);
	node(const node &);
	node(node *);
	node();

	// Deconstructor
	~node();
	
	// Assignment Operator
	const node &operator=(const node &);
	
	// Setters
	void set(node *);
	void set(params);
	void set(token *, std::vector <node *>/* shared_ptr <cfg> */);
	void set(token *, nd_label, std::vector <node *> /*, shared_ptr <cfg> */);

	void retokenize(token *);
	void reparametrize(params);

	// Getters
	token *get_token() const;
	nd_class get_class() const;
	nd_label get_label() const;
	params get_params() const;
	// shared_ptr <cfg> get_config() const;
	std::vector <node *> get_leaves() const;

	node *child_at(size_t) const;
	node *operator[](size_t) const;

	// Functional
	bool valid() const;

	bool matches(node *) const;
	bool matches(const node &) const;

	token *value() const;

	void simplify();
	void compress();

	void differentiate(const std::string &);
	void stable_differentiate(const std::string &);

	void integrate(const std::string &);
	void stable_integrate(const std::string &);
	
	void classify();
	void label_all();
	void label(const std::vector <std::string> &);
	
	// Helper
	void print(int = 1, int = 0) const;
	void address_print(int = 1, int = 0) const;

	void traverse(const std::function <void (node)> &);
	
	std::string display() const;

	// Arithmetic 
	template <class A, class B>
	friend const node <A, B> &operator+(const node <A, B> &, const node <A, B> &);

	template <class A, class B>
	friend const node <A, B> &operator-(const node <A, B> &, const node <A, B> &);
	
	template <class A, class B>
	friend const node <A, B> &operator*(const node <A, B> &, const node <A, B> &);
	
	template <class A, class B>
	friend const node <A, B> &operator/(const node <A, B> &, const node <A, B> &);
private:
	// Token Factories
	opn *get(opcode) const;
	opn *get(const std::string &) const;

	// Memory Functions
	node *copy() const;
	
	void clear();

	// Stree Convertion
	node *convert(stree *, vtable <T> = vtable <T> ());
	node *convert_operation(stree *, vtable <T> = vtable <T> ());
	node *convert_summation(stree *, vtable <T> = vtable <T> ());
	node *convert_variable_cluster(stree *, vtable <T> = vtable <T> ());

	// Special Nodes
	bool special() const;

	// Labeling Methods
	void label_as_operand();
	void label_as_operation();
	void label_as_special(const std::vector <std::string> &vars);

	// Node Matching
	bool matches_term(const node &) const;

	// Compression Methods
	void compress_as_separable();
	void compress_as_multiplied();
	void compress_as_divided();
	void compress_as_power();
	void compress_as_exponential();

	// Differentiation Methods
	void differentiate_as_multiplied(const std::string &);
	void differentiate_as_divided(const std::string &);
	void differentiate_as_power(const std::string &);
	void differentiate_as_exponential(const std::string &);
	void differentiate_as_trigonometric(const std::string &);
	void differentiate_as_logarithmic(const std::string &);
	void differentiate_as_constant_logarithmic(const std::string &);
	void differentiate_as_function(const std::string &);

	// Integration Methods

	// Display Methods
	std::string display_as_operand(nd_label) const;
	std::string display_as_trigonometric() const;
	std::string display_as_function() const;
public:
	// Exceptions
	class incomputable_tree {};

	// Base class for informative exceptions
	class node_error {
		std::string str;
	public:
		node_error(std::string s)
			: str(s) {}

		const std::string &what() const {
			return str;
		}
	};
	
	// Exception for syntax error
	class syntax_error : public node_error {
	public:
		syntax_error(std::string s)
			: node_error(s) {}
	};
	
	// Exception for unknown symbols
	class undefined_symbol : public node_error {
	public:
		undefined_symbol(std::string s)
			: node_error(s) {}
	};
};

//////////////////////////////////////////
// Constructors
//////////////////////////////////////////

template <class T, class U>
node <T, U> ::node(std::string str, vtable <T> tbl, params params)
	: pars(params), type(l_none), cls(c_none)
{
	stree *st = new stree(str);

	cout << endl << "Node Stree:" << endl;
	st->print();
	
	node *out = convert(st, tbl);
	
	cout << endl << "Out:" << endl;
	out->address_print();
	out->print();

	*this = *out;

	// delete out;
	delete st;

	cout << endl << "This:" << endl;
	print();

	reparametrize(params);
	simplify();
}

template <class T, class U>
node <T, U> ::node(stree *raw, vtable <T> tbl, params prs)
	: pars(prs)
{
	node *out = convert(raw, tbl);

	*this = *out;

	delete out;

	reparametrize(prs);
	simplify();

	/* cout << "CONSTRUCTOR TWO:" << endl;
	print(); */
}

template <class T, class U>
node <T, U> ::node(token *t, nd_label l, std::vector <node <T, U> *> lv)
	: tok(t), type(l), leaves(lv)
{
	/* cout << "CONSTRUCTOR SIX:" << endl;
	print(); */
}

template <class T, class U>
node <T, U> ::node(token *t, std::vector <node <T, U> *> lv)
{
	tok = t;
	leaves = lv;
	// cfg_ptr = cptr;
	type = l_none;

	/* cout << "CONSTRUCTOR FIVE:" << endl;
	print(); */
}

template <class T, class U>
node <T, U> ::node(const node &other)
{
	*this = other;

	simplify();

	// cout << "Copying into this @" << this << endl;

	/* cout << "CONSTRUCTOR FOUR:" << endl;
	print(); */
}

template <class T, class U>
node <T, U> ::node(node *other)
{
	*this = *other;

	simplify();

	/* cout << "CONSTRUCTOR THREE:" << endl;
	print(); */
}

template <class T, class U>
node <T, U> ::node() : tok(nullptr), type(l_none), cls(c_none),
	pars({}), leaves({}) {}

//////////////////////////////////////////
// Deconstructors
//////////////////////////////////////////

template <class T, class U>
node <T, U> ::~node()
{
	/* cout << "DECONSTRUCTOR:" << endl;
	print(); */

	delete tok;

	for (size_t i = 0; i < leaves.size(); i++)
		delete leaves[i];
}

//////////////////////////////////////////
// Assignment Operator
//////////////////////////////////////////

template <class T, class U>
const node <T, U> &node <T, U> ::operator=(const node <T, U> &other)
{
	// cout << string(30, '_') << endl;
	// cout << "Assigning into this @" << this << endl;
	if (this != &other) {
		tok = other.tok->copy();
		type = other.type;
		cls = other.cls;

		// change cptr to shared_ptr
		// cfg_ptr = other.cfg_ptr;
		pars = other.pars;

		clear();
		for (size_t i = 0; i < other.leaves.size(); i++)
			leaves.push_back(other.leaves[i]->copy());
	}

	// cout << "Result:" << endl;
	// print();

	return *this;
}

//////////////////////////////////////////
// Setters
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::set(node *nd)
{
	tok = nd->tok;
	type = nd->type;
	leaves = nd->leaves;
	// cfg_ptr = nd->cfg_ptr;
}

template <class T, class U>
void node <T, U> ::set(params p)
{
	pars = p;
}

template <class T, class U>
void node <T, U> ::set(token *t, std::vector <node <T, U> *> lv)
{
	tok = t;
	leaves = lv;
	// cfg_ptr = cptr;
	type = l_none;
}

template <class T, class U>
void node <T, U> ::set(token *t, nd_label l, std::vector <node <T, U> *> lv)
{
	tok = t;
	type = l;
	leaves = lv;
	// cfg_ptr = cptr;
}

template <class T, class U>
void node <T, U> ::retokenize(token *t)
{
	delete tok;
	tok = t;
}

template <class T, class U>
void node <T, U> ::reparametrize(params pr)
{
	pars = pr;

	for (node *nd : leaves)
		nd->reparametrize(pr);
}

//////////////////////////////////////////
// Getters
//////////////////////////////////////////

template <class T, class U>
token *node <T, U> ::get_token() const
{
	return tok;
}

template <class T, class U>
nd_class node <T, U> ::get_class() const
{
	return cls;
}

template <class T, class U>
nd_label node <T, U> ::get_label() const
{
	return type;
}

template <class T, class U>
typename node <T, U> ::params node <T, U> ::get_params() const
{
	return pars;
}

/* template <class T, class U>
shared_ptr <typename node <T, U> ::cfg> node <T, U> ::get_config() const
{
	return cfg_ptr;
} */

template <class T, class U>
std::vector <node <T, U> *> node <T, U> ::get_leaves() const
{
	return leaves;
}

template <class T, class U>
node <T, U> *node <T, U> ::child_at(size_t i) const
{
	return leaves[i];
}

template <class T, class U>
node <T, U> *node <T, U> ::operator[](size_t i) const
{
	return leaves[i];
}

//////////////////////////////////////////
// Functional
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::valid() const
{
	size_t operands = 0;
	
	switch (tok->caller()) {
	case token::OPERAND:
		return true;
	case token::OPERATION:
		for (auto itr : leaves) {
			switch(itr->tok->caller()) {
			case token::OPERAND:
				operands++;
				break;
			case token::OPERATION:
				if (!itr->valid())
					return false;
				operands++;
				break;
			default:
				return false;
			}
		}

		if (operands != (dynamic_cast <opn *> (tok))->operands())
			return false;
	}

	return true;
}

template <class T, class U>
bool node <T, U> ::matches(const node <T, U> &other) const
{
	std::vector <node> first;
	std::vector <node> second;

	queue <node> que;

	node current;

	if (type == l_separable)
		que.push(*this);
	else
		first.push_back(*this);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_separable:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			first.push_back(current);
			break;
		}
	}
	
	if (other.type == l_separable)
		que.push(other);
	else
		second.push_back(other);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_separable:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			second.push_back(current);
			break;
		}
	}

	std::vector <node> complete;

	for (auto nd : first) {
		for (auto itr = second.begin(); itr != second.end(); itr++) {
			if (nd.matches_term(*itr)) {
				complete.push_back(nd);
				second.erase(itr);
				break;
			}
		}
	}

	return ((first.size() == complete.size()) && second.empty());
}

template <class T, class U>
token *node <T, U> ::value() const
{
	std::vector <token *> vals;
	std::vector <T> hard;

	std::vector <std::type_index> types;
	
	operation_holder *ophptr;
	
	opn *optr;

	token *tmp;

	opd_r *o_r;
	opd_z *o_z;
	opd_q *o_q;
	opd_cr *o_cr;
	opd_cq *o_cq;
	
	switch (tok->caller()) {
	case token::OPERAND:
		return tok;
		/* o_r = dynamic_cast <opd_r *> (tok);
		if (o_r != nullptr)
			return o_r;

		o_z = dynamic_cast <opd_z *> (tok);
		if (o_z != nullptr)
			return o_z;
		
		o_q = dynamic_cast <opd_q *> (tok);
		if (o_q != nullptr)
			return o_q;

		o_z = dynamic_cast <opd_z *> (tok);
		if (o_z != nullptr)
			return o_z;
		
		o_q = dynamic_cast <opd_q *> (tok);
		if (o_q != nullptr)
			return o_q; */

		throw incomputable_tree();
	case token::OPERATION_HOLDER:
		for (auto nd : leaves) {
			types.push_back(typeid(*(nd->value())));
			vals.push_back(nd->value());
		}
			// cout << "\tTYPE:" << typeid(*(nd->tok)).name() << endl;
		
		ophptr = dynamic_cast <operation_holder *> (tok);
		/* if (brn.mkop(ophptr->rep, types) != nullptr)
			cout << "\tINVALID OPERATION" << endl;
		else
			cout << "\tVALID OPERATION" << endl; */

		tmp = brn.value(ophptr->rep, types, vals);

		if (tmp == nullptr) {
			cout << "err @ node:" << endl;
			print();

			throw incomputable_tree();
		}

		return tmp;

		// break;
	/* case token::OPERATION:
		optr = dynamic_cast <opn *> (tok);

		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			for (auto itr : leaves)
				vals.push_back(itr->tok);
			return (*optr)(vals);
		}

		for (auto itr : leaves)
			vals.push_back(itr->value());

		tmp = (*optr)(vals);

		for (size_t i = 0; i < vals.size(); i++)
			delete vals[i];
		
		return tmp; */
	case token::VARIABLE:
		if (!(dynamic_cast <var *> (tok))->is_param()) 
			return dynamic_cast <var *> (tok);
		throw incomputable_tree();
	/* case token::FUNCTOR:
		for (auto itr : leaves)
			hard.push_back(itr->value());
		
		// change later to allow functions to accept
		// different types of tokens for computation
		// return (dynamic_cast <ftr *> (tok))->compute(hard);
		break; */
	}

	throw incomputable_tree();
}

template <class T, class U>
void node <T, U> ::simplify()
{
	label_all();

	// cout << endl << "POST WHOLE LABEL:" << endl;
	// print();

	compress();
	label_all();
}

template <class T, class U>
void node <T, U> ::compress()
{
	if (type == l_operation_constant
			|| type == l_function_constant) {
		token *tmp = value();

		delete tok;
		clear();

		tok = tmp;
		type = l_constant;

		return;
	}
	
	switch (type) {
	case l_power:
		compress_as_power();
		break;
	case l_separable:
		compress_as_separable();
		break;
	case l_multiplied:
		// cout << "MULT DECOMP:" << endl;
		// print();

		compress_as_multiplied();
		break;
	case l_divided:
		compress_as_divided();
		break;
	default:
		for (node *nd : leaves)
			nd->compress();
		break;
	}
}

template <class T, class U>
void node <T, U> ::differentiate(const std::string &var)
{
	switch (type) {
	case l_constant_logarithmic:
		differentiate_as_constant_logarithmic(var);
		break;
	case l_logarithmic:
		differentiate_as_logarithmic(var);
		break;
	case l_trigonometric:
		differentiate_as_trigonometric(var);
		break;
	case l_power:
		differentiate_as_power(var);
		break;
	case l_exponential:
		differentiate_as_exponential(var);
		break;
	case l_multiplied:
		differentiate_as_multiplied(var);
		break;
	case l_divided:
		differentiate_as_divided(var);
		break;
	case l_function:
		differentiate_as_function(var);
		break;
	case l_separable:
		leaves[0]->differentiate(var);
		leaves[1]->differentiate(var);
		break;
	case l_variable:
		delete tok;

		tok = new opd(1);
		break;
	case l_operation_constant:
	case l_constant:
		delete tok;
		clear();

		tok = new opd(0);
		type = l_constant;
		break;
	default:
		break;
	}
}

template <class T, class U>
void node <T, U> ::stable_differentiate(const std::string &var)
{
	label(var);
	differentiate(var);
}

template <class T, class U>
void node <T, U> ::integrate(const std::string &var)
{
}

template <class T, class U>
void node <T, U> ::stable_integrate(const std::string &var)
{
	label(var);
	integrate(var);
}

template <class T, class U>
void node <T, U> ::classify()
{
	for (auto nd : leaves)
		nd->classify();

	switch (type) {
	case l_constant:
	case l_operation_constant:
	case l_variable:
	case l_power:
		cls = c_polynomial;
		break;
	case l_separable:
	case l_multiplied:
		if (leaves[0]->cls == leaves[1]->cls == c_polynomial)
			cls = c_polynomial;
		break;
	case l_divided:
		if (leaves[0]->cls == leaves[1]->cls == c_polynomial)
			cls = c_rational;
		break;
	default:
		break;
	}
}

template <class T, class U>
void node <T, U> ::label_all()
{
	std::vector <std::string> names;
	
	for (auto var : pars)
		names.push_back(var.symbol());

	label(names);
}

template <class T, class U>
void node <T, U> ::label(const std::vector <std::string> &vars)
{
	std::string sym;
	
	bool constant = true;

	if (special())
		return label_as_special(vars);

	for (auto nd : leaves)
		nd->label(vars);

	switch (tok->caller()) {
	case token::OPERATION_HOLDER:
		label_as_operation();
		break;
	case token::FUNCTOR:
		type = l_function;
		
		for (node *nd : leaves) {
			if (nd->type != l_constant &&
					nd->type != l_operation_constant) {
				constant = false;
				break;
			}
		}
		
		if (constant)
			type = l_function_constant;

		break;
	case token::VARIABLE:
		sym = (dynamic_cast <var *> (tok))->symbol();
		
		if (find(vars.begin(), vars.end(), sym) != vars.end())
			type = l_variable;
		else
			type = l_constant;

		break;
	case token::OPERAND:
		label_as_operand();
		break;
	}
}

//////////////////////////////////////////
// Helper
//////////////////////////////////////////
template <class T, class U>
void node <T, U> ::print(int num, int lev) const
{
	int counter = lev;
	while (counter > 0) {
		std::cout << "\t";
		counter--;
	}

	std::string pr = "[";

	/* for (int i = 0; i < pars.size(); i++) {
		pr += pars[i].symbol();

		if (i < pars.size() - 1)
			pr += ", ";
	} */

	pr += "]";

	std::cout << "#" << num << " - [" << strclass[cls] << ", " << strlabs[type] << "] "
		<< tok->str() << " @ " << this << ", " << pr << endl;

	counter = 0;
	for (node *itr : leaves) {
		if (itr == nullptr)
			continue;
		itr->print(++counter, lev + 1);
	}
}

template <class T, class U>
void node <T, U> ::address_print(int num, int lev) const
{
	int counter = lev;
	while (counter > 0) {
		std::cout << "\t";
		counter--;
	}

	std::cout << "#" << num << " - [" << strlabs[type] << "] @ "
		<< this << std::endl;

	counter = 0;
	for (node *itr : leaves) {
		if (itr == nullptr)
			continue;
		itr->address_print(++counter, lev + 1);
	}
}

template <class T, class U>
void node <T, U> ::traverse(const std::function <void (node)> &fobj)
{
	fobj(*this);

	for (node *nd : leaves)
		nd->traverse(fobj);
}

template <class T, class U>
std::string node <T, U> ::display() const
{
	string str;
	size_t i;

	switch (type) {
	case l_separable:
		//if (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt()) == op_add)
		//	return leaves[0]->display() + " + " + leaves[1]->display();

		return leaves[0]->display() + " +/- " + leaves[1]->display();
	case l_multiplied: 
		return leaves[0]->display_as_operand(type) + " * " + leaves[1]->display_as_operand(type);
	case l_divided:
		return leaves[0]->display_as_operand(type) + " / " + leaves[1]->display_as_operand(type);
	case l_constant ... l_constant_matrix_complex_real:
		return tok->str();
	case l_variable:
	case l_summation_variable:
		return (dynamic_cast <var *> (tok))->symbol();
	//case l_function:
	//	return display_as_function();
	case l_summation_function:
		// str = (dynamic_cast <ftr *> (tok))->display();

		/* i = str.find(")");
		if (i != string::npos)
			return str.substr(i + 4, str.length()); */
		
		return "?";
	// case l_polynomial: Unnecessary label?
	case l_exp:
	case l_power:
	case l_exponential:
		return leaves[0]->display_as_operand(type) + "^" + leaves[1]->display_as_operand(type);
	// remove operation constant later
	case l_operation_constant:
		return value()->str();
	case l_constant_logarithmic:
	case l_logarithmic:
		return "log_{" + leaves[0]->display() + "} (" + leaves[1]->display() + ")";
	case l_trigonometric:
		return display_as_trigonometric();
	case l_summation:
		return "sum^{" + leaves[2]->display() + "}_{" + leaves[0]->display() + " = " + leaves[1]->display()
			+ "} " + leaves[3]->display();
	case l_factorial:
		return leaves[0]->display() + "!";
	default:
		return "?";
	}
}

//////////////////////////////////////////
// Arithmetic
//////////////////////////////////////////

template <class T, class U>
const node <T, U> &operator+(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_add), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator-(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_sub), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator*(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_mul), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator/(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_div), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

//////////////////////////////////////////
// Token Factories
//////////////////////////////////////////

template <class T, class U>
typename node <T, U> ::opn *node <T, U> ::get(opcode ocode) const
{
	// return cfg_ptr->alloc_opn(ocode);
	return nullptr;
}

template <class T, class U>
typename node <T, U> ::opn *node <T, U> ::get(const std::string &str) const
{
	// return cfg_ptr->alloc_opn(str);
	return nullptr;
}

//////////////////////////////////////////
// Memory Functions
//////////////////////////////////////////

template <class T, class U>
node <T, U> *node <T, U> ::copy() const
{
	node *cpy;

	cpy = new node(tok->copy(), type, {});
	
	cpy->pars = pars;

	node *tmp;
	for (size_t i = 0; i < leaves.size(); i++) {
		tmp = leaves[i]->copy();

		/* cout << "RECEIVED:" << endl;
		tmp->print();
		cout << "-------------------" << endl; */

		cpy->leaves.push_back(tmp);
	}

	/* cout << "-------------------" << endl;
	cout << "RETURNED:" << endl;
	cpy->print(); */
	
	return cpy;
}

template <class T, class U>
void node <T, U> ::clear()
{
	for (size_t i = 0; i < leaves.size(); i++)
		delete leaves[i];
	leaves.clear();
}

//////////////////////////////////////////
// Stree Convertion
//////////////////////////////////////////

template <class T, class U>
node <T, U> *node <T, U> ::convert(stree *st, vtable <T> tbl)
{
	node *out = nullptr;

	T val;

	U num;
	U den;

	nd_label mn;

	char c;
	char i;

	istringstream iss;

	vector <node *> vec;

	// create aliases
	vector <Rational <U>> rats;
	vector <T> reals;
	vector <Complex <Rational <U>>> crats;
	vector <Complex <T>> creals;
	
	vector <vector <Rational <U>>> m_rats;
	vector <vector <T>> m_reals;
	vector <vector <Complex <Rational <U>>>> m_crats;
	vector <vector <Complex <T>>> m_creals;

	// matrices
	
	vector <vector <node *>> mat;

	// flags
	bool fl_real = false;
	
	/* Vector <Rational <U>> rats;
	Vector <T> reals;
	Vector <Complex <Rational <U>>> crats;
	Vector <Complex <T>> creals; */

	switch (st->kind()) {
	case l_operation:
		out = convert_operation(st, tbl);
		break;
	case l_variable_cluster:
		out = convert_variable_cluster(st, tbl);
		break;
	case l_matrix:
		for (stree *s : st->children()) {
			vec.clear();

			for (stree *t : s->children())
				vec.push_back(convert(t));

			mat.push_back(vec);
		}

		mn = l_none;
		for (auto row : mat) {
			for (node *nd : row) {
				if (nd->type == l_constant_real)
					fl_real = true;

				mn = max(mn, nd->type);
			}
		}

		if ((mn == l_constant_complex_rational) && fl_real)
			mn = l_constant_complex_real;
		
		switch (mn) {
		case l_constant_integer:
		case l_constant_rational:
			for (auto row : mat) {
				rats.clear();

				for (node *nd : row) {
					Rational <U> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();

					rats.push_back(tmp);
				}

				m_rats.push_back(rats);
			}
			
			out = new node {new opd_m_q(m_rats), l_constant_matrix_rational, {}};
			break;
		case l_constant_real:
			for (auto row : mat) {
				reals.clear();

				for (node *nd : row) {
					T tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();
					
					auto t3 = dynamic_cast <operand <T> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();

					reals.push_back(tmp);
				}

				m_reals.push_back(reals);
			}
			
			out = new node {new opd_m_r(m_reals), l_constant_matrix_real, {}};
			break;
		case l_constant_complex_rational:
			for (auto row : mat) {
				crats.clear();

				for (node *nd : row) {
					Complex <Rational <U>> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = Rational <U> (t1->get());
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();
					
					auto t3 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();

					crats.push_back(tmp);
				}

				m_crats.push_back(crats);
			}
			
			out = new node {new opd_m_cq(m_crats), l_constant_matrix_complex_rational, {}};
			break;
		case l_constant_complex_real:
			for (auto row : mat) {
				creals.clear();

				for (node *nd : row) {
					Complex <T> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = (T) t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = (T) t2->get();
					
					auto t3 = dynamic_cast <operand <T> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();
					
					auto t4 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
					if (t4 != nullptr)
						tmp = {(T) t4->get().real(), (T) t4->get().imag()};
					
					auto t5 = dynamic_cast <operand <Complex <T>> *> (nd->tok);
					if (t5 != nullptr)
						tmp = t5->get();

					creals.push_back(tmp);
				}

				m_creals.push_back(creals);
			}
			
			out = new node {new opd_m_cr(m_creals), l_constant_matrix_complex_real, {}};
			break;
		default:
			throw syntax_error("err");
		}

		break;
	case l_vector:
		for (stree *s : st->children())
			vec.push_back(convert(s));

		mn = l_none;
		for (node *nd : vec) {
			if (nd->type == l_constant_real)
				fl_real = true;

			mn = max(mn, nd->type);
		}

		if ((mn == l_constant_complex_rational) && fl_real)
			mn = l_constant_complex_real;

		// real & crat -> creal not crat

		// turn into a function(mn, vec), then return function(mn, vec)
		switch (mn) {
		case l_constant_integer:
		case l_constant_rational:
			for (node *nd : vec) {
				Rational <U> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();

				rats.push_back(tmp);
			}
			
			out = new node {new opd_v_q(rats), l_constant_vector_rational, {}};
			break;
		case l_constant_real:
			for (node *nd : vec) {
				T tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();
				
				auto t3 = dynamic_cast <operand <T> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();

				reals.push_back(tmp);
			}
			
			out = new node {new opd_v_r(reals), l_constant_vector_real, {}};
			break;
		case l_constant_complex_rational:
			for (node *nd : vec) {
				Complex <Rational <U>> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = Rational <U> (t1->get());
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();
				
				auto t3 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();

				crats.push_back(tmp);
			}
			
			out = new node {new opd_v_cq(crats), l_constant_vector_complex_rational, {}};
			break;
		case l_constant_complex_real:
			for (node *nd : vec) {
				Complex <T> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = (T) t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = (T) t2->get();
				
				auto t3 = dynamic_cast <operand <T> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();
				
				auto t4 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
				if (t4 != nullptr)
					tmp = {(T) t4->get().real(), (T) t4->get().imag()};
				
				auto t5 = dynamic_cast <operand <Complex <T>> *> (nd->tok);
				if (t5 != nullptr)
					tmp = t5->get();

				creals.push_back(tmp);
			}
			
			out = new node {new opd_v_cr(creals), l_constant_vector_complex_real, {}};
			break;
		default:
			throw syntax_error("err");
		}

		break;
	case l_complex_rational:
		iss = istringstream(st->str());

		iss >> num >> c >> den >> i;

		if (c == 'i')
			den = 1;
		else
			assert((i == 'i') && (c == '/'));

		out = new node {new opd_cq({0, {num, den}}),
			l_constant_complex_rational, {}};
		break;
	case l_complex_real:
		iss = istringstream(st->str());

		iss >> val >> i;

		assert(i == 'i');

		out = new node {new opd_cr ({0, val}),
			l_constant_complex_real, {}};
		break;
	case l_number_rational:
		iss = istringstream(st->str());

		iss >> num >> c >> den;

		if (c != '/')
			den = 1;

		out = new node {new opd_q ({num, den}),
			l_constant_rational, {}};
		break;
	case l_number_real:
		iss = istringstream(st->str());

		iss >> val;

		out = new node {new opd_r(val), l_constant_real, {}};
		break;
	case l_number_integer:
		iss = istringstream(st->str());

		iss >> num;

		out = new node {new opd_z(num), l_constant_integer, {}};
		break;
	default:
		break;
	}

	if (out)
		out->pars = pars;

	return out;
}

template <class T, class U>
node <T, U> *node <T, U> ::convert_operation(stree *st, vtable <T> tbl)
{
	/* if (st->str() == "sum")
		return convert_summation(st, tbl); */

	node *out = new node {new operation_holder(st->str()), {}};

	std::vector <std::type_index> fmt;
	
	node *cv;
	for (stree *s : st->children()) {
		cv = convert(s, tbl);

		if (cv) {
			out->leaves.push_back(cv);

			/* cout << "cv (from " << s << ")" << endl;
			cv->print();

			cout << "\t\t" << typeid(cv->tok).name() << endl;
			cout << "\t\t" << typeid(*(cv->tok)).name() << endl;
			cout << "\t\t" << typeid(opd_z).name() << endl;
			cout << "\t\t" << typeid(opd_r).name() << endl;
			cout << "\t\t" << typeid(opd_z *).name() << endl;
			cout << "\t\t" << typeid(opd_r *).name() << endl;

			if (typeid(opd_z *) == typeid(cv->tok))
				cout << "\tInteger" << endl;
			else if (typeid(opd_r *) == typeid(cv->tok))
				cout << "\tReal" << endl;
			else
				cout << "\tNone" << endl;

			fmt.push_back(typeid(*(cv->tok))); */
		}
	}

	/* token *tptr = brn.mkop(st->str(), fmt);

	* if (tptr == nullptr) {
		cout << "Failure to receive appropriate operand:" << endl;
		st->print();
		exit(0);
	} *

	// cout << "TOKEN: " << tptr->str() << " @ " << tptr << endl;

	out->tok = tptr; */

	return out;
}

/* template <class T, class U>
node <T, U> *node <T, U> ::convert_summation(stree *st, vtable <T> tbl)
{
	// Requires 4 operands
	// [remove asserts later]
	assert(st->children().size() == 4);

	string str = st->children()[0]->str();
	// string eqn = st->children()[3]->str();

	node *out = new node {get(st->str()), {
		new node {new var {str, true, T()}, {}},
		convert(st->children()[1]),
		convert(st->children()[2]),
		new node {new ftr {
			"f", 
			st->children()[3],
			{var {str, true, T()}},
			tbl},
		{}}
	}};

	return out;
} */

template <class T, class U>
node <T, U> *node <T, U> ::convert_variable_cluster(stree *st, vtable <T> vtbl)
{
	/* node *out;

	node *save;
	node *temp;
	node *in;
	
	out = new node {get(op_mul), {
		new node {new opd(1), l_none, {}},
		new node {new opd(1), l_none, {}}
	}};

	temp = out;

	int num = 0;

	std::string str = st->str();
	std::string acc;

	var vr;
	// ftr fr;

	for (int i = 0; i < str.length(); i++) {
		acc += str[i];

		auto vitr = find_if(pars.begin(), pars.end(),
			[&](const var &v) {
				return v.symbol() == acc;
			}
		);

		if (vitr != pars.end()) {
			out = new node {get(op_mul), l_none, {
				out,
				new node {new var {vitr->symbol(), true}, {}}
			}};
			
			for (stree *s : st->children()) {
				out = new node {get(op_mul), {
					out,
					convert(s, vtbl)
				}};
			}

			acc.clear();
			num++;
		}

		try {
			vr = vtbl.find(acc);
			
			out = new node {get(op_mul), {
				out,
				new node {new var(vr), {}}
			}};

			for (stree * s : st->children()) {
				out = new node {get(op_mul), {
					out,
					convert(s, vtbl)
				}};
			}

			acc.clear();
			num++;

			continue;
		} catch(...) {}
		
		* try {
			fr = tbl.find_ftr(acc);

			out = new node {get(op_mul), {
				out,
				new node {new ftr(fr), {}}
			}};

			for (stree *s : st->children())
				out->leaves[1]->leaves.push_back(convert(s, tbl));

			acc.clear();
			num++;

			continue;
		} catch(...) {} *
	}

	if (!num) {
		// cout << "Could not disassemble cluster: " << acc << endl;
		throw undefined_symbol(acc);
	}

	return out; */
}

//////////////////////////////////////////
// Special Nodes
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::special() const
{
	/* opn *optr;
	switch (tok->caller()) {
	case token::OPERATION:
		optr = dynamic_cast <opn *> (tok);
		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			return true;
		default:
			break;
		}

		break;
	default:
		break;
	} */

	return false;
}

//////////////////////////////////////////
// Labeling Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::label_as_operand()
{
	if (dynamic_cast <opd_r *> (tok) != nullptr)
		type = l_constant_real;
	else if (dynamic_cast <opd_z *> (tok) != nullptr)
		type = l_constant_integer;
	else if (dynamic_cast <opd_q *> (tok) != nullptr)
		type = l_constant_rational;
	else if (dynamic_cast <opd_cr *> (tok) != nullptr)
		type = l_constant_complex_real;
	else if (dynamic_cast <opd_cq *> (tok) != nullptr)
		type = l_constant_complex_rational;
	else
		type = l_constant;
}

template <class T, class U>
void node <T, U> ::label_as_operation()
{
	bool constant = true;
	for (node *nd : leaves) {
		switch (nd->type) {
		case l_constant:
		case l_constant_real:
		case l_constant_integer:
		case l_constant_rational:
		case l_constant_complex_real:
		case l_constant_complex_rational:
		case l_operation_constant:
			break;
		default:
			constant = false;
			break;
		}

		if (!constant)
			break;
	}
	
	if (constant) {
		// cout << "(const) this: " << this << endl;
		type = l_operation_constant;
		return;
	} else {
		// cout << "(non - const) this: " << this << endl;
	}

	/* opn *optr = dynamic_cast <opn *> (tok);
	switch (cfg_ptr->code(optr->fmt())) {
	case op_add:
	case op_sub:
		type = l_separable;
		break;
	case op_mul:
		type = l_multiplied;
		break;
	case op_div:
		type = l_divided;
		break;
	case op_exp:
		if (leaves[0]->type == l_constant
				|| leaves[0]->type == l_operation_constant)
			type = l_exponential;
		else if (leaves[1]->type == l_constant
				|| leaves[1]->type == l_operation_constant)
			type = l_power;
		break;
	case op_sin:
	case op_cos:
	case op_tan:
	case op_csc:
	case op_sec:
	case op_cot:
		type = l_trigonometric;
		break;
	case op_log:
		if (leaves[0]->type == l_constant)
			type = l_constant_logarithmic;
		else
			type = l_logarithmic;
		break;
	case op_fac:
		type = l_factorial;
		break;
	default:
		break;
	} */
}

template <class T, class U>
void node <T, U> ::label_as_special(const std::vector <std::string> &vars)
{
	// Early checking
	opn *optr = dynamic_cast <opn *> (tok);

	/* if (optr) {
		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			leaves[0]->type = l_summation_variable;
			leaves[1]->label(vars);
			leaves[2]->label(vars);
			leaves[3]->type = l_summation_function;
			type = l_summation;
			return;
		default:
			break;
		}
	} */
}

//////////////////////////////////////////
// Node Matching
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::matches_term(const node <T, U> &other) const
{
	std::vector <node> first;
	std::vector <node> second;

	queue <node> que;

	node current;

	if (type == l_multiplied)
		que.push(*this);
	else
		first.push_back(*this);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_multiplied:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			first.push_back(current);
			break;
		}
	}
	
	if (other.type == l_multiplied)
		que.push(other);
	else
		second.push_back(other);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_multiplied:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			second.push_back(current);
			break;
		}
	}

	std::vector <node> complete;

	for (auto nd : first) {
		for (auto itr = second.begin(); itr != second.end(); itr++) {
			if ((*nd.tok) == itr->tok) {
				complete.push_back(nd);
				second.erase(itr);
				break;
			}
		}
	}

	return ((first.size() == complete.size()) && second.empty());
}

//////////////////////////////////////////
// Compression Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::compress_as_separable()
{
	T val;
	T sign;

	/* if (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt()) == op_add)
		sign = cfg_ptr->one;
	else
		sign = cfg_ptr->negative;

	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();
		if (val == T(0)) {
			tok = get(op_mul);

			delete leaves[0]->tok;
			leaves[0]->tok = new opd(sign);
			
			label_all();
			compress();
		}
	} else if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();
		if (val == T(0)) {
			tok = get(op_mul);
			
			delete leaves[1]->tok;
			leaves[1]->tok = new opd(cfg_ptr->one);
			
			label_all();
			compress();
		}
	} */
}

template <class T, class U>
void node <T, U> ::compress_as_multiplied()
{
	/* cout << string(50, '_') << endl;
	cout << "PRE-COMPRESSION:" << endl; 
	print(); */

	/* put the following ds
	 into a single augumented
	 data structure */
	/* std::unordered_map <std::string, T> chart;
	std::vector <node *> misc;
	std::queue <node *> que;
	std::string name;
	
	std::vector <token *> vals;

	opn *optr = get(op_mul);
	opd *constant = new opd(1);
	node *temp, *t;
	node *current;

	que.push(this);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current->type) {
		case l_multiplied:
			que.push(current->leaves[0]);
			que.push(current->leaves[1]);
			break;
		case l_variable:
			name = (dynamic_cast <var*> (current->tok))->symbol();
			if (chart.find(name) == chart.end())
				chart[name] = T(0);
			chart[name] += T(1);
			break;
		case l_operation_constant:
			vals = {
				new opd(current->value()),
				constant
			};

			constant = dynamic_cast <opd *> ((*optr)(vals));

			vals.clear();

			break;
		case l_constant:
			vals = {
				constant,
				current->tok
			};

			constant = dynamic_cast <opd *> ((*optr)(vals));

			vals.clear();

			break;
		default:
			misc.push_back(current);
			break;
		}
	}

	for (size_t i = 0; i < misc.size(); i++)
		misc[i]->compress();

	tok = nullptr;
	if (constant->get() == T(0)) {
		set(new opd(0), l_constant, {});
		return;
	}

	if (constant->get() != T(1))
		set(constant, l_constant, {});

	for (auto itr : chart) {
		if (tok)
			temp = copy();

		if (itr.second == T(1)) {
			t = new node(new var(itr.first, true), {});
		} else {
			t = new node(get(op_exp), {
				new node(new var(itr.first, true), {}),
				new node(new opd(itr.second), {})
			});
		}

		if (tok) {
			set(cfg_ptr->alloc_opn(op_mul), {temp, t});
		} else {
			set(t);
		}
	}

	for (auto itr : misc) {
		if (tok) {	
			temp = copy();
			set(cfg_ptr->alloc_opn(op_mul), {temp, itr});
		} else {
			set(itr->tok, itr->leaves);
		}
	}
	
	* cout << "POST-COMPRESSION:" << endl; 
	print(); */
}

template <class T, class U>
void node <T, U> ::compress_as_divided()
{
	T val;
	
	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();

		if (val == T(1)) {
			delete tok;
			tok = get(op_exp);

			leaves[0] = leaves[1]->copy();

			delete leaves[1];
			leaves[1] = new node(new opd(-1), {});
		} else if (val == T(0)) {
			set(new opd(0), {});
		}
	} else if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();

		/* if (val == cfg_ptr->one) {
			set(leaves[0]);
		} */
		/* else if (val == 0) <- throw zero division exception
			set(new opd(1), {}); */
	}
}

template <class T, class U>
void node <T, U> ::compress_as_power()
{
	T val;

	node *lr;
	node *cpy;
	if (leaves[0]->type == l_multiplied) {
		cpy = copy();

		lr = leaves[0]->leaves[1];

		cpy->leaves[0] = lr;
		leaves[0]->leaves[1] = cpy;
		leaves[1] = leaves[0]->leaves[1];
		leaves[0] = leaves[0]->leaves[0];
		tok = get(op_mul);

		leaves[0]->compress();
		leaves[1]->compress();
		
		for (node *nd : leaves)
			nd->compress();
		
		return;
	}

	for (node *nd : leaves)
		nd->compress();
	
	if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();
		if (val == T(1))
			set(leaves[0]);
		else if (val == T(0))
			set(new opd(1), {});
	}
}

template <class T, class U>
void node <T, U> ::compress_as_exponential()
{
	T val;

	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();
		if (val == T(1))
			set(new opd(1), {});
		else if (val == 0)
			set(new opd(0), {});
	}
}

//////////////////////////////////////////
// Differentiation Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::differentiate_as_multiplied(const std::string &var)
{
	node *lcpy;
	node *rcpy;

	tok = get(op_add);

	lcpy = leaves[0]->copy();
	lcpy->differentiate(var);

	rcpy = leaves[1]->copy();
	rcpy->differentiate(var);
	
	leaves = {
		new node(get(op_mul), {lcpy, leaves[1]}),
		new node(get(op_mul), {leaves[0], rcpy}),
	};
}

template <class T, class U>
void node <T, U> ::differentiate_as_divided(const std::string &var)
{
	node *denom = leaves[1]->copy();
	node *lcpy = leaves[0]->copy();
	node *rcpy = leaves[1]->copy();

	lcpy->differentiate({var});
	rcpy->differentiate({var});
	
	tok = get(op_div);

	std::vector <node *> lv_cpy = {
		new node(get(op_sub), {
			new node(get(op_mul), {lcpy, leaves[1]}),
			new node(get(op_mul), {leaves[0], rcpy}),
		}),

		new node(get(op_exp), {
				denom,
				new node(new opd(2), {})
		}),
	};

	leaves = lv_cpy;
}

template <class T, class U>
void node <T, U> ::differentiate_as_power(const std::string &var)
{
	T val = (dynamic_cast <opd *> (leaves[1]->tok))->get();

	node *done = leaves[0]->copy();
	done->differentiate({var});
	
	// delete tok;
	tok = get(op_mul);

	// delete leaves[1];
	leaves[1] = new node(get(op_exp), {
			leaves[0],
			new node(new opd(val - 1), {})
	});
	
	// delete leaves[0];
	leaves[0] = new node(get(op_mul), {
			new node(new opd(val), {}),
			done
	});
}

template <class T, class U>
void node <T, U> ::differentiate_as_exponential(const std::string &var)
{
	node *cpy;

	cpy = leaves[1]->copy();
	cpy->differentiate(var);

	T val = (dynamic_cast <opd *> (leaves[0]->tok))->get();

	set(get(op_mul), {
		cpy,
		new node {get(op_mul), {
			new node {new opd(log(val)), {}},
			new node {get(op_exp), {
				leaves[0],
				leaves[1]
			}}
		}}
	});
}

template <class T, class U>
void node <T, U> ::differentiate_as_trigonometric(const std::string &var)
{
	node *cpy;
	
	cpy = leaves[0]->copy();
	cpy->differentiate(var);

	/* switch (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt())) {
	case op_sin:
		set(get(op_mul), {
			cpy,
			new node {get(op_cos), {
				leaves[0]
			}},
		});

		break;
	case op_cos:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_sin), {
					leaves[0]
				}},
			}}
		});
		break;
	case op_tan:
		set(get(op_mul), {
			cpy,
			new node {get(op_exp), {
				new node {get(op_sec), {
					leaves[0],
				}},
				new node {new opd(2), {}}
			}}
		});
		break;
	case op_csc:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_mul), {
					new node {get(op_csc), {
						leaves[0]->copy()
					}},
					new node {get(op_cot), {
						leaves[0]
					}}
				}}
			}}
		});
		break;
	case op_sec:
		set(get(op_mul), {
			cpy,
			new node {get(op_mul), {
				new node {get(op_sec), {
					leaves[0]->copy()
				}},
				new node {get(op_tan), {
					leaves[0]
				}}
			}}
		});
		break;
	case op_cot:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_exp), {
					new node {get(op_csc), {
						leaves[0],
					}},
					new node {new opd(2), {}}
				}}
			}}
		});
		break;
	default:
		break;
	} */
}

template <class T, class U>
void node <T, U> ::differentiate_as_logarithmic(const std::string &var)
{
}

template <class T, class U>
void node <T, U> ::differentiate_as_constant_logarithmic(const std::string &var)
{
	T val = log(leaves[0]->value());

	node *cpy = leaves[1]->copy();
	cpy->differentiate(var);

	set(get(op_div), {
		cpy,
		new node {get(op_mul), {
			new node {new opd(val), {}},
			leaves[1]
		}},
	});
}

/* template <class T, class U>
void node <T, U> ::differentiate_as_function(const std::string &var)
{
	// Move using declaration to class
	ftr *ft = dynamic_cast <ftr *> (tok);

	// Remove asserts later
	assert(ft->ins() == leaves.size() && leaves.size() > 0);

	std::vector <node *> terms;

	node *out;
	node *cpy;
	node *lcp;
	
	ftr tmp;

	// First Node
	tmp = ft->differentiate((*ft)[0].symbol());
	
	lcp = copy();
	lcp->retokenize(new ftr(tmp));

	cpy = leaves[0]->copy();

	cpy->differentiate(var);
	// cpy->label_all();

	out = new node {get(op_add), {
		new node {get(op_mul), {
			lcp,
			cpy
		}},
		new node {new opd(0), l_none, {}}
	}};

	// Other parameters
	for (int i = 1; i < ft->ins(); i++) {
		tmp = ft->differentiate((*ft)[i].symbol());

		lcp = copy();

		lcp->retokenize(new ftr(tmp));

		cpy = leaves[i]->copy();

		cpy->differentiate(var);
		// cpy->label_all();

		* cout << "\t#" << (i + 1) << endl;
		
		cout << "\t\t(a): " << tmp << endl;
		cout << "\t\t(b): " << cpy->display() << endl; *

		out = new node {get(op_add), {
			new node {get(op_mul), {
				lcp,
				cpy
			}},
			out
		}};
	}

	// cout << endl;

	*this = out->copy();
} */

//////////////////////////////////////////
// Display Methods
//////////////////////////////////////////

template <class T, class U>
std::string node <T, U> ::display_as_operand(nd_label required) const
{
	std::string out = display();

	switch (required) {
	case l_trigonometric:
		if (type == l_separable
			|| type == l_multiplied)
			out = "(" + out + ")";
		break;
	case l_exp:
	case l_power:
	case l_exponential:
		if (type != l_constant
			&& type != l_variable)
			out = "(" + out + ")";
		break;
	case l_multiplied:
	case l_divided:
		if (type == l_separable)
			out = "(" + out + ")";
		break;
	default:
		break;
	}

	return out;
}

template <class T, class U>
std::string node <T, U> ::display_as_trigonometric() const
{
	std::string stropn;
	std::string space = " ";

	/* stropn = leaves[0]->display_as_operand(l_trigonometric);

	if (stropn[0] == '(')
		space = "";

	switch (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt())) {
	case op_sin:
		return "sin" + space + stropn;
	case op_cos:
		return "cos" + space + stropn;
	case op_tan:
		return "tan" + space + stropn;
	case op_csc:
		return "csc" + space + stropn;
	case op_sec:
		return "sec" + space + stropn;
	case op_cot:
		return "cot" + space + stropn;
	} */

	throw node_error("Node labeled as trigonometric, \
		but token is of an undetecvtable type");
}

/* template <class T, class U>
std::string node <T, U> ::display_as_function() const
{
	std::string out = (dynamic_cast <ftr *> (tok))->symbol();

	out += "(";

	for (size_t i = 0; i < leaves.size(); i++) {
		out += leaves[i]->display();

		if (i < leaves.size() - 1)
			out += ", ";
	}

	out += ")";

	return out;
} */

#endif
