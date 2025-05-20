#include "../include/main.h"

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 2) {
		std::cerr
			<< "Please put a 1 or a 0 for manager or provider mode! Only one option on the command line please!\n";
		std::cerr << "\t./" << argv[0] << " 0 - run in manager mode\n";
		std::cerr << "\t./" << argv[0] << " 1 - run in provider mode\n";
		return 1;
	}


	if (argv[1][0] == '0') {
		start_provider();
	}
	else if (argv[1][0] == '1') {
		start_manager();
	}
	else {
		std::cerr << "Use only 1 or 0 for the command line options!\n";
		return 1;
	}
	return 0;
}

bool
start_manager()
{
	return true;
}

bool
start_provider()
{
	return true;
}
