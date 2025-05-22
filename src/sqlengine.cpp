#include "../include/sqlengine.h"

// SQLEngine constructor, does not need an input unless overriding file path
// default path: .dbinfo
//
// .dbinfo default style:
// host=fake_host
// port=5432
// dbname=name
// user=username
// password=12345
//
SQLEngine::SQLEngine(const std::string &db_path)
{
	std::map<std::string, std::string> db_info;

	// Open .dbinfo
	std::ifstream infile(db_path);
	if (!infile) {
		std::cerr << "Failed to retrieve database info.\n" << std::endl;
		return;
	}

	// Intake .dbinfo, use map to make key-value pairs
	std::string key_value;
	while (std::getline(infile, key_value)) {
		size_t eq = key_value.find('=');
		if (eq == std::string::npos)
			continue;
		std::string key = key_value.substr(0, eq);
		std::string value = key_value.substr(eq + 1);
		db_info[key] = value;
	}

	// Create String from .dbinfo lines
	std::ostringstream conn_str;
	conn_str << "host=" << db_info["host"] << " port=" << db_info["port"] << " dbname=" << db_info["dbname"] << " user=" << db_info["user"] << " password=" << db_info["password"];

	// Attempt to connect with given key-value pairs
	try {
		// Assign Unique pointer
		conn = std::make_unique<pqxx::connection>(conn_str.str());
		if (!conn->is_open()) {
			std::cerr << "Failed to open database connection.\n";
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Connection Failed: " << e.what() << "\n";
	}
}

// Checks if our connection is working,
// used as an err checker in main in case
// constructor failed to connect
bool
SQLEngine::is_connected() const
{
	return conn && conn->is_open();
}

// End of Engine Tools
//

// Member interactions--------------------------------------------------------
//
// Adds the passed in member object to the Member table
bool
SQLEngine::add_member(const Member &member)
{
	// Confirm Connection
	if (!conn || !conn->is_open()) {
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
SQLEngine::update_member(const Member &member)
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
SQLEngine::delete_member(const std::string &id)
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
SQLEngine::validate_member(const std::string &id)
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
//

//
// Provider Interactions ----------------------------------------

// Add Provider to db
// @PARAM: const Provider &provider : Populated Provider Object
// provider.provider_id WILL BE OVERWRITTEN
bool
SQLEngine::add_provider(const Provider &provider)
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
SQLEngine::update_provider(const Provider &provider)
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
SQLEngine::delete_provider(const std::string &id)
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
SQLEngine::validate_provider(const std::string &id)
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
//

//
// Service List and Records ----------------------------------------------------
//

// Retrieves a specific service via service_code and returns new object
// @PARAMS: const std::string &code - 6-digit service_code
Service
SQLEngine::get_service(const std::string &code)
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
SQLEngine::get_all_services()
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

// Adds a service record to the dbms
// @PARAMS: const ServiceRecord &record - Service Record populated with new db
// info
// **SERVICE CODE AND TIMESTAMP OVERWRITTEN**
bool
SQLEngine::save_service_record(const ServiceRecord &record)
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

// Weekly Reporting
std::vector<MemberReport>
SQLEngine::generate_member_service_reports()
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);

		transaction.exec_params();

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}

std::vector<ProviderReport>
SQLEngine::generate_provider_service_reports()
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);

		transaction.exec_params();

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}

ManagerSummary
SQLEngine::generate_manager_summary_reports()
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(*conn);

		transaction.exec_params();

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}
