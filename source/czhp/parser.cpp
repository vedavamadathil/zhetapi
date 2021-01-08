#include "global.hpp"

#define __lineup(c) 		\
	if (c == '\n')		\
		line++;

#define __skip_space()		\
	while (isspace(c = getchar()));

#define __skip_to_char(s)	\
	while ((c = getchar()) != s);

static inline bool __file_exists(string file)
{
	if (FILE *f = fopen(file.c_str(), "r")) {
		fclose(f);

		return true;
	} else {
		return false;
	} 
}

static inline string __get_dir(string file)
{
	size_t i = file.length() - 1;
	while (file[i--] != '/');

	return file.substr(0, i + 2);
}

// Global scope code
vector <string> global;

// Constants
Operand <bool> *op_true = new Operand <bool> (true);
Operand <bool> *op_false = new Operand <bool> (false);

size_t line = 1;
string file = "";

static int parse_parenthesized(string &parenthesized)
{
	char c;

	while (isspace(c = getchar())) {
		__lineup(c);
	}

	if (c != '(')
		return -1;

	while ((c = getchar()) != ')')
		parenthesized += c;

	return 0;
}

static int extract_block(string &block)
{
	char c;
	
	__skip_space();
	// while (isspace(c = getchar()));

	if (c == '{') {
		while ((c = getchar()) != '}')
			block += c;
	} else {
		fseek(stdin, -1, SEEK_CUR);

		while ((c = getchar()) != '\n')
			block += c;

		__lineup(c);
	}

	return 0;
}

static int parse_block()
{
	char c;
	
	while (isspace(c = getchar())) {
		__lineup(c);
	}

	if (c == '{') {
		parse('}');
	} else {
		fseek(stdin, -1, SEEK_CUR);

		parse('\n');

		__lineup(c);
	}

	return 0;
}

static int parse_block_ignore()
{
	char c;
	
	while (isspace(c = getchar())) {
		__lineup(c);
	}

	if (c == '{') {
		while ((c = getchar()) != '}');
	} else {
		fseek(stdin, -1, SEEK_CUR);

		while ((c = getchar()) != '\n' && c != EOF);

		__lineup(c);
	}

	return 0;
}

static int parse_function(string &ident, vector <string> &params)
{
	char c;

	__skip_space();
	
	fseek(stdin, -1, SEEK_CUR);

	while ((c = getchar()) != '(')
		ident += c;

	string tmp;
	while ((c = getchar()) != ')') {
		if (c == ',') {
			if (!tmp.empty()) {
				params.push_back(tmp);

				tmp.clear();
			}
		} else if (!isspace(c)) {
			tmp += c;
		}
	}

	if (!tmp.empty())
		params.push_back(tmp);

	return 0;
}

bool if_prev = false;
bool if_true = false;

static void check(string &keyword)
{
	string parenthesized;
	string block;
	string library;

	if (keyword == "if") {
		if (parse_parenthesized(parenthesized)) {
			printf("Syntax error at line %lu: missing parenthesis after an if\n", line);
			exit(-1);
		}

		if_prev = true;

		Token *t = execute(parenthesized);

		if (*t == op_true) {
			if_true = true;

			parse_block();
		} else {
			if_true = false;

			parse_block_ignore();
		}
		
		keyword.clear();
	}

	if (keyword == "elif") {
		if (!if_prev) {
			printf("Error at line %lu: need an if before an elif\n", line);

			exit(-1);
		}

		if (parse_parenthesized(parenthesized)) {
			printf("Syntax error at line %lu: missing parenthesis after an if\n", line);
			exit(-1);
		}

		Token *t = execute(parenthesized);

		if (*t == op_true) {
			if_true = true;

			parse_block();
		} else {
			if_true = false;

			parse_block_ignore();
		}
		
		keyword.clear();
	}

	if (keyword == "else") {
		if (!if_prev) {
			printf("Error at line %lu: need an if before an else\n", line);

			exit(-1);
		}
		
		if_prev  = false;

		if (if_true)
			parse_block_ignore();
		else
			parse_block();
		
		keyword.clear();
	}

	if (keyword == "while") {
		if (parse_parenthesized(parenthesized)) {
			printf("Syntax error at line %lu: missing parenthesis after a while\n", line);
			exit(-1);
		}
		
		if (extract_block(block)) {
			printf("Syntax error at line %lu: missing statement after a while\n", line);
			exit(-1);
		}

		Token *t = execute(parenthesized);
		while (*t == op_true) {
			parse(block);
			
			t = execute(parenthesized);
		}
		
		keyword.clear();
	}

	if (keyword == "include") {
		cin >> library;

		// Check current dir
		string current_dir = "./" + library + ".zhplib";
		string script_dir = __get_dir(file) + library + ".zhplib";

		// For now only import *.zhplib
		if (__file_exists(current_dir))
			import_library(current_dir);
		else if (__file_exists(script_dir))
			import_library(script_dir);
		else
			printf("Error at line %lu: could not import library '%s'\n", line, library.c_str());

		keyword.clear();
	}

	if (keyword == "alg") {
		cout << "Algorithm!" << endl;

		string ident;

		vector <string> params;

		if (parse_function(ident, params)) {
			cout << "Failed to parse function..." << endl;

			exit(-1);
		} else {
			cout << "ident = " << ident << endl;
			cout << "params:" << endl;
			for (auto i : params)
				cout << "\t\"" << i << "\"" << endl;
		}

		keyword.clear();
	}
}

