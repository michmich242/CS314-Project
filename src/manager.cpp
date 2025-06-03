#include "../include/manager.h"
#include "../include/sqlengine.h"
#include "../include/utils.h"
#include <regex>

// MANAGER TERMINAL FLOW DRIVERS
//
//
bool
Manager::start_manager()
{
	int check{0};

	while (check != 3) {
		std::cout << "1. Member Manager Terminal" << std::endl;
		std::cout << "2. Provider Manager Terminal" << std::endl;
		std::cout << "3. Exit Program" << std::endl;
		std::cout << "Enter your option (1 - 3): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		if (check == 1) {
			display_Member_Menu();
		}
		else if(check == 2){
			display_Provider_Menu();
		}
	}


	return true;
}

void
Manager::display_Member_Menu()
{
	int check{0};
	while (check != 4) {
		std::cout << "1. Add Member\n";
		std::cout << "2. Update Member\n";
		std::cout << "3. Remove Member\n";
		std::cout << "4. Quit to Main Manager Menu\n";
		std::cout << "Enter your option (1 - 4): ";

		std::cin >> check;
		std::cin.ignore(100, '\n');

		if (check == 1) {
			add_member();
		}
		else if (check == 2) {
			update_member();
		}
		else if (check == 3){
			delete_member();
		}
	}
}

//
//
// END OF FLOW DRIVERS

bool
Manager::add_member()
{
	if (!db.is_connected()) {
		return false;
	}

	Member member;
	get_valid_member_input(member);
	return db.add_member(member);
}

bool
Manager::update_member()
{

	std::string input_id;
	Member member;
	try {

		std::regex nine_digits("^\\d{9}$");
		while (!(std::regex_match(input_id, nine_digits))) {
			std::cout << "Enter a 9 digit member ID: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}
		member.set_ID(input_id);
		std::cout << member.get_ID() << "\n";

		if (!db.get_member(member)) {
			std::cerr << "No member with given ID";
			return false;
		}
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	member.Display_Member_Info();

	std::cout << "Enter new the updated values for the following\n\n";

	get_valid_member_input(member);
	return db.update_member(member);
}

// PRIVATE UTILS
//
//
void
Manager::get_valid_member_input(Member &member)
{
	// Helper lambda

	auto get_input = [](const std::string &prompt, const std::regex &pattern, int max_len = -1) {
		std::string input;
		while (true) {
			std::cout << prompt;
			std::getline(std::cin, input);
			if ((max_len == -1 || input.length() <= static_cast<size_t>(max_len)) && std::regex_match(input, pattern)) {
				return input;
			}
			std::cout << "Invalid input. Please try again.\n";
		}
	};

	member.set_name(get_input("Enter member name (max 25 characters): ", std::regex("^.{1,25}$")));
	member.set_address(get_input("Enter street address (max 25 characters): ", std::regex("^.{1,25}$")));
	member.set_city(get_input("Enter city (max 14 characters): ", std::regex("^.{1,14}$")));
	member.set_state(get_input("Enter state (2 letters): ", std::regex("^[A-Za-z]{2}$")));
	member.set_zip(get_input("Enter ZIP code (5 digits): ", std::regex("^\\d{5}$")));
}


bool Manager::delete_member(){

	std::string input_id;

	try {

		std::regex nine_digits("^\\d{9}$");
		while (!(std::regex_match(input_id, nine_digits))) {
			std::cout << "Enter a 9 digit member ID: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}

	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	return db.delete_member(input_id);


}



void Manager::get_valid_provider_input(Provider & provider){
	
	auto get_input = [](const std::string &prompt, const std::regex &pattern, int max_len = -1) {
		std::string input;
		while (true) {
			std::cout << prompt;
			std::getline(std::cin, input);
			if ((max_len == -1 || input.length() <= static_cast<size_t>(max_len)) && std::regex_match(input, pattern)) {
				return input;
			}
			std::cout << "Invalid input. Please try again.\n";
		}
	};

	provider.set_name(get_input("Enter Provider name (max 25 characters): ", std::regex("^.{1,25}$")));
	provider.set_address(get_input("Enter street address (max 25 characters): ", std::regex("^.{1,25}$")));
	provider.set_city(get_input("Enter city (max 14 characters): ", std::regex("^.{1,14}$")));
	provider.set_state(get_input("Enter state (2 letters): ", std::regex("^[A-Za-z]{2}$")));
	provider.set_zip(get_input("Enter ZIP code (5 digits): ", std::regex("^\\d{5}$")));
}



void Manager::display_Provider_Menu(){
	int check{0};
	while (check != 4) {
		std::cout << "1. Add Provider\n";
		std::cout << "2. Update Provider\n";
		std::cout << "3. Remove Provider\n";
		std::cout << "4. Quit to Main Manager Menu\n";
		std::cout << "Enter your option (1 - 4): ";

		std::cin >> check;
		std::cin.ignore(100, '\n');

		if (check == 1) {
			P_add_provider();
		}
		else if (check == 2) {
			P_update_provider();
		}
		else if (check == 3){
			P_delete_provider();
		}
	}

}

bool Manager::P_add_provider(){
	if (!db.is_connected()) {
		return false;
	}

	Provider provider;
	get_valid_provider_input(provider);
	return db.add_provider(provider);

}



bool Manager::P_update_provider(){
	std::string input_id;
	Provider provider;
	try {

		std::regex nine_digits("^\\d{9}$");
		while (!(std::regex_match(input_id, nine_digits))) {
			std::cout << "Enter a 9 digit provider ID: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}
		provider.set_ID(input_id);
		std::cout << provider.get_ID() << "\n";

		if (!db.get_provider(provider)) {
			std::cerr << "No provider with given ID\n";
			return false;
		}
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	provider.Display_Provider_Info();

	std::cout << "Enter new the updated values for the following\n\n";

	get_valid_provider_input(provider);
	return db.update_provider(provider);

}


bool Manager::P_delete_provider(){


	std::string input_id;

	try {

		std::regex nine_digits("^\\d{9}$");
		while (!(std::regex_match(input_id, nine_digits))) {
			std::cout << "Enter a 9 digit member ID: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}

	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	return db.delete_provider(input_id);

}





