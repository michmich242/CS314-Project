#include "../include/main.h"
#include "../include/manager.h"

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
	int check {0};

	while(check != 3){
		std::cout << "1. Member Manager Terminal" << std::endl;
		std::cout << "2. Provider Manager Terminal" << std::endl;
		std::cout << "3. Exit Program" << std::endl;
		std::cout << "Enter your option (1 - 3): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		if(check == 1){
			Member Manager_Member;
			Manager_Member.display_Member_Menu();
		}

	}



	return true;
}

bool
start_provider()
{
	return true;
}
