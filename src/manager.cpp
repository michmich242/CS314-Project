#include "../include/manager.h"
#include "../include/sqlengine.h"
#include "utils.cpp"


Member::Member() : member_name(""), member_id(""), address(), status(false), My_DB(nullptr)
{
	My_DB = new SQLEngine;
}

Member::~Member()
{
	delete My_DB;
	My_DB = nullptr;
}

Member::Member(const std::string &passed_member_name, const Address &passed_address, const bool &passed_status)
	: member_name(passed_member_name), member_id(""), address(passed_address), status(passed_status)
{

	My_DB = new SQLEngine;
}

void
Member::Display_Member_Info()
{
	std::cout << "Member name: " << member_name << std::endl
			  << "Member ID: " << member_id << std::endl
			  << "Address " << address.street << std::endl
			  << "City " << address.city << std::endl
			  << "State " << address.state << std::endl
			  << "Zip " << address.zip << std::endl
			  << "Status " << ((status == 1) ? "True\n" : "False\n");
}

bool
Member::add_DB()
{
	return false;
}

bool
Member::update_DB()
{

	int member_id_test = 0;

	/*
	if (!My_DB->validate_member(member_id)) {
		return false;
	}
	*/

	if (member_id.length() != 9) {
		return false;
	}


	try {
		member_id_test = stoi(member_id);
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
Member::delete_member_DB()
{

	int member_id_test = 0;

	/*
	if (!My_DB->validate_member(member_id)) {
		return false;
	}
	*/

	if (member_id.length() != 9) {
		return false;
	}


	try {
		member_id_test = stoi(member_id);
		std::cout << member_id_test << std::endl;
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << std::endl;
		return false;
	}

	//return My_DB->delete_member(member_id);

	return false;
}

std::string &
Member::get_name()
{
	return member_name;
}

std::string &
Member::set_name(const std::string &to_set)
{
	member_name = to_set;
	return member_name;
}

std::string &
Member::get_ID()
{
	return member_id;
}

std::string &
Member::set_ID(const std::string &to_set)
{
	member_id = to_set;
	return member_id;
}

Address &
Member::get_address()
{
	return address;
}

Address &
Member::set_address(const Address &to_set)
{
	address = to_set;
	return address;
}

/*
std::string & Member::get_city(){
>>>>>>> f930c2b7d2c0753fca6b349d13a6236719c1e63e
	return city;
}

std::string & Member::set_city(const std::string & to_set){
	city = to_set;
	return city;
}

std::string & Member::get_state(){
	return State;
}

std::string & Member::set_state(const std::string & to_set){
	State = to_set;
	return State;
}

std::string & Member::get_zip(){
	return zip;
}

std::string & Member::set_zip(const std::string & to_set){
	zip = to_set;
	return zip;
}
*/

bool &
Member::get_status()
{
	return status;
}

bool &
Member::set_status(const bool &switcher)
{
	status = switcher;
	return status;
}

bool
Member::GET_MEMBER_FROM_DB(const std::string &MEMBER_ID)
{
	return false;
}

/*
//
// MEMBER SqL
//-------------------------------------------------------------------------
//
// Adds the passed in member object to the Member table
bool
Member::add_member(const Member &member)
{
	// Confirm Connection
	if (conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction, typical read/write connection
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Check if Member already exists
		res = transaction.exec_params("SELECT 1 FROM members WHERE member_id = $1", member.get_id());

		if (!res.empty()) {
			std::cerr << "Member with ID: " << member.get_id() << " already exists";
			return false;
		}

		// Run Query
		res = transaction.exec_params("INSERT INTO members (name, address, city, state, zip, status)) "
									  "VALUES ($1, $2, $3, $4, $5, $6) RETURNING member_id",
									  member.get_name(), member.get_address(), member.get_city(), member.get_state(), member.get_zip(), true);

		// Check if the member number was generated
		// assign to member_id
		if (!res.empty()) {
			std::string new_member_id = res[0][0].as<std::string>();
			member.set_id(new_member_id);
		}

		// Finalizes Transactions
		// Protects from partial read/writes
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
Member::update_member(const Member &member)
{
	// Double check if connection is open
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Attempt to Update
		res = transaction.exec_params("UPDATE members "
									  "SET name = $1, address = $2, city = $3, zip "
									  "= $4, state = $5, status = $6"
									  "WHERE member_id = $7",

									  // variables being passed to $#
									  member.get_name(), member.get_address(), member.get_city(), member.get_zip(), member.get_state(), member.get_status(), member.get_id());
		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << member.get_id() << "\n";
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
Member::delete_member(const std::string &id)
{
	// Check Connection
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("DELETE FROM members WHERE member_id = $1", id);

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
Member::validate_member(const std::string &id)
{
	// Check Connection
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("SELECT active_status FROM members WHERE member_id = $1", id);
		transaction.commit();

		// Check
		if (res.empty()) {
			std::cerr << "Member not found with ID: " << id << "\n";
			return false;
		}

		// Assign member status to return boolean
		bool status = res[0][0].as<bool>();
		return status;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Validating Member: " << e.what() << "\n";
		return false;
	}
}

// End of member class functions --------------------------------
*/

//--------------------------------------------------------------------------
/* provider functions start */

Provider::Provider() : name(""), provider_id(""), address()
{
}

Provider::~Provider()
{
}

Provider::Provider(const std::string &passed_name, const Address &passed_address) : name(passed_name), provider_id(""), address(passed_address)
{
}

bool
Provider::add_DB()
{
	return false;
}

bool
Provider::update_DB()
{
	return false;
}

bool
delete_DB()
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
Provider::get_id()
{
	return provider_id;
}

std::string &
Provider::set_id(const std::string &to_set)
{
	provider_id = to_set;
	return provider_id;
}

Address &
Provider::get_address()
{
	return address;
}

Address &
Provider::set_address(const Address &to_set)
{
	address = to_set;
	return address;
}

bool
Provider::GET_PROVIDER_FROM_DB(const std::string &PROVIDER_ID)
{
	return false;
}


/*
//----------------------------------------------------------------------
// Provider SQL

// Add Provider to db
// @PARAM: const Provider &provider : Populated Provider Object
// provider.provider_id WILL BE OVERWRITTEN
bool
Provider::add_provider(const Provider &provider)
{
	// Check Connection
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Check if Member already exists
		res = transaction.exec_params("SELECT 1 FROM providers WHERE provider_id = $1", provider.get_id());

		if (!res.empty()) {
			std::cerr << "Member with ID: " << provider.get_id() << " already exists";
			return false;
		}

		// Run Query
		res = transaction.exec_params("INSERT INTO providers (name, address, city, state, zip, status))"
									  "VALUES ($1, $2, $3, $4, $5, $6)",
									  provider.get_name(), provider.get_address(), provider.get_city(), provider.get_state(), provider.get_zip(), true);

		// Check if the provider number was generated
		// assign to provider_id
		if (!res.empty()) {
			std::string new_provider_id = res[0][0].as<std::string>();
			provider.set_id(new_provider_id);
		}

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
// @PARAM: const Provider &provider: Provider object with updated data
// Refers to provider_id for context on which provider to update
bool
Provider::update_provider(const Provider &provider)
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Attempt to Update
		res = transaction.exec_params("UPDATE providers "
									  "SET name = $1, address = $2, city = $3, zip "
									  "= $4, state = $5, status = $6"
									  "WHERE provider_id = $7",

									  // variables being passed to $#
									  provider.get_name(), provider.get_address(), provider.get_city(), provider.get_zip(), provider.get_state(), provider.get_status(),
									  provider.get_id());
		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No provider found with ID: " << provider.get_id() << "\n";
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
Provider::delete_provider(const std::string &id)
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
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

//
bool
Provider::validate_provider(const std::string &id)
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("SELECT provider_id FROM providers WHERE provider_id = $1", id);
		transaction.commit();
		// Check
		if (res.empty()) {
			std::cerr << "Provider not found with ID: " << id << "\n";
			return false;
		}

		// Check did not catch, confirm existance of ID
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Validating Provider: " << e.what() << "\n";
		return false;
	}
}

// End of Provider Functions----------------------------------------------------

//----------------------------------------------------------------------
*/
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
									  record.get_date(), record.get_provider(), record.get_member(), record.get_service(), record.get_comment(););

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