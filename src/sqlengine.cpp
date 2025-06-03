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
		std::cerr << "Failed to retrieve database info.\n";
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


// Weekly Reporting
bool
SQLEngine::generate_member_service_reports(std::vector<MemberReport> &reports)
{
	if (is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		reports.clear();

		auto result = transaction.exec(R"(
                SELECT m.member_id, m.name, m.address, m.city, m.state, m.zip, sr.date_of_service,
                            p.name AS provider_name, s.description AS service_name
                FROM service_records sr
                INNER JOIN members m ON sr.member_id = m.member_id
                INNER JOIN providers p ON sr.provider_id = p.provider_id
                INNER JOIN services s ON sr.service_code = s.service_code
                ORDER BY m.member_id, sr.date_of_service
        )");
		// Condition to only have last 7 days
		// WHERE sr.date_of_service >= CURRENT_DATE - INTERVAL '7 days'


		// key_value pairing for each unique member_id
		std::unordered_map<std::string, MemberReport> report_map;
		for (const auto &row : result) {
			std::string member_id = row["member_id"].as<std::string>();

			// New Member ? Add to map
			if (report_map.find(member_id) == report_map.end()) {
				MemberReport report;
				report.member_id = member_id;
				report.member_name = row["name"].as<std::string>();
				report.address = row["address"].as<std::string>();
				report.city = row["city"].as<std::string>();
				report.state = row["state"].as<std::string>();
				report.zip = row["zip"].as<std::string>();
				report_map[member_id] = std::move(report);
			}

			// Member_id seen before ? add to map
			report_map[member_id].services.push_back({row["date_of_service"].as<std::string>(),
													  row["provider_name"].as<std::string>(),
													  row["service_name"].as<std::string>()});
		}

		// drop member_id key, only save value in reports
		for (auto &[_, report] : report_map) {
			reports.push_back(std::move(report));
		}

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Creating Member Reports: " << e.what() << "\n";
		return false;
	}
}

bool
SQLEngine::generate_provider_service_reports(std::vector<ProviderReport> &reports)
{
	if (!conn || !conn->is_open()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		reports.clear();

		auto result = transaction.exec(R"(
                SELECT p.provider_id, p.name, p.address, p.city, p.state, p.zip, 
                       m.name as member_name, sr.date_of_service, sr.timestamp_received,
                       m.member_id, sr.service_code, s.description as service_name, s.fee
                FROM service_records sr
                JOIN providers p ON sr.provider_id = p.provider_id
                JOIN members m ON sr.member_id = m.member_id
                JOIN services s ON sr.service_code = s.service_code
                ORDER BY p.provider_id, sr.timestamp_received
        )");
		// Condition to only have last 7 days
		// WHERE sr.date_of_service >= CURRENT_DATE - INTERVAL '7 days'


		// key_value pairing for each unique Provider_id
		std::unordered_map<std::string, ProviderReport> report_map;
		for (const auto &row : result) {
			std::string provider_id = row["member_id"].as<std::string>();

			// New Provider ? Add to map
			if (report_map.find(provider_id) == report_map.end()) {
				ProviderReport report;
				report.provider_name = row["name"].as<std::string>();
				report.provider_id = provider_id;
				report.address = row["address"].as<std::string>();
				report.city = row["city"].as<std::string>();
				report.state = row["state"].as<std::string>();
				report.zip = row["zip"].as<std::string>();
				report.num_consultations = 0;
				report.total_fee = 0.0F;

				report_map[provider_id] = std::move(report);
			}

			// Provider_id seen before ? add to map
			report_map[provider_id].services.push_back({row["date_of_service"].as<std::string>(),
														row["member_name"].as<std::string>(),
														row["service_name"].as<std::string>()});
			report_map[provider_id].num_consultations++;
			report_map[provider_id].total_fee += row["fee"].as<float>();
		}

		// drop provider_id key, only save value in reports
		for (auto &[_, report] : report_map) {
			reports.push_back(std::move(report));
		}

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error inserting member: " << e.what() << "\n";
		return false;
	}
}

