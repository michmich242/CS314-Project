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
SQLEngine::SQLEngine(const std::string &db_path) {
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
  conn_str << "host=" << db_info["host"] << " port=" << db_info["port"]
           << " dbname=" << db_info["dbname"] << " user=" << db_info["user"]
           << " password=" << db_info["password"];

  // Attempt to connect with given key-value pairs
  try {
    // Assign Unique pointer
    conn = std::make_unique<pqxx::connection>(conn_str.str());
    if (!conn->is_open()) {
      std::cerr << "Failed to open database connection.\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "Connection Failed: " << e.what() << "\n";
  }
}

// Checks if our connection is working,
// used as an err checker in main in case
// constructor failed to connect
bool SQLEngine::is_connected() const { return conn && conn->is_open(); }

// Member interactions
bool add_member(const Member &member) {}
bool update_member(const Member &member) {}
bool delete_member(const std::string &id) {}
bool validate_member(const std::string &id) {}
bool get_member_status(const std::string &id) {}

// Provider Interactions
bool add_provider(const Provider &provider) {}
bool update_provider(const Provider &provider) {}

bool delete_provider(const std::string &id) {}
bool validate_provider(const std::string &id) {}

// Service List and Records
bool save_service_record(const ServiceRecord &record) {}
Service get_service(const std::string &code) {}
std::vector<Service> get_all_services() {}

// Weekly Reporting
std::vector<ServiceRecord> generate_member_service_reports() {}
std::vector<ServiceRecord> generate_provider_service_reports() {}
std::vector<ProviderSummary> generate_provider_summary_report() {}
ManagerSummary generate_manager_summary_reports() {}
