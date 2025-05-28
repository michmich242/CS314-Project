#include "../include/member.h"
#include "../include/sqlengine.h"
#include "../include/utils.h"
#include <regex>

SQLEngine *My_DB = new SQLEngine();

void
Member::get_valid_member_input(std::string &member_name, std::string &address, std::string &city, std::string &state,
							   std::string &zip, std::string &status_str)
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

	member_name = get_input("Enter member name (max 25 characters): ", std::regex("^.{1,25}$"));
	address = get_input("Enter street address (max 25 characters): ", std::regex("^.{1,25}$"));
	city = get_input("Enter city (max 14 characters): ", std::regex("^.{1,14}$"));
	state = get_input("Enter state (2 letters): ", std::regex("^[A-Za-z]{2}$"));
	zip = get_input("Enter ZIP code (5 digits): ", std::regex("^\\d{5}$"));
	status_str = get_input("Is the member active? (1 for active, 0 for inactive): ", std::regex("^[01]$"));

	status = (status_str == "1");
}

void
Member::display_Member_Menu()
{
	int check{0};
	while (check != 4) {
		std::cout << "1. Add Member" << std::endl;
		std::cout << "2. Update Member" << std::endl;
		std::cout << "3. Remove Member" << std::endl;
		std::cout << "4. Quit to Main Manager Menu" << std::endl;
		std::cout << "Enter your option (1 - 4): ";

		std::cin >> check;
		std::cin.ignore(100, '\n');

		if (check == 1) {
			add_member();
		}
		else if(check == 2){
			update_member();
		}
	}
}

Member::Member() : name(""), id(""), address(), status(false)
{
}

Member::~Member(){

}

Member::Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city,
			   const std::string &passed_zip, const bool &passed_status)
	: name(passed_member_name), id(""), address(passed_address), city(passed_city), zip(passed_zip),
	  status(passed_status)
{
}

void
Member::Display_Member_Info()
{	
	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << "	Member name: " << name << std::endl
			  << "	Member ID: " << id << std::endl
			  << "	Address " << address << std::endl
			  << "	City " << city << std::endl
			  << "	State " << state << std::endl
			  << "	Zip " << zip << std::endl
			  << "	Status " << ((status == 1) ? "True\n" : "False\n");
	std::cout << "----------------------------------------------------" << std::endl;
}

bool
Member::add_member()
{
	if (!My_DB->is_connected()) {
		return false;
	}

	std::string hold_name;
	std::string hold_id;
	std::string hold_address;
	std::string hold_city;
	std::string hold_state;
	std::string hold_zip;
	std::string stats;

	get_valid_member_input(name, address, city, state, zip, stats);

	return add_member_DB(*this);
}




bool
Member::update_member()
{

	std::string input_id;

	if (!My_DB->is_connected()) {
		std::cout << "DB not connected" << std::endl;
		return false;
	}


	try {

		std::regex nine_digits("^\\d{9}$");
		while(!(std::regex_match(input_id, nine_digits))){
			std::cout << "Enter a 9 digit member ID: ";
			std::cin >> input_id;
			std::cin.ignore(100, '\n');
		}

		id = input_id;
		std::cout << id << std::endl;

		//bool GET_MEMBER_FROM_DB(const std::string &MEMBER_ID);

	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << std::endl;
		return false;
	}




	return update_member_DB(*this);
}


/*
bool
Member::delete_member()
{

	int member_id_test = 0;

	
	if (!My_DB->validate_member(member_id)) {
		return false;
	}
	

	if (id.length() != 9) {
		return false;
	}


	try {
		member_id_test = stoi(id);
		std::cout << member_id_test << std::endl;
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << std::endl;
		return false;
	}

	// return My_DB->delete_member(member_id);

	return false;
}
*/


std::string &
Member::get_name()
{
	return name;
}

std::string &
Member::set_name(const std::string &to_set)
{
	name = to_set;
	return name;
}

std::string &
Member::get_ID()
{
	return id;
}

std::string &
Member::set_ID(const std::string &to_set)
{
	id = to_set;
	return id;
}

std::string &
Member::get_address()
{
	return address;
}

std::string &
Member::set_address(const std::string &to_set)
{
	address = to_set;
	return address;
}

std::string &
Member::get_city()
{
	return city;
}

std::string &
Member::set_city(const std::string &to_set)
{
	city = to_set;
	return city;
}

std::string &
Member::get_state()
{
	return state;
}

std::string &
Member::set_state(const std::string &to_set)
{
	state = to_set;
	return state;
}

std::string &
Member::get_zip()
{
	return zip;
}

