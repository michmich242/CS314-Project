// SQL API for ChocAn Database
//

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <pqxx/params>
#include <pqxx/pqxx>
#include <sstream>

#include "member.h"
#include "provider.h"
#include "service.h"
#include "service_record.h"

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

	std::vector<ServiceSummary> services;
	int num_consultations;
	float total_fee;
};

// Manager Summary Structs
struct ProviderSummary {
	Provider provider;
	std::vector<ServiceRecord> records;
	int num_consultations;
	float total_fee;
};

struct ManagerSummary {
	std::vector<ProviderSummary> summaries;
	int total_consultations;
	float total_fees;
};

class SQLEngine {
  public:

	SQLEngine();
	bool is_connected() const;
	pqxx::connection &get_connection();

	// Manager Terminal
	// Weekly Reporting
	bool generate_member_service_reports(std::vector<MemberReport> &);
	bool generate_provider_service_reports(std::vector<ProviderReport> &);
	bool generate_manager_summary_reports(std::vector<ManagerSummary> &);


	// Member Class Functions
	bool add_member(Member &member);
	bool update_member(Member &member);
	bool delete_member(const std::string &id);
	bool get_member(Member &member);
	bool validate_member(const std::string &id);

	// Provider Class Functions
	// SQL Interactions
	bool add_provider(Provider &provider);
	bool update_provider(Provider &provider);
	bool delete_provider(const std::string &id);
	bool get_provider(Provider &provider);
	bool validate_provider(const std::string &provider_id);

	// Service Directory Functions
	Service get_service(const std::string &code);

	bool add_service(Service & service);
	bool update_service(Service & service);
	bool delete_service(const std::string & code);

	bool get_all_services(std::vector<Service> &services);


	// Provider Terminal Save Service Record
	bool save_service_record(ServiceRecord &record);

  private:

	bool conn_db(const std::string &db_path = ".dbinfo");
	std::unique_ptr<pqxx::connection> conn;
};
