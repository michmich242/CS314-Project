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


/* commenting out weekly reporting stuff
since it appears to be under construction */

/*

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

*/
