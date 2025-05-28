#include "../include/service_record.h"
#include "../include/sqlengine.h"
#include "utils.cpp"
#include <regex>

SQLEngine *My_DB = new SQLEngine();
//----------------------------------------------------------------------

/*ServiceRecord functions start*/

ServiceRecord::ServiceRecord() : timestamp(""), date_of_service(""), provider_id(""), member_id(""), comment("")
{
	std::cout << "initializing ServiceRecord object..." << std::endl;
}

ServiceRecord::~ServiceRecord()
{
}

std::string &
ServiceRecord::get_date()
{
	return date_of_service;
}

std::string &
ServiceRecord::get_provider()
{
	return provider_id;
}

std::string &
ServiceRecord::get_member()
{
	return member_id;
}

std::string &
ServiceRecord::get_service_code()
{
	return service_code;
}

std::string &
ServiceRecord::get_comment()
{
	return comment;
}

std::string &
ServiceRecord::set_date(const std::string &date)
{
	date_of_service = date;
	return date_of_service;
}

std::string &
ServiceRecord::set_provider(const std::string &provider_id)
{
	this->provider_id = provider_id;
	return this->provider_id;
}

std::string &
ServiceRecord::set_member(const std::string &member_id)
{
	this->member_id = member_id;
	return this->member_id;
}

std::string &
ServiceRecord::set_service_code(const std::string &service_code)
{
	this->service_code = service_code;
	return this->service_code;
}

std::string &
ServiceRecord::set_comment(const std::string &comment)
{
	this->comment = comment;
	return this->comment;
}

// --------------------------------------------------------------------------------
// Service Record SQL
// Adds a service record to the dbms
// @PARAMS: const ServiceRecord &record - Service Record populated with new db
// info
// **SERVICE CODE AND TIMESTAMP OVERWRITTEN**
bool
ServiceRecord::save_record_DB(ServiceRecord &record)
{
	// Ensure My_DBection
	if (!My_DB || !My_DB->is_My_DBected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_My_DBection());

		// Attempt Query
		std::string new_service_code = transaction.query_value<std::string>(
			pqxx::zview(R"(INSERT INTO ServiceRecords
						  (date_of_service, provider_id, member_id, service_code, comments)
            			  VALUES($1, $2, $3, $4, $5) RETURNING service_code)"),
			pqxx::params{get_date(), get_provider(), get_member(), get_service_code(), get_comment()});

		set_service_code(new_service_code);

		// Finalize transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}
