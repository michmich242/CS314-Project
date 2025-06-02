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
SQLEngine::SQLEngine()
{
	if (!conn_db()) {
		std::cerr << "Failed to connect\n";
	}
}

bool
SQLEngine::conn_db(const std::string &db_path)
{
	std::map<std::string, std::string> db_info;

	// Open .dbinfo
	std::ifstream infile(db_path);
	if (!infile) {
		std::cerr << "Failed to retrieve database info.\n" << std::endl;
		return false;
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
	// std::ostringstream direct_connect;
	// direct_connect << db_info["DConnect"];
	std::ostringstream transaction_pooler;
	transaction_pooler << db_info["TPooler"];

	// Attempt to connect with given key-value pairs
	try {
		// Assign Unique pointer
		conn = std::make_unique<pqxx::connection>(transaction_pooler.str());

		if (!conn->is_open()) {
			std::cerr << "Failed to open database connection.\n";
			return false;
		}

		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Connection Failed: " << e.what() << "\n";
		return false;
	}
}

pqxx::connection &
SQLEngine::get_connection()
{
	if (!is_connected()) {
		conn_db();
	}
	return *conn;
}

// Checks if our connection is working,
// used as an err checker in main in case
// constructor failed to connect
bool
SQLEngine::is_connected() const
{
	return conn && conn->is_open();
}

//
// End of Engine Tools
//


/*
// Weekly Reporting
bool
SQLEngine::generate_member_service_reports(std::vector<MemberReport> &vector)
{
	if (is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

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

*/
// MANAGER TERMINAL CRUD



//-------------------------------------------------------------------------
// MEMBER CRUD FUNCTIONS
//
// Adds the passed in member object to the Member table
bool
SQLEngine::add_member(Member &member)
{
	// Confirm Connection
	if (!is_connected()) {
		std::cerr << "db not open\n";
		return false;
	}

	try {
		// Start a transaction, typical read/write My_DBection
		pqxx::work transaction(get_connection());

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
SQLEngine::update_member(Member &member)
{
	// Double check if My_DBection is open
	if (!is_connected()) {
		std::cerr << "db not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		// Attempt to Update
		pqxx::result res =
			transaction.exec(pqxx::zview(R"(UPDATE chocan.members
						   SET name = $1, address = $2, city = $3, zip= $4, state_abbrev = $5, active_status = $6
						   WHERE member_id = $7)"),

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

bool
SQLEngine::get_member(Member &member)
{
	if (!is_connected()) {
		std::cout << "DB connection failed\n";
		return false;
	}
	try {
		pqxx::work transaction(*conn);
		pqxx::result res = transaction.exec(pqxx::zview(R"(SELECT name, address, city, zip, state_abbrev, active_status
			 FROM chocan.members 
			 WHERE member_id = $1)"),
											pqxx::params{member.get_ID()});

		if (res.empty()) {
			std::cout << "No member found with ID: " << member.get_ID() << "\n";
			return false;
		}

		member.set_name(res[0][0].c_str());
		member.set_address(res[0][1].c_str());
		member.set_city(res[0][2].c_str());
		member.set_zip(res[0][3].c_str());
		member.set_state(res[0][4].c_str());
		member.set_status(res[0][5].as<bool>());


		return true;
	}
	catch (const std::exception &e) {
		std::cout << "Error retrieving member: " << e.what() << "\n";
		return false;
	}
}

// Delete member given id, value is expected to be properly pre-processed
// Member IDs are in the range of 100000000 -> 199999999
bool
SQLEngine::delete_member(const std::string &id)
{
	// Check Connection
	if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		// Run Query
		auto res = transaction.exec(pqxx::zview("DELETE FROM chocan.members WHERE member_id = $1"), pqxx::params{id});

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
	if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		try {
			bool status = transaction.query_value<bool>(pqxx::zview("SELECT active_status FROM chocan.members WHERE member_id = $1"), pqxx::params{id});
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

//
// End of member CRUD
//


//----------------------------------------------------------------------
// Provider CRUD
//
// Add Provider to db
// @PARAM: Provider &provider : Populated Provider Object
// provider.provider_id WILL BE OVERWRITTEN
bool
SQLEngine::add_provider(Provider &provider)
{
	// Check Connection
	if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

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
			transaction.query_value<std::string>(pqxx::zview("INSERT INTO chocan.providers (name, address, city, state_abbrev, zip)"
															 " VALUES ($1, $2, $3, $4, $5) RETURNING provider_id"),
												 pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_state(), provider.get_zip()});

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
SQLEngine::update_provider(Provider &provider)
{
	if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());
		pqxx::result res =
			transaction.exec(pqxx::zview(R"(UPDATE chocan.providers 
			                SET name = $1, address = $2, city = $3, zip = $4, state_abbrev = $5 
                			WHERE provider_id = $6)"),

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
SQLEngine::delete_provider(const std::string &id)
{
	if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());
		pqxx::result res;

		// Run Query
		res = transaction.exec(pqxx::zview("DELETE FROM chocan.providers WHERE provider_id = $1"), pqxx::params{id});

		// Ensure a row was deleted
		if (res.affected_rows() == 0) {
			std::cerr << "No provider found with ID: " << id << "\n";
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

bool
SQLEngine::get_provider(Provider & provider)

{
	if (!is_connected()) {
		std::cout << "DB connection failed\n";
		return false;
	}
	try {
	pqxx::work transaction(*conn);
	pqxx::result res = transaction.exec(pqxx::zview(R"(SELECT name, address, city, zip, state_abbrev
			FROM chocan.providers 
			WHERE provider_id = $1)"),
										pqxx::params{provider.get_ID()});

	if (res.empty()) {
		std::cout << "No provider found with ID: " << provider.get_ID() << "\n";
		return false;
	}
	
	provider.set_name(res[0][0].c_str());
	provider.set_address(res[0][1].c_str());
	provider.set_city(res[0][2].c_str());
	provider.set_zip(res[0][3].c_str());
	provider.set_state(res[0][4].c_str());

	std::cout << provider.get_name() << provider.get_address() << std::endl;


	return true;
	}
	catch (const std::exception &e) {
	std::cout << "Error retrieving member: " << e.what() << "\n";
	return false;
	}
}

//
// End of Provider CRUD
//

//------------------------------------------------------------------------------
// SERVICE DIRECTORY FUNCTIONS
//
// Retrieves a specific service via service_code and returns new object
// @PARAMS: const std::string &code - 6-digit service_code
Service
SQLEngine::get_service(const std::string &code)
{
	// Check My_DBection
	if (is_connected()) {
		std::cerr << "db not open\n";
		return Service{};
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		// Run Query
		auto [i_description, fee] = transaction.query1
		<std::string, float>(pqxx::zview("SELECT description, fee FROM Services WHERE service_code = $1"), 
		pqxx::params{code});

		return Service(code, fee, i_description);
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return Service{};
	}
}

// Query db for all services
bool
SQLEngine::get_all_services(std::vector<Service> &services)
{
	// Check Conn
	if (is_connected()) {
		std::cerr << "db not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		services.clear();

		// Run Query
		for (auto [code, description, fee] : transaction.query<std::string, std::string, float>("SELECT service_code, description, fee FROM Services ORDER BY service_code")) {
			services.emplace_back(code, fee, description);
		}

		if (services.empty()) {
			std::cerr << "No services available\n";
			return false;
		}
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}

//
// End of Service Directory Functions
//


// --------------------------------------------------------------------------------
// Service Record SQL
// Adds a service record to the dbms
// @PARAMS: const ServiceRecord &record - Service Record populated with new db
// info
// **SERVICE CODE AND TIMESTAMP OVERWRITTEN**
bool
SQLEngine::save_service_record(ServiceRecord &record)
{
	// Ensure My_DBection
	if (!is_connected()) {
		std::cerr << "db not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		// Attempt Query
		std::string new_id =
			transaction.query_value<std::string>(pqxx::zview(R"(INSERT INTO ServiceRecords
						  (date_of_service, provider_id, member_id, service_code, comments)
            			  VALUES($1, $2, $3, $4, $5) RETURNING service_code)"),
												 pqxx::params{record.get_date(), record.get_provider(), record.get_member(), record.get_service_code(), record.get_comment()});

		record.set_service_code(new_id);

		// Finalize transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}
