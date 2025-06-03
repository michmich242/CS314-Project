#include "../include/provider_user.h"
#include "../include/utils.h"

bool
Provider_User::start_provider()
{

	int check{0};
	std::cout << "Welcome to the provider terminal!" << std::endl;

	while (check != 4) {

		std::cout << "1. member validation" << std::endl;
		std::cout << "2. service billing" << std::endl;
		std::cout << "3. write service directory to file" << std::endl;
		std::cout << "4. quit" << std::endl;
		std::cout << "Enter your option (1 - 4): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		// member validation
		if (check == 1) {
			if (member_validation_wrapper()) {
				std::cout << "Member Status: Active" << std::endl;
			}
			else {
				std::cout << "Member Status: Innactive" << std::endl;
			}
		}
		// service record
		if (check == 2) {
			if (create_service_record()) {
				std::cout << "Service record successfully created!" << std::endl;
			}
			else {
				std::cout << "Service record creation failed." << std::endl;
			}
		}
		// provider directory
		if (check == 3) {
			// write service directory to file
			generate_service_directory();
		}
	}
	return true;
}

Provider_User::Provider_User(SQLEngine &db_ref) : db(db_ref)
{

	std::cout << "Provider_User initialized..." << std::endl;
}

bool
Provider_User::login_wrapper()
{

	std::string input;
	std::cout << "Please enter your 9 digit provider number: ";
	std::getline(std::cin, input);

	return login(input);
}

bool
Provider_User::login(std::string &input)
{
	while (!utils::is_valid_num(input)) {
		std::cout << "Please try again: ";
		std::getline(std::cin, input);
	}
	if (db.validate_provider(input)) {
		this->user.set_ID(input);
		return db.get_provider(this->user);
	}
	return false;
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
Provider_User::member_validation_wrapper()
{
	std::string member_ID;

	std::cout << "enter 9-digit member number: " << std::endl;
	std::getline(std::cin, member_ID);

	if (!member_validation(member_ID)) {
		return false;
	}

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

	// std::cout << "Service name: " << service.get_name() << std::endl;

	std::string comment = utils::get_comments();

	// std::cout << "Total fee: " << service.get_fee() << std::endl;
	std::cout << "Total fee: " << service.get_fee() << std::endl;

	// create service record object
	ServiceRecord record(date_of_service, "", user.get_ID(), member_number, service_code, comment);

	// save record to db

	return db.save_service_record(record);
}

bool
Provider_User::generate_service_directory()
{
	std::vector<Service> services;

	if (!db.get_all_services(services)) {
		std::cerr << "Error: failed to get services from database" << std::endl;
		return false;
	}

	std::ofstream file("../service_directory.txt");
	if (!file.is_open()) {
		std::cerr << "Error: Could not open service_directory.txt for writing" << std::endl;
		return false;
	}

	// header
	file << "+ - - - - - - - - - - - +" << std::endl;
	file << "+   SERVICE DIRECTORY   +" << std::endl;
	file << "+ - - - - - - - - - - - +\n" << std::endl;
	file << std::left << std::setw(10) << "Code" << std::setw(10) << "Fee"
		 << "Description\n";
	file << std::string(50, '-') << "\n";

	// write to file
	std::cout << "generating file..." << std::endl;
	for (auto &service : services) {
		file << std::left << std::setw(10) << service.get_code() << std::setw(10) << std::fixed << std::setprecision(2)
			 << service.get_fee() << service.get_description() << "\n";
	}

	file.close();
}

void
Provider_User::display_service_directory()
{
}