std::string &
Member::set_zip(const std::string &to_set)
{
	zip = to_set;
	return zip;
}

bool
Member::get_status() const
{
	return status;
}

bool
Member::set_status()
{
	status = (status == true) ? false : true;
	return status;
}


//
// MEMBER SqL
//-------------------------------------------------------------------------
//
// Adds the passed in member object to the Member table
bool
Member::add_member_DB(Member &member)
{
	// Confirm Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction, typical read/write My_DBection
		pqxx::work transaction(My_DB->get_connection());

		try {
			auto exists = transaction.query_value<int>(pqxx::zview("SELECT 1 FROM chocan.members WHERE member_id = $1"),
													   pqxx::params{get_ID()});

			std::cerr << "Member with ID: " << get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// member doesnt exist
			std::string new_member_id = transaction.query_value<std::string>(
				pqxx::zview("INSERT INTO chocan.MEMBERS (name, address, city, state_abbrev, zip, active_status) "
							"VALUES ($1, $2, $3, $4, $5, $6) RETURNING member_id"),
				pqxx::params{get_name(), get_address(), get_city(), get_state(), get_zip(), true});

			member.set_ID(new_member_id);

			transaction.commit();
			return true;
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Error Inserting Member: " << e.what() << "\n";
		return false;
	}

	return true;
}

// Updates the member_id with the information found in member
// The member should have mathcing member_id to the one
// being modified
bool
Member::update_member_DB(Member &member)
{
	// Double check if My_DBection is open
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		if(!(GET_MEMBER_FROM_DB(&transaction))){
			std::cout << "Unable to retrieve ID: " << get_ID() << " from DB\n" << std::endl;
			return false;
		}

		std::cout << "Successfully retrieved member ID from the DB." << std::endl;

		Display_Member_Info();

		std::string hold_name;
		std::string hold_id;
		std::string hold_address;
		std::string hold_city;
		std::string hold_state;
		std::string hold_zip;
		std::string stats;

		std::cout << "Enter new the updated values for the following\n" << std::endl;

		get_valid_member_input(name, address, city, state, zip, stats);




		// Attempt to Update
		pqxx::result res = transaction.exec(
			pqxx::zview(R"(UPDATE chocan.members
						   SET name = $1, address = $2, city = $3, zip= $4, state_abbrev = $5, active_status = $6
						   WHERE member_id = $7)"),

			// variables being passed to $#
			pqxx::params{get_name(), get_address(), get_city(), get_zip(), get_state(), get_status(), get_ID()});
		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << get_ID() << "\n";
			return false;
		}





		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Updating Member: " << e.what() << "\n";
		return false;
	}
}



bool Member::GET_MEMBER_FROM_DB(pqxx::work * transaction){
	if(!My_DB || !My_DB->is_connected()){
		std::cout << "DB connection failed\n" << std::endl;
		return false;
	}
	try{
		pqxx::result res = transaction->exec(pqxx::zview(R"(SELECT name, address, city, zip, state_abbrev, active_status
			 FROM chocan.members 
			 WHERE member_id = $1)"), pqxx::params{get_ID()});

		if(res.empty()){
			std::cout << "No member found with ID: " << get_ID() << std::endl;
			return false;
		}

		name = res[0][0].c_str();
		address = res[0][1].c_str();
		city = res[0][2].c_str();
		zip = res[0][3].c_str();
		state = res[0][4].c_str();
		status = res[0][5].as<bool>();

	
		return true;
	}
	catch (const std::exception &e){
		std::cout << "Error retrieving member: " << e.what() << std::endl;
		return false;
	}
}




// Delete member given id, value is expected to be properly pre-processed
// Member IDs are in the range of 100000000 -> 199999999
bool
Member::delete_member_DB(const std::string &id)
{
	// Check Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		// Run Query
		auto res = transaction.exec_params("DELETE FROM chocan.members WHERE member_id = $1", id);

		// Ensure a row was deleted
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << id << "\n";
			return false;
		}

		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Deleting Member: " << e.what() << "\n";
		return false;
	}
}

// Check Member Active Status
bool
Member::validate_member_DB(const std::string &id)
{
	// Check Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		try {
			bool status = transaction.query_value<bool>(
				pqxx::zview("SELECT status FROM chocan.members WHERE member_id = $1"), pqxx::params{id});
			transaction.commit();
			return status;
		}
		catch (const pqxx::unexpected_rows &) {
			std::cerr << "Member not found with ID:" << id << "\n";
			return false;
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Error validating member: " << e.what() << "\n";
		return false;
	}
}

// End of member class functions --------------------------------
