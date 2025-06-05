#include "../include/manager.h"
#include "../include/service.h"
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

	while (check != 5) {
		std::cout << "1. Member Manager Terminal\n";
		std::cout << "2. Provider Manager Terminal\n";
		std::cout << "3. Service Manager Terminal\n";
		std::cout << "4. Generate Reports\n";
		std::cout << "5. Exit Program\n";
		std::cout << "Enter your option (1 - 5): ";


		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << std::endl;

		if (check == 1) {
			display_Member_Menu();
		}
		else if (check == 2) {
			display_Provider_Menu();
		}
		else if (check == 3) {
			display_Service_Menu();
		}
		else if (check == 4) {
			display_Report_Menu();
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
		else if (check == 3) {
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

bool
Manager::delete_member()
{

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

void
Manager::get_valid_provider_input(Provider &provider)
{

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

void
Manager::display_Provider_Menu()
{
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
		else if (check == 3) {
			P_delete_provider();
		}
	}
}

bool
Manager::P_add_provider()
{
	if (!db.is_connected()) {
		return false;
	}

	Provider provider;
	get_valid_provider_input(provider);
	return db.add_provider(provider);
}

bool
Manager::P_update_provider()
{
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

bool
Manager::P_delete_provider()
{


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

void
Manager::display_Service_Menu()
{
	int check{0};
	while (check != 4) {
		std::cout << "1. Add Service\n";
		std::cout << "2. Update Service\n";
		std::cout << "3. Remove Service\n";
		std::cout << "4. Quit to Main Manager Menu\n";
		std::cout << "Enter your option (1 - 4): ";

		std::cin >> check;
		std::cin.ignore(100, '\n');

		if (check == 1) {
			S_add_service();
		}
		else if (check == 2) {
			S_update_service();
		}
		else if (check == 3) {
			S_delete_service();
		}
	}
}

void
Manager::get_valid_service_input(Service &service)
{
	std::string fee_string;
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

	service.set_description(get_input("Enter service description (max 100 characters): ", std::regex("^.{1,100}$")));


	fee_string = get_input("Enter service fee (0 to 99999): ", std::regex("^\\d{1,5}$"));
	service.set_fee(std::stof(fee_string));
}

bool
Manager::S_add_service()
{

	if (!db.is_connected()) {
		return false;
	}

	Service service;
	get_valid_service_input(service);
	return db.add_service(service);
}

bool
Manager::S_update_service()
{

	std::string input_id;
	Service service;
	try {

		std::regex nine_digits("^\\d{6}$");
		while (!(std::regex_match(input_id, nine_digits))) {
			std::cout << "Enter a 6 digit service code: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}
		service.set_code(input_id);
		std::cout << service.get_code() << "\n";

		if (!db.get_service(service)) {
			std::cerr << "No service with given code\n";
			return false;
		}
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	service.Display_Service_Info();

	std::cout << "Enter new the updated values for the following\n\n";

	get_valid_service_input(service);
	return db.update_service(service);
}

bool
Manager::S_delete_service()
{

	std::string input_id;

	try {

		std::regex six_digits("^\\d{6}$");
		while (!(std::regex_match(input_id, six_digits))) {
			std::cout << "Enter a 6 digit service code: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << "\n";
		return false;
	}

	return db.delete_service(input_id);
}

bool
Manager::generate_manager_summary()
{
	ManagerSummary msum{.num_consultations = 0, .total_fees = 0.0};
	if (!db.generate_manager_summary_reports(msum)) {
		std::cerr << "\n**Error, failed to generate Manager Summary**\n";
		return false;
	}
	std::string timestamp = gen_timestamp();
	std::ofstream file("./Reports/ManagerSummary/Manager_Summary_" + timestamp + ".txt");
	if (!file.is_open()) {
		std::cerr << "\n**Error could not open file for writing**\n";
		return false;
	}

	file << "+ - - - - - - - - - - - +\n";
	file << "+    Manager Summary    +\n";
	file << "+ - - - - - - - - - - - +\n\n";

	file << std::left << std::setw(20) << "Provider";
	file << std::right << std::setw(15) << "Consultations" << std::setw(12) << "Fees\n";

	file << std::left << std::string(60, '-') << "\n";

	file << std::fixed << std::setprecision(2);
	for (auto &summary : msum.summaries) {
		file << std::left << std::setw(20) << summary.provider.get_name() << std::right << std::setw(15)
			 << summary.num_consultations << std::setw(12) << summary.total_fee << "\n";
	}

	file << std::left << "\n\n";
	file << std::left << "Total Providers:      " << msum.summaries.size();
	file << std::left << "\nTotal Consultations: " << msum.num_consultations;
	file << std::left << "\nTotal Fee:           " << msum.total_fees << "\n\n";


	file.close();
	return true;
}

bool
Manager::generate_provider_report()
{
	std::vector<ProviderReport> reports;

	if (!db.generate_provider_service_reports(reports)) {
		std::cerr << "\n**Error, failed to generate Provider Reports**\n";
		return false;
	}
	for (auto &provider : reports) {
		std::string timestamp = gen_timestamp();
		std::ofstream file("./Reports/ProviderReports/" + provider.provider_name + "_" + timestamp + ".txt");
		if (!file.is_open()) {
			std::cerr << "\n**Error could not open file for writing**\n";
			return false;
		}

		file << "+ - - - - - - - - - - - +\n";
		file << "+    Provider Summary   +\n";
		file << "+ - - - - - - - - - - - +\n\n";

		file << "Name:    " << provider.provider_name << "\n"
			 << "ID:      " << provider.provider_id << "\n"
			 << "Address: " << provider.address << "\n"
			 << "City:    " << provider.city << "\n"
			 << "State:   " << provider.state << "\n"
			 << "Zip:     " << provider.zip << "\n\n";

		file << "Services Provided\n";
		file << std::setw(15) << "Date of Service" << std::setw(15) << "Timestamp" << std::setw(25) << "Member"
			 << std::setw(10) << "ID" << std::setw(8) << "Service" << std::setw(8) << "Fee\n";
		file << std::string(81, '-') << "\n";
		for (auto &service : provider.services) {
			file << std::left << std::setw(15) << service.date_of_service << std::setw(15) << service.system_timestamp
				 << std::setw(25) << service.member_name << std::setw(10) << service.member_id << std::setw(8)
				 << service.service_code << std::setw(8) << service.fee << "\n";

			provider.num_consultations++;
			provider.total_fee += service.fee;
		}

		file << "\n\n";
		file << "Total Consultations: " << provider.num_consultations;
		file << "\nTotal Fee:           " << provider.total_fee;
		file << "\n";
		file.close();
	}
	return true;
}

bool
Manager::generate_member_report()
{
	std::vector<MemberReport> reports;

	if (!db.generate_member_service_reports(reports)) {
		std::cerr << "\n**Error, failed to generate Memeber Reports**\n";
		return false;
	}

	for (auto &member : reports) {
		std::string timestamp = gen_timestamp();
		std::ofstream file("./Reports/MemberReports/" + member.member_name + "_" + timestamp + ".txt");
		if (!file.is_open()) {
			std::cerr << "\n**Error could not open file for writing**\n";
			return false;
		}

		file << "+ - - - - - - - - - - - +\n";
		file << "+     Member Summary    +\n";
		file << "+ - - - - - - - - - - - +\n\n";

		file << "Name:    " << member.member_name << "\n"
			 << "ID:      " << member.member_id << "\n"
			 << "Address: " << member.address << "\n"
			 << "City:    " << member.city << "\n"
			 << "State:   " << member.state << "\n"
			 << "Zip:     " << member.zip << "\n\n";

		file << "Services Provided\n";
		file << std::setw(15) << "Date of Service" << std::setw(15) << "Timestamp" << std::setw(25) << "Member"
			 << std::setw(10) << "ID" << std::setw(8) << "Service" << std::setw(8) << "Fee\n";
		file << std::string(81, '-') << "\n";
		for (auto &service : member.services) {
			file << std::left << std::setw(15) << service.date_of_service << std::setw(25) << service.provider_name
				 << std::setw(10) << service.service_name << "\n";
		}
		file.close();
	}
	return true;
}

bool
Manager::generate_EFT_Data()
{
	return true;
}

void
Manager::display_Report_Menu()
{
	int check{0};
	while (check != 6) {
		std::cout << "1. Generate All Reports\n";
		std::cout << "2. Generate Manager Summary\n";
		std::cout << "3. Generate Provider Report\n";
		std::cout << "4. Generate Member Report\n";
		std::cout << "5. Generate EFT Data\n";
		std::cout << "6. Return to Main Menu\n";
		std::cout << "Enter your option (1 - 6): ";

		std::cin >> check;
		std::cin.ignore(100, '\n');
		std::cout << "\n\n";

		if (check == 1) {
			generate_manager_summary();
			generate_provider_report();
			generate_member_report();
			generate_EFT_Data();
		}
		else if (check == 2) {
			generate_manager_summary();
		}
		else if (check == 3) {
			generate_provider_report();
		}
		else if (check == 4) {
			generate_member_report();
		}
		else if (check == 5) {
			generate_EFT_Data();
		}
	}
}

std::string
Manager::gen_timestamp()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm *time_pointer = std::localtime(&now_time);

	std::ostringstream oss;
	oss << std::put_time(time_pointer, "%m%d%Y");
	return oss.str();
}
