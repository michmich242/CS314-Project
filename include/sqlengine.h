// SQL API for ChocAn Database
//

#include "manager.h"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <pqxx/pqxx>
#include <sstream>

// Struct to pass back provider summaries
// Passed back in a vector
struct ProviderSummary {
	std::string provider_id;
	std::string provider_name;
	int num_consultations;
	float total_fee;
};

struct ManagerSummary {
	std::vector<ProviderSummary> providers;
	int total_consultations;
	float total_fees;
};

class SQLEngine {
  public:

	SQLEngine(const std::string &db_path = ".dbinfo");
	bool is_connected() const;

	// Member interactions
	bool add_member(const Member &member);
	bool update_member(const Member &member);
	bool delete_member(const std::string &id);
	bool validate_member(const std::string &id);

	// Provider Interactions
	bool add_provider(const Provider &provider);
	bool update_provider(const Provider &provider);
	bool delete_provider(const std::string &id);
	bool validate_provider(const std::string &id);

	// Service List and Records
	bool save_service_record(const ServiceRecord &record);
	Service get_service(const std::string &code);
	std::vector<Service> get_all_services();

	// Weekly Reporting
	std::vector<ServiceRecord> generate_member_service_reports();
	std::vector<ServiceRecord> generate_provider_service_reports();
	std::vector<ProviderSummary> generate_provider_summary_report();
	ManagerSummary generate_manager_summary_reports();

  private:

	std::unique_ptr<pqxx::connection> conn;
};