bool
SQLEngine::generate_manager_summary_reports(std::vector<ManagerSummary> &summaries)
{
	if (!is_connected()) {
		std::cerr << "db connection not open\n";
		return false;
	}

	try {
		pqxx::work transaction(get_connection());

		summaries.clear();

		auto res = transaction.exec(R"(
            SELECT p.provider_id, p.name, p.address, p.city, p.state, p.zip,
                   m.member_id, m.name, sr.date_of_service, sr.timestamp_received,
                   sr.service_code, s.description, s.fee, sr.record_id
            FROM service_records sr
            JOIN providers p ON sr.provider_id = p.provider_id
            JOIN members m ON sr.member_id = m.member_id
            JOIN services s ON sr.service_code = s.service_code
            ORDER BY p.provider_id, sr.timestamp_received
        )");

		std::map<std::string, ProviderSummary> provider_map;
		int total_consultations = 0;
		float total_fees = 0.0F;

		for (const auto &row : res) {
			std::string provider_id = row["provider_id"].as<std::string>();

			// check if provider_id in summary
			if (provider_map.find(provider_id) == provider_map.end()) {
				Provider provider(row["name"].as<std::string>(), row["address"].as<std::string>(),
								  row["city"].as<std::string>(), row["state"].as<std::string>(),
								  row["zip"].as<std::string>());


				provider_map[provider_id] =
					ProviderSummary{.provider = provider, .records = {}, .num_consultations = 0, .total_fee = 0.0};
			}

			ServiceRecord record(row["date_of_service"].as<std::string>(), row["timestamp_received"].as<std::string>(),
								 row["provider_id"].as<std::string>(), row["member_id"].as<std::string>(),
								 row["service_code"].as<std::string>(), row["description"].as<std::string>());

			record.set_ID(row["record_id"].as<std::string>());

			auto &summary = provider_map[provider_id];
			summary.records.push_back(record);
			summary.num_consultations += 1;
			summary.total_fee += row["fee"].as<float>();

			total_consultations += 1;
			total_fees += row["fee"].as<float>();
		}

		ManagerSummary manager_summary;
		manager_summary.total_consultations = total_consultations;
		manager_summary.total_fees = total_fees;

		for (auto &[id, summary] : provider_map) {
			manager_summary.summaries.push_back(std::move(summary));
		}

		summaries.push_back(std::move(manager_summary));
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error generating manager summary: " << e.what() << "\n";
		return false;
	}
}

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

		std::string new_member_id = transaction.query_value<std::string>(
			pqxx::zview("INSERT INTO chocan.MEMBERS (name, address, city, state_abbrev, zip, active_status) "
						"VALUES ($1, $2, $3, $4, $5, $6) RETURNING member_id"),
			pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_state(),
							member.get_zip(), true});

		member.set_ID(new_member_id);

		transaction.commit();
		return true;
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
							 pqxx::params{member.get_name(), member.get_address(), member.get_city(), member.get_zip(),
										  member.get_state(), member.get_status(), member.get_ID()});

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
			bool status = transaction.query_value<bool>(
				pqxx::zview("SELECT active_status FROM chocan.members WHERE member_id = $1"), pqxx::params{id});
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

		// Run Query
		std::string new_provider_id = transaction.query_value<std::string>(
			pqxx::zview("INSERT INTO chocan.providers (name, address, city, state_abbrev, zip)"
						" VALUES ($1, $2, $3, $4, $5) RETURNING provider_id"),
			pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(), provider.get_state(),
						 provider.get_zip()});

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
							 pqxx::params{provider.get_name(), provider.get_address(), provider.get_city(),
										  provider.get_zip(), provider.get_state(), provider.get_ID()});

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
SQLEngine::get_provider(Provider &provider)

{
	if (!is_connected()) {
		std::cout << "DB connection failed\n";
		return false;
	}
	try {
		pqxx::work transaction(get_connection());
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


		return true;
	}
	catch (const std::exception &e) {
		std::cout << "Error retrieving member: " << e.what() << "\n";
		return false;
	}
}

bool
SQLEngine::validate_provider(const std::string &provider_id)
{
	if (!is_connected()) {
		std::cout << "DB connection failed\n";
		return false;
	}

	try {
		pqxx::work transaction(*conn);
		pqxx::result res = transaction.exec(pqxx::zview(R"(SELECT 1 
            FROM chocan.providers 
            WHERE provider_id = $1)"),
											pqxx::params{provider_id});

		if (res.empty()) {
			std::cout << "No provider found with ID: " << provider_id << "\n";
			return false;
		}

		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Database error in verify_provider_exists: " << e.what() << "\n";
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
bool
SQLEngine::get_service(Service & service)
{
	if (!is_connected()) {
		std::cout << "DB connection failed\n";
		return false;
	}
	try {
		pqxx::work transaction(get_connection());
		pqxx::result res = transaction.exec(pqxx::zview(R"(SELECT description, fee
			FROM chocan.services 
			WHERE service_code = $1)"),
											pqxx::params{service.get_code()});

		if (res.empty()) {
			std::cout << "No service code found with: " << service.get_code() << "\n";
			return false;
		}

		service.set_description(res[0][0].c_str());
		service.set_fee(std::stof(res[0][1].c_str()));


		return true;
	}
	catch (const std::exception &e) {
		std::cout << "Error retrieving member: " << e.what() << "\n";
		return false;
	}
}

// Query db for all services
bool
SQLEngine::get_all_services(std::vector<Service> &services)
{
	// Check Conn
	if (!is_connected()) {
		std::cerr << "db not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());

		services.clear();

		// Run Query
		for (auto [code, description, fee] : transaction.query<std::string, std::string, float>(
				 "SELECT service_code, description, fee FROM chocan.services ORDER BY service_code")) {
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
// **CURRENT DATE is handled automatically by db **
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
		std::string new_id = transaction.query_value<std::string>(
			pqxx::zview(R"(INSERT INTO chocan.Service_Records
						  (date_of_service, provider_id, member_id, service_code, comments)
            			  VALUES($1, $2, $3, $4, $5) RETURNING service_code)"),
			pqxx::params{record.get_date(), record.get_provider(), record.get_member(), record.get_service_code(),
						 record.get_comment()});

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

bool
SQLEngine::add_service(Service &service)
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
		std::string returning_code =  transaction.query_value<std::string>(
			pqxx::zview(R"(INSERT INTO chocan.services
						  (description, fee)
            			  VALUES($1, $2) RETURNING service_code)"),
			pqxx::params{service.get_description(), service.get_fee()});

		// Finalize transaction
		service.set_code(returning_code);
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error adding service: " << e.what() << "\n";
		return false;
	}
}

bool
SQLEngine::update_service(Service &service)
{
		if (!is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(get_connection());
		pqxx::result res =
			transaction.exec(pqxx::zview(R"(UPDATE chocan.services 
			                SET description = $1, fee = $2
                			WHERE service_code = $3)"),

							 // variables being passed to $#
							 pqxx::params{service.get_description(), service.get_fee(), service.get_code()});

		// Check if query modified a row
		if (res.affected_rows() == 0) {
			std::cerr << "No service code found with: " << service.get_code() << "\n";
			return false;
		}

		// Finalize Transaction
		transaction.commit();
		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error Updating service: " << e.what() << "\n";
		return false;
	}
}

bool
delete_service(const std::string &code)
{
}
