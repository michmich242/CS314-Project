#include "../include/provider_user.h"
#include "../include/utils.h"

bool
Provider_User::start_provider()
{

	int check{0};
	std::cout << "Welcome to the provider terminal!\n";

	while (check != 4) {

		std::cout << "1. Member Validation\n";
		std::cout << "2. Service Billing" << std::endl;
		std::cout << "3. Write Service Directory To File" << std::endl;
		std::cout << "4. Quit" << std::endl;
		std::cout << "Enter your option (1 - 4): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		// member validation
		if (check == 1) {
			if (member_validation_wrapper()) {
				std::cout << "Member Status: Active\n";
			}
			else {
				std::cout << "Member Status: SUSPENDED\n";
			}
		}
		// service record
		if (check == 2) {
			if (create_service_record()) {
				std::cout << "Service record successfully created!\n";
			}
			else {
				std::cout << "Service record creation failed.\n";
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

	std::cout << "Provider_User initialized..\n";
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
		std::cout << "Invalid Input. Please try again: ";
		std::getline(std::cin, member_ID);
	}

	return db.validate_member(member_ID);
}

bool
Provider_User::member_validation_wrapper()
{
	std::string member_ID;

	std::cout << "Enter 9-Digit Member Number: ";
	std::getline(std::cin, member_ID);

	return member_validation(member_ID);
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

	if (!db.validate_member(member_number)) {
		std::cout << '\n';
		return false;
	}

	std::string date_of_service = utils::get_date_of_service();


	std::string service_code = utils::get_service_code();
	Service service;

	if (!db.validate_service(service_code)) {
		std::cout << '\n';
		return false;
	}

	service.set_code(service_code);


	if (!db.get_service(service)) {
		return false;
	}

	// std::cout << "Service name: " << service.get_name() << std::endl;

	std::string comment = utils::get_comments();

	// std::cout << "Total fee: " << service.get_fee() << std::endl;
	std::cout << "Total Fee: " << service.get_fee() << "\n";

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
		std::cerr << "Error: failed to get services from database\n";
		return false;
	}

	std::ofstream file("service_directory.txt");
	if (!file.is_open()) {
		std::cerr << "Error: Could not open service_directory.txt for writing\n";
		return false;
	}

	// header
	file << "+ - - - - - - - - - - - +\n";
	file << "+   SERVICE DIRECTORY   +\n";
	file << "+ - - - - - - - - - - - +\n\n";
	file << std::left << std::setw(10) << "Code" << std::setw(10) << "Fee"
		 << "Description\n";
	file << std::string(50, '-') << "\n";

	// write to file
	std::cout << "generating file...\n";
	for (auto &service : services) {
		file << std::left << std::setw(10) << service.get_code() << std::setw(10) << std::fixed << std::setprecision(2)
			 << service.get_fee() << service.get_description() << "\n";
	}

	file.close();
	return true;
}
