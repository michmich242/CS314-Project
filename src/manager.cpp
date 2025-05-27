#include "../include/manager.h"
#include "../include/sqlengine.h"
#include "utils.cpp"
#include <regex>

SQLEngine *My_DB = new SQLEngine();

void
Member::get_valid_member_input(std::string &member_name, std::string &address, std::string &city, std::string &state, std::string &zip, std::string &status_str)
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
	}
}

Member::Member() : name(""), id(""), address(), status(false)
{
}

Member::~Member()
{
}

Member::Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city, const std::string &passed_zip, const bool &passed_status)
	: name(passed_member_name), id(""), address(passed_address), city(passed_city), zip(passed_zip), status(passed_status)
{
}

void
Member::Display_Member_Info()
{
	std::cout << "Member name: " << name << std::endl
			  << "Member ID: " << id << std::endl
			  << "Address " << address << std::endl
			  << "City " << city << std::endl
			  << "State " << state << std::endl
			  << "Zip " << zip << std::endl
			  << "Status " << ((status == 1) ? "True\n" : "False\n");
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

	int member_id_test = 0;

	if (!My_DB->is_connected()) {
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

	/*
		if(!utils::is_valid_num(member_id_test)){
			return false;
		}
	*/

	/*
	return My_DB->update_member(*this);
	*/
}

bool
Member::delete_member()
{

	int member_id_test = 0;

	/*
	if (!My_DB->validate_member(member_id)) {
		return false;
	}
	*/

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

bool
Member::GET_MEMBER_FROM_DB(const std::string &MEMBER_ID)
{
	return false;
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
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction, typical read/write connection
		pqxx::work transaction(My_DB->get_connection());

		try {
			auto exists = transaction.query_value<int>(pqxx::zview("SELECT 1 FROM chocan.members WHERE member_id = $1"), pqxx::params{member.get_ID()});

			std::cerr << "Member with ID: " << member.get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// member doesnt exist
			std::string new_member_id =
				transaction.query_value<std::string>(pqxx::zview("INSERT INTO chocan.MEMBERS (name, address, city, state_abbrev, zip, active_status) "
																 "VALUES ($1, $2, $3, $4, $5, $6) RETURNING member_id"),
													 pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_state(), member.get_zip(), true});

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
	// Double check if connection is open
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		// Attempt to Update
		pqxx::result res = transaction.exec_params(
			R"(UPDATE chocan.members
						   SET name = $1, address = $2, city = $3, zip= $4, state_abbrev = $5, active_status = $6
						   WHERE member_id = $7 LIMIT 1)",

			// variables being passed to $#
			pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_zip(), member.get_state(), member.get_status(), member.get_ID()});
		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << member.get_ID() << "\n";
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

// Delete member given id, value is expected to be properly pre-processed
// Member IDs are in the range of 100000000 -> 199999999
bool
Member::delete_member_DB(const std::string &id)
{
	// Check Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db connection not open\n";
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
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		try {
			bool status = transaction.query_value<bool>(pqxx::zview("SELECT status FROM chocan.members WHERE member_id = $1"), pqxx::params{id});
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


//--------------------------------------------------------------------------
/* provider functions start */

Provider::Provider() : name(""), id(""), address(""), city(""), state(""), zip("")
{
}

Provider::~Provider()
{
}

Provider::Provider(const std::string &passed_name, const std::string &passed_address, const std::string &passed_city, const std::string &passed_zip)
	: name(passed_name), id(""), address(passed_address), city(passed_city), zip(passed_zip)
{
}

bool
Provider::add_provider()
{
	return false;
}

bool
Provider::update_provider()
{
	return false;
}

bool
delete_provider()
{
	return false;
}

std::string &
Provider::get_name()
{
	return name;
}

std::string &
Provider::set_name(const std::string &to_set)
{
	name = to_set;
	return name;
}

std::string &
Provider::get_ID()
{
	return id;
}

std::string &
Provider::set_ID(const std::string &to_set)
{
	id = to_set;
	return id;
}

std::string &
Provider::get_address()
{
	return address;
}

std::string &
Provider::set_address(const std::string &to_set)
{
	address = to_set;
	return address;
}

std::string &
Provider::get_city()
{
	return city;
}

std::string &
Provider::set_city(const std::string &to_set)
{
	city = to_set;
	return city;
}

std::string &
Provider::get_state()
{
	return state;
}

std::string &
Provider::set_state(const std::string &to_set)
{
	state = to_set;
	return state;
}

std::string &
Provider::get_zip()
{
	return zip;
}

std::string &
Provider::set_zip(const std::string &to_set)
{
	zip = to_set;
	return zip;
}

bool
Provider::GET_PROVIDER_FROM_DB(const std::string &PROVIDER_ID)
{
	return false;
}

//----------------------------------------------------------------------
// Provider SQL

// Add Provider to db
// @PARAM: Provider &provider : Populated Provider Object
// provider.provider_id WILL BE OVERWRITTEN
bool
Provider::add_provider_DB(Provider &provider)
{
	// Check Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		try {
			auto exists = transaction.query_value<int>(pqxx::zview("SELECT 1 FROM chocan.providers WHERE provider_id = $1"), pqxx::params{provider.get_ID()});

			std::cerr << "Provider with ID: " << provider.get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// provider doesnt exist
		}
		// Run Query
		std::string new_provider_id =
			transaction.query_value<std::string>(pqxx::zview("INSERT INTO chocan.providers (name, address, city, state_abbrev, zip, active_status))"
															 "VALUES ($1, $2, $3, $4, $5, $6)"),
												 pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_state(), provider.get_zip(), true});

		provider.set_ID(new_provider_id);

		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Inserting Provider: " << e.what() << "\n";
		return false;
	}
}

