#include "global.hpp"

// Zhetapi API storage
Engine *engine = new Engine();

Token *execute(string str)
{
	// Skip comments
	if (str[0] == '#')
		return nullptr;
	
	// cout << "line = " << line << ", STR = " << str << endl;

	vector <string> tmp = split(str);
	
	size_t tsize = tmp.size();

	if (tsize > 1) {
		zhetapi::Token *tptr = nullptr;
		
		node_manager::undefined_symbol us("");
		bool pe = false;

		try {
			zhetapi::node_manager mg(tmp[tsize - 1], engine);

			tptr = mg.value();
		} catch (const Engine::unknown_op_overload &e)  {
			cout << "err: " << e.what() << endl;
			exit(-1);
		} catch (const node_manager::undefined_symbol &e) {
			us = e;
			pe = true;
		}

		for (int i = tsize - 2; i >= 0; i--) {
			string ftr = tmp[i] + " = " + tmp[tsize - 1];

			try {
				zhetapi::Function f(ftr, engine);

				engine->put(f);
			} catch (...) {
				if (pe) {
					cout << "err:" << us.what() << endl;
					exit(-1);
				}

				engine->put(tmp[i], tptr);
			}
		}
		
		delete tptr;
	} else {		
		// All functions and algorithms are stored in engine
		node_manager mg;
	
		/* cout << "BARN:" << endl;
		engine.print(); */

		Token *tptr = nullptr;

		try {
			mg = node_manager(str, engine);
			
			tptr = mg.value();
		} catch (const Engine::unknown_op_overload &e)  {
			cout << "err evaluating \'" << str << "\'\n" << e.what() << endl;
			exit(-1);
		} catch (const node_manager::undefined_symbol &e) {
			cout << "Error at line " << line
				<< ": the symbol \""
				<< e.what() << "\" was not defined "
				"in this scope" << endl;

			exit(-1);
		}

		/* cout << "mg:" << endl;
		mg.print(); */

		// "Execute" the statement
		return tptr;
	}

	return nullptr;
}
