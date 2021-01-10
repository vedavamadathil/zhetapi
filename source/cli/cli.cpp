// C/C++ headers
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <functional>

#include <ncurses.h>

// Engine headers
#include <zhplib.hpp>

#include <expression.hpp>

using namespace std;
using namespace zhetapi;

// Typedefs
typedef zhetapi::Operand <int> z;
typedef zhetapi::Operand <Rational <int>> q;
typedef zhetapi::Operand <double> r;

// Barn for variables and functions
zhetapi::Barn barn;

// Answer variable
zhetapi::Variable ans(new z(0), "$");

// List of commands
map <string, pair <string, function <void ()>>> cmds;

// Inserting variables into barn
void insert(const string &name, const string &expr)
{
	zhetapi::node_manager mgr(expr, barn);

	zhetapi::Token *tptr = mgr.value();

	z *o_z = dynamic_cast <z *> (tptr);
	if (o_z) {
		barn.put(name, o_z->get());
		cout << "\n\t" << tptr->str() << "\n\n";
		return;
	}
	
	q *o_q = dynamic_cast <q *> (tptr);
	if (o_q) {
		barn.put(name, o_q->get());
		cout << "\n\t" << tptr->str() << "\n\n";
		return;
	}
	
	r *o_r = dynamic_cast <r *> (tptr);
	if (o_r) {
		barn.put(name, o_r->get());
		cout << "\n\t" << tptr->str() << "\n\n";
		return;
	}
}

// Parsing
void parse(const string &str)
{
	size_t count;
	size_t index;

	count = 0;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '=') {
			++count;

			index = i;
		}
	}
	
	if (count == 0) {
		try {
			zhetapi::node_manager mgr(str, barn);

			zhetapi::Token *tptr = mgr.value();

			cout << "\n\t" << tptr->str() << "\n\n";
			
			barn.put(tptr, "$");
			
			delete tptr;
		} catch (zhetapi::node_manager::undefined_symbol e) {
			cout << "\n\t" << e.what() << "\n\n";
		}
	} else if (count == 1) {
		try {
			zhetapi::Function tmp = std::string(str);

			barn.put(tmp);

			return;
		} catch(...) {}

		string var = str.substr(0, index);
		string expr = str.substr(index + 1);

		istringstream iss(var);

		iss >> var;

		insert(var, expr);
	} else {
		cout << "\tError: invalid syntax" << endl;
	}
}

// Commands
void command(std::string cmd)
{
	if (cmds.find(cmd) != cmds.end())
		(cmds[cmd].second)();
	else
		cout << "\n\tUnknown command \"" << cmd << "\"\n\n";
}

namespace cmd {

	void quit()
	{
		exit(0);
	}

	void save()
	{
		barn.print();
	}

	void list()
	{
		cout << "\n\tCommand summary:\n";
		for (auto pr : cmds)
			cout << "\t\t" << pr.first << "\t" << pr.second.first << endl;
		cout << "\n";
	}

	void symbols()
	{
		cout << "\n";
		barn.list();
		cout << "\n";
	}

}

int main()
{
	// Filling out barn
	barn.put(Variable(new Operand <double> (exp(1)), "e"));
	barn.put(Variable(new Operand <double> (acos(-1)), "pi"));

	// Ans
	barn.put(ans);

	// Commands
	cmds["q"] = {"Quit the CLI", cmd::quit};
	cmds["quit"] = {"Quit the CLI", cmd::quit};
	cmds["list"] = {"Lists all available commands", cmd::list};
	cmds["symbols"] = {"Lists all current symbols", cmd::symbols};

	// Input
	string line;
	while (true) {
		cout << "(zhetapi-cli) ";

		getline(cin, line);

		if (!line.empty() && line[0] == '#')
			command(line.substr(1));
		else if (!line.empty())
			parse(line);

		zhetapi::Function::barn = barn;
	}
}