// Parsing machine
int parse(char ex)
{
	static bool quoted = false;
	static int paren = 0;

	string tmp;
	char c;

	while ((c = getchar()) != ex) {
		if (!quoted) {
			if (c == '\"')
				quoted = true;
			if (c == '(')
				paren++;
			if (c == ')')
				paren--;
			
			if (c == '\n' || (!paren && c == ',')) {
				if (!tmp.empty()) {
					execute(tmp);

					tmp.clear();
				}
			} else if (!isspace(c)) {
				tmp += c;
			}
		} else {
			if (c == '\"')
				quoted = false;
			
			tmp += c;
		}

		__lineup(c);

		// cout << "tmp = " << tmp << endl;
		check(tmp);
	}

	// Flush last instruction
	if (!tmp.empty()) {
		execute(tmp);

		tmp.clear();
	}

	return 0;
}

// Parsing machine
int parse(string str)
{
	static bool quoted = false;
	static int paren = 0;

	string tmp;
	char c;

	for (size_t i = 0; i < str.length(); i++) {
		c = str[i];

		if (!quoted) {
			if (c == '\"')
				quoted = true;
			if (c == '(')
				paren++;
			if (c == ')' && paren)
				paren--;
			
			if (c == '\n' || (!paren && c == ',')) {
				if (!tmp.empty()) {
					// cout << "executing tmp = " << tmp << endl;
					execute(tmp);

					tmp.clear();
				}
			} else if (!isspace(c)) {
				tmp += c;
			}
		} else {
			if (c == '\"')
				quoted = false;
			
			tmp += c;
		}

		__lineup(c);

		// cout << "tmp = " << tmp << endl;
		check(tmp);
	}

	// Flush last instruction
	if (!tmp.empty()) {
		execute(tmp);

		tmp.clear();
	}

	return 0;
}

// Splitting equalities
vector <string> split(string str)
{
	bool quoted = false;

	char pc = 0;

	vector <string> out;
	size_t n;

	n = str.length();

	string tmp;
	for (size_t i = 0; i < n; i++) {
		if (!quoted) {
			bool ignore = false;

			if (pc == '>' || pc == '<' || pc == '!'
				|| (i > 0 && str[i - 1] == '='))
				ignore = true;
			
			if (!ignore && str[i] == '=') {
				if (i < n - 1 && str[i + 1] == '=') {
					tmp += "==";
				} else if (!tmp.empty()) {
					out.push_back(tmp);

					tmp.clear();
				}
			} else {
				if (str[i] == '\"')
					quoted = true;
				
				tmp += str[i];
			}
		} else {
			if (str[i] == '\"')
				quoted = false;
			
			tmp += str[i];
		}

		pc = str[i];
	}

	if (!tmp.empty())
		out.push_back(tmp);
	
	/* cout << "split:" << endl;
	for (auto s : out)
		cout << "\ts = " << s << endl; */

	return out;
}
