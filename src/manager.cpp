#include "../include/manager.h"
#include "../include/sqlengine.h"
#include "utils.cpp"

SQLEngine *My_DB = new SQLEngine();

Member::Member() : name(""), id(""), address(""), city(""), state(""), zip(""), status(false), My_DB(nullptr)
{
	My_DB = new SQLEngine;
}

Member::~Member()
{
	delete My_DB;
	My_DB = nullptr;
}

Member::Member(const std::string &name, const std::string &address, const std::string &city, const std::string &state,
			   const std::string &zip, const bool &status)
	: name(name), id(""), address(address), city(city), state(state), zip(zip), status(status)
{

	My_DB = new SQLEngine;
}

void
Member::Display_Member_Info()
{
	std::cout << "Member name: " << name << "\nMember ID: " << id << "\nAddress " << address << "\nCity " << city
			  << "\nState " << state << "\nZip " << zip << "\nStatus " << status;
}

bool
Member::add_member()
{
	return false;
}

bool
Member::update_member()
{
	return false;
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

bool
Member::set_name(const std::string &to_set)
{
	name = to_set;
	return true;
}

std::string &
Member::get_ID()
{
	return id;
}

bool
Member::set_ID(const std::string &to_set)
{
	id = to_set;
	return true;
}

std::string &
Member::get_address()
{
	return address;
}

bool
Member::set_address(const std::string &to_set)
{
	address = to_set;
	return true;
}

std::string &
Member::get_city()
{
	return city;
}

bool
Member::set_city(const std::string &to_set)
{
	city = to_set;
	return true;
}

std::string &
Member::get_state()
{
	return state;
}

bool
Member::set_state(const std::string &to_set)
{
	state = to_set;
	return true;
}

std::string &
Member::get_zip()
{
	return zip;
}

bool
Member::set_zip(const std::string &to_set)
{
	zip = to_set;
	return true;
}

bool &
Member::get_status()
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
			auto exists = transaction.query_value<int>(pqxx::zview("SELECT 1 FROM members WHERE member_id = $1"),
													   pqxx::params{member.get_ID()});

			std::cerr << "Member with ID: " << member.get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// member doesnt exist
		}

		std::string new_member_id = transaction.query_value<std::string>(
			pqxx::zview("INSERT INTO MEMBERS (name, address, city, state, zip, status) "
						"VALUES ($1, $2, $3, $4, $5, $6) RETURNING member_id"),
			pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_state(),
						 member.get_zip(), true});

		member.set_ID(new_member_id);

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Inserting Member: " << e.what() << "\n";
		return false;
	}
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
			R"(UPDATE members
                           SET name = $1, address = $2, city = $3, zip= $4, state = $5, status = $6
    			           WHERE member_id = $7 LIMIT 1)",

			// variables being passed to $#
			pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_zip(),
						 member.get_state(), member.get_status(), member.get_ID()});
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
		auto res = transaction.exec_params("DELETE FROM members WHERE member_id = $1", id);

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
			bool status = transaction.query_value<bool>(pqxx::zview("SELECT status FROM members WHERE member_id = $1"),
														pqxx::params{id});
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

Provider::Provider(const std::string &name, const std::string &address, const std::string &city,
				   const std::string &state, const std::string &zip)
	: name(name), id(""), address(address), city(city), state(state), zip(zip)
{
}

std::string &
Provider::get_name()
{
	return name;
}

bool
Provider::set_name(const std::string &to_set)
{
	name = to_set;
	return true;
}

std::string &
Provider::get_ID()
{
	return id;
}

bool
Provider::set_ID(const std::string &to_set)
{
	id = to_set;
	return true;
}

std::string &
Provider::get_address()
{
	return address;
}

bool
Provider::set_address(const std::string &to_set)
{
	address = to_set;
	return true;
}

std::string &
Provider::get_city()
{
	return city;
}

bool
Provider::set_city(const std::string &to_set)
{
	city = to_set;
	return true;
}

std::string &
Provider::get_state()
{
	return state;
}

bool
Provider::set_state(const std::string &to_set)
{
	state = to_set;
	return true;
}

std::string &
Provider::get_zip()
{
	return zip;
}

bool
Provider::set_zip(const std::string &to_set)
{
	zip = to_set;
	return true;
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
			auto exists = transaction.query_value<int>(pqxx::zview("SELECT 1 FROM providers WHERE provider_id = $1"),
													   pqxx::params{provider.get_ID()});

			std::cerr << "Provider with ID: " << provider.get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// provider doesnt exist
		}
		// Run Query
		std::string new_provider_id = transaction.query_value<std::string>(
			pqxx::zview("INSERT INTO providers (name, address, city, state, zip, status))"
						"VALUES ($1, $2, $3, $4, $5, $6)"),
			pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_state(),
						 provider.get_zip(), true});

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
			R"(UPDATE providers 
			SET name = $1, address = $2, city = $3, zip = $4, state = $5 
			WHERE provider_id = $6)",

			// variables being passed to $#
			pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_zip(),
						 provider.get_state(), provider.get_ID()});

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
		res = transaction.exec_params("DELETE FROM providers WHERE provider_id = $1", id);

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

// Check Member Active Status
bool
Provider::validate_provider_DB(const std::string &id)
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
			bool status = transaction.query_value<bool>(
				pqxx::zview("SELECT status FROM providers WHERE provider_id = $1"), pqxx::params{id});
			transaction.commit();
			return status;
		}
		catch (const pqxx::unexpected_rows &) {
			std::cerr << "Provider not found with ID:" << id << "\n";
			return false;
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Error validating provider: " << e.what() << "\n";
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