// Modify an existing provider in the DB
// @PARAM: Provider &provider: Provider object with updated data
// Refers to provider_id for context on which provider to update
bool
Provider::update_provider_DB(Provider &provider)
{
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());
		pqxx::result res = transaction.exec_params(
			R"(UPDATE chocan.providers 
			SET name = $1, address = $2, city = $3, zip = $4, state_abbrev = $5 
			WHERE provider_id = $6)",

			// variables being passed to $#
			pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_zip(), provider.get_state(), provider.get_ID()});

		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No provider found with ID: " << provider.get_ID() << "\n";
			return false;
		}

		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Updating Provider: " << e.what() << "\n";
		return false;
	}
}

// Remove a provider from the database
// @PARAM: const sd::string &id - 9 digit string id,
// ID is required to be fully numeric and between <100000000-999999999>
bool
Provider::delete_provider_DB(const std::string &id)
{
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("DELETE FROM chocan.providers WHERE provider_id = $1", id);

		// Ensure a row was deleted
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << id << "\n";
			return false;
		}

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Deleting Member: " << e.what() << "\n";
		return false;
	}
}
// End of Provider Functions----------------------------------------------------

//----------------------------------------------------------------------

/*ServiceRecord functions start*/

ServiceRecord::ServiceRecord() : timestamp(""), date_of_service(""), provider_id(""), member_id(""), comment("")
{
	std::cout << "initializing ServiceRecord object..." << std::endl;
}

ServiceRecord::~ServiceRecord()
{
}

/*
// --------------------------------------------------------------------------------
// Service Record SQL
// Adds a service record to the dbms
// @PARAMS: const ServiceRecord &record - Service Record populated with new db
// info
// **SERVICE CODE AND TIMESTAMP OVERWRITTEN**
bool
ServiceRecord::save_service_record(const ServiceRecord &record)
{
	// Ensure connection
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Attempt Query
		res = transaction.exec_params("INSERT INTO ServiceRecords"
									  "(date_of_service, provider_id, member_id, service_code, comments)"
									  "VALUES($1, $2, $3, $3, $5) RETURNING service_code",
									  record.get_date(), record.get_provider(), record.get_member(),
record.get_service(), record.get_comment(););

		// Check result, should have returned service_code
		if (!res.empty()) {
			std::string new_service_code = res[0][0].as<std::string>();
			record.set_service_code(new_service_code);
		}

		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}
*/
// ---------------------------------------------------------------------------------
/*Service functions start*/

Service::Service() : service_code(""), service_fee(""), service_description("")
{
	std::cout << "initializing Service object..." << std::endl;
}

// -------------------------------------------------------------------------------
// Service SQL

/*
// Retrieves a specific service via service_code and returns new object
// @PARAMS: const std::string &code - 6-digit service_code
Service
Service::get_service(const std::string &code)
{
	// Check connection
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return Service{};
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("SELECT name, fee FROM Services WHERE service_code = $1", code);

		// Check result
		if (res.empty()) {
			std::cerr << "Could not find service with code:" << code << "\n";
			return Service{};
		}

		// Create new Service from result
		Service service(code, res[0][0].as<std::string>(), res[0][1].as<float>());
		return service;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return Service{};
	}
}

// Query db for all services
std::vector<Service>
Service::get_all_services()
{
	// Check Conn
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("SELECT service_code, name, fee FROM Services");
		if (res.affected_rows() == 0) {
			std::cerr << "No Services Available\n";
			return false;
		}

		// Create and populate Service Vector
		std::vector<Service> vec;
		for (int i = 0; i < res.size(); i++) {
			Service service(res[i][0].as<std::string>(), res[i][1].as<std::string>(), res[i][2].as<float>());
			vec.push_back(service);
		}

		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}

*/
