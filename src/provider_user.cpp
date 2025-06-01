#include "../include/provider_user.h"
#include "../include/utils.h"

bool
Provider_User::start_provider() {

	int check {0};
	std::cout << "Welcome to the provider terminal!" << std::endl;

	while (check != 3) {
		
		std::cout << "1. member validation" << std::endl;
		std::cout << "2. service billing" << std::endl;
		std::cout << "3. provider directory" << std::endl;
		std::cout << "Enter your option (1 - 3): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		if (check == 1) {
			if(member_validation_wrapper()) {
				std::cout << "Member Status: Active" << std::endl;
			} else {
				std::cout << "Member Status: Innactive" << std::endl;
			}
		}
		if (check == 2) {
			create_service_record();
		}
	}
	return true;
}

Provider_User::Provider_User(SQLEngine &db_ref) : db(db_ref)
{
	// Hard-Coded Demo Provider
	user = Provider("demo", "123 demo st.", "Demoville", "Dm", "99999");
	user.set_ID("123456789");

	std::cout << "Provider_User initialized..." << std::endl;
}



// Default login, we setup provider user in demo so login is not necessary
bool
Provider_User::login(std::string &input)
{
	//  std::cout << "Please enter your 9 digit provider number: ";
	//  std::getline(std::cin, input);

	while (!utils::is_valid_num(input)) {
		std::cout << "Please try again: ";
		std::getline(std::cin, input);
	}

	// check if provider exists in db
	// if(db->validate_provider(num_as_string)) { return true; };

	// std::cout << "setting provider id: " << provider.set_id(num_as_string) << std::endl;

	return true;
}

// Member Validation using utils
bool
Provider_User::member_validation(std::string &member_ID)
{
	while (!utils::is_valid_num(member_ID)) {
		std::cout << "Invalid input. Please try again: ";
		std::getline(std::cin, member_ID);
	}

	return db.validate_member(member_ID);
}

bool
Provider_User::member_validation_wrapper() {
	std::string member_ID;

	std::cout << "enter 9-digit member number: " << std::endl;
	std::getline(std::cin, member_ID);

	if(!member_validation(member_ID)) { return false; }

	return true;
}

// Calls db.save_service_record(record)
// Return true or false, returned record should have newly populated ID
bool
Provider_User::create_service_record()
{

	/*
	 This builds a Provider Class Object and passes it to another function to save the information

	 Re-Validate member number.
	 Input date of service (MM-DD-YYYY).
	 Input 6-digit service code

	 Display and confirm service
	 Optionally enter comments (max 100 characters).
	 Display fee for the selected service.
	*/

	std::string member_number = utils::get_member_number();
	std::string date_of_service = utils::get_date_of_service();
	std::string service_code = utils::get_service_code();

	Service service = db.get_service(service_code);

	//std::cout << "Service name: " << service.get_name() << std::endl;
	std::cout << "Service name: " << "not connected to db!!" << std::endl;

	std::string comment = utils::get_comments();

	// std::cout << "Total fee: " << service.get_fee() << std::endl;
	std::cout << "Total fee: " << service.get_fee() << std::endl;


	return false;
}

// call db.get_all_services(std::vector<Services> &services)
void
Provider_User::display_service_directory()
{
}
