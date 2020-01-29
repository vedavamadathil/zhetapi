#ifndef PARSER_H
#define PARSER_H

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

// Custom Built Libraries
#include "debug.h"
#include "token.h"
#include "operand.h"
#include "operation.h"
#include "defaults.h"
#include "token_tree.h"

namespace tokens {
	// Beginning of the parser class
	template <class data_t>
	class parser : public token {
		/* The following are states of parsing
		 * the expressions, etc. Immediate resolution
		 * is carried out */
		enum STATES {NORM, PAREN, OPERATION};
	public:
		/* The following are static member functions that
		 * give purpose to the tokens
		 *
		 * const token &get_next( std::string, std::size_t):
		 *   returns the next valid token in the passed
		 *   string from the specified index, or throws an
		 *   error if no token was detected and modifies the
                 *   passed index value appropriately
		 */
		static std::pair <token *, std::size_t> get_next(std::string,
			std::size_t) noexcept(false);
		static std::vector <token *> get_tokens(std::string);

                // Returns the index of the operation who's format
                // matches the format this passed, and none_op if none
                static std::size_t get_matching(std::string);

                // Always check to make sure
                // operand <data_t> is an operand
                type caller() const override;
                std::string str() const override;
	};

	

        // Parser's parsing functions
        template <typename data_t>
        std::pair <token *, std::size_t> parser <data_t> ::get_next(std::string
			input, std::size_t index)
        {
		// Current state of parsing
		STATES state = NORM;

                // Add parenthesis (groups)
                // after basic operand parsing
                // is working
                std::size_t i;
                char c;

                // Accumulated string
                std::string cumul;
		std::string paren;

                std::istringstream ss(input);
                std::size_t opn_index;

                operand <data_t> oper;
		
		trees::token_tree <data_t> tr;

                ss.seekg(index);

                for (i = index; i < input.length(); i++) {
                        c = input[i];

			switch(c) {
			case '(':
				state = PAREN;
				break;
			case ')':
				// Throw error
				std::cout << "Error - Missing \
					opening parenthesis" << std::endl;
				return {nullptr, -1};
			}

			if (state == PAREN) {
				// Ignore the parenthesis
				ss.seekg(i + 1);
				while (ss >> c) {
					if (c == ')') {
						// dp_var(ss.tellg());
						// std::cout << "paren: " << paren << std::endl;
						// std::cout << "tellg(): " << ss.tellg() << std::endl;
						tr = trees::token_tree <data_t> (paren);

						if (ss.tellg() >= input.length())
							return {tr.value()->dptr->get_oper(),
								-1};
						return {tr.value()->dptr->get_oper(),
							ss.tellg()};
					}
					paren += c;
				}
			}

                        if (std::isdigit(c)) {
				// dp_var(c)
				// dp_var(ss.tellg())
                                ss >> oper;
                                return {new operand <data_t> (oper),
					ss.tellg()};
                        }

			// isspace is ok because we already
			// took care of parenthesis (add
			// function sensetivity to parentheses)
			// if (state == OPERATION && isspace(c)) {
			// 	state = NORM;

				
			// }

                        // c is the start of an
			// operation or function
                        if (!std::isspace(c)) {
				//state = OPERATION;
                                cumul += c;
			}

			// dp_msg("got here")
			opn_index = get_matching(cumul);
			// dp_msg("lost after")
			if (opn_index != defaults <data_t> ::NONOP)
				return {&defaults <data_t> ::opers[opn_index], i + 1};
                }

                return {nullptr, -1};
        }

	template <typename data_t>
	std::vector <token *> parser <data_t> ::get_tokens(std::string input)
	{
		std::pair <token *, std::size_t> opair;
		std::vector <token *> tokens;
		std::size_t index = 0;
		int ses_len = 5;

		std::cout << std::string(10, '*') << std::endl;
		for (int i = 0; i < input.length(); i++)
			std::cout << i << "\t" << input[i] << std::endl;
		std::cout << std::endl;
		// dp_var(input)

		while (true) {
			opair = get_next(input, index);

			dp_var(opair.first->str());
			dp_var(opair.second)

                        if (opair.second == UINT64_MAX) {
				// dp_var(input);
				// dp_var(opair.first);
				// dp_var(opair.second);
                                tokens.push_back(opair.first);
                                break;
                        }

			tokens.push_back(opair.first);
			index = opair.second;
		}

		// stl_reveal <token *> (tokens, [](token *t) {return t->str();});

		// std::cout << "Returning" << std::endl;
		std::cout << std::string(5, '*') << std::endl;

		return tokens;
	}

        template <typename data_t>
        std::size_t parser <data_t> ::get_matching(std::string str)
        {
		// dp_var(str)
		// dp_msg("in matching process")
                for (int i = 0; i < defaults <data_t> ::NOPERS; i++) {
			// dp_msg("looping")
			//dp_var(i)
			//dp_var(defaults <data_t> ::opers[i].str())
                        if (defaults <data_t> ::opers[i].matches(str))
                                return i;
                }

		// dp_msg("returning")

                return defaults <data_t> ::NONOP;
        }

        // Derived member functions
        template <typename data_t>
        token::type parser <data_t> ::caller() const
        {
                return PARSER;
        }

        template <typename data_t>
        std::string parser <data_t> ::str() const
        {
                // Add some more description
                // to the returned string
                return "parser";
        }
}

#endif
