#include "../include/service.h"
#include "../include/sqlengine.h"
#include "utils.cpp"
#include <regex>


// ---------------------------------------------------------------------------------
/*Service functions start*/

Service::Service() : code(""), fee(0.0), description("")
{
	std::cout << "initializing Service object..." << std::endl;
}

Service::Service(const std::string &code, float fee, const std::string &description)
	: code(code), fee(fee), description(description)
{
}

bool
add_service()
{
	// Request Service Details
	//
}

bool
delete_service(const std::string &service_code)
{
}

bool
update_service(const std::string &service_code)
{
}

bool
get_service(Service &empty_service)
{
}

// ------------------------------------------------------------------------------
// Service SQL


// Retrieves a specific service via service_code and returns new object
// @PARAMS: const std::string &code - 6-digit service_code
Service
Service::get_service_DB(const std::string &code)
{
	// Check My_DBection
	if (My_DB != nullptr || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return Service{};
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		// Run Query
		auto [i_description, fee] = transaction.query1<std::string, float>(
			pqxx::zview("SELECT description, fee FROM Services WHERE service_code = $1"), pqxx::params{code});

		return Service(code, fee, i_description);
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return Service{};
	}
}

// Query db for all services
bool
Service::get_all_services(std::vector<Service> &services)
{
	// Check Conn
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		services.clear();

		// Run Query
		for (auto [code, name, fee] : transaction.query<std::string, std::string, float>(
				 "SELECT service_code, description, fee FROM Services ORDER BY service_code")) {
			services.emplace_back(code, description, fee);
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
