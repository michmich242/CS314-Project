// SQL API for ChocAn Database
//

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <pqxx/pqxx>
#include <sstream>


/*
• Member name (25 characters).
• Member number (9 digits).
• Member street address (25 characters).
• Member city (14 characters).
• Member state (2 letters).
• Member zip code (5 digits).
• For each service provided, the following details are required:
	o Date of service (MM-DD-YYYY).
	o Provider name (25 characters).
	o Service name (20 characters).
*/
struct ServiceSummary {
	std::string date_of_service;
	std::string provider_name;
	std::string service_name;
};

struct MemberReport {
	std::string member_name;
	std::string member_id;
	std::string address;
	std::string city;
	std::string state;
	std::string zip;

	std::vector<ServiceSummary> services;
};

// Struct to pass back provider summaries
// Passed back in a vector
/*
• Provider name (25 characters).
• Provider number (9 digits).
• Provider street address (25 characters).
• Provider city (14 characters).
• Provider state (2 letters).
• Provider zip code (5 digits).
• For each service provided, the following details are required:
	o Date of service (MM-DD-YYYY).
	o Date and time data were received by the computer (MM-DD-YYYY:HH:MM:SS).
	o Member name (25 characters).
	o Member number (9 digits).
	o Service code (6 digits).
	o Fee to be paid (up to $999.99).
• Total number of consultations with members (3 digits).
• Total fee for the week (up to $99,999.99).
*/
struct ProviderReport {
	std::string provider_name;
	std::string provider_id;
	std::string address;
	std::string city;
	std::string state;
	std::string zip;

	std::vector<ServiceSummary> records;
	int num_consultations;
	float total_fee;
};

struct ManagerSummary {
	std::vector<ProviderReport> provider_reports;
	std::vector<MemberReport> member_reports;
};

class SQLEngine {
  public:

	SQLEngine(const std::string &db_path = ".dbinfo");
	bool is_connected() const;

	// Weekly Reporting
	std::vector<MemberReport> generate_member_service_reports();
	std::vector<ProviderReport> generate_provider_service_reports();
	ManagerSummary generate_manager_summary_reports();

	pqxx::connection * get_connection();

  private:

	std::unique_ptr<pqxx::connection> conn;
};

extern SQLEngine * My_DB;
