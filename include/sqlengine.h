// SQL API for ChocAn Database
//
#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <string>

// Struct to pass back provider summaries
// Passed back in a vector
struct ProviderSummary {
	std::string provider_id;
	std::string privider_name;
	int num_consultations;
	float total_fee;
};

class SQLEngine {
  public:

	// Member interactions
	bool add_member(const Member &member);
	bool update_member(const Member &member);
	bool delete_member(const std::string &id);
	bool validate_member(const std::string &id);
	bool get_member_status(const std::string &id);

	// Provider Interactions
	bool add_provider(const Provider &provider);
	bool update_provider(const Provider &provider);
	bool delete_provider(const std::string &id);

	// Service List and Records
	bool save_service_record(const ServiceRecord &record);
	Service get_service(const std::string &code);
	std::vector<Service> getAllServices();

	// Weekly Reporting
	std::vector<ServiceRecord> get_service(const std::string &service_id);
	std::vector<ServiceRecord> generate_service_reports();
	std::vector<ProviderSummary> generate_provider_reports();

  private:

	pqxx::connection conn;
};
