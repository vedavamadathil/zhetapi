#ifndef APPLICATION_H_
#define APPLICATION_H_

// C++ Standard Libraries
#include <string>

// remove later (replace with formatter)
#include <iostream>

using namespace std;

// Custom built Libraries
#include "expression.h"
#include "var_stack.h"
#include "formatter.h"
#include "assignment.h"

/**
 * @brief need another parameter for formatter
 */
template <class T>
class application {
public:
	enum m_type {
		ASSIGNMENT,
		EQUALITY,
		EXPRESSION
	};

	application(std::string = "");

	void operator()(var_stack <T> &) const;
private:
	std::string m_input;
	m_type m_kind;
};

/**
 * @brief testing
 */
template <class T>
application <T> ::application(std::string str) :
	m_input(str)
{
	if (m_input.find('=') == std::string::npos)
		m_kind = EXPRESSION;
	else
		m_kind = ASSIGNMENT;
}

/**
 * @brief add formatter parameter later
 */
template <class T>
void application <T> ::operator()(var_stack<T> &vst) const
{
	switch (m_kind) {
	case ASSIGNMENT:
		assignment <T> ::resolve(m_input, vst);
		break;
	case EXPRESSION:
		cout << "\t" << expression <T> ::in_place_evaluate(m_input, vst) << endl;
		break;
	default:
		// Type not supported
		cout << "Input type not supported yet" << endl;
		break;
	}
}

#endif