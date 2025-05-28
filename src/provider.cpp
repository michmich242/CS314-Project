#include "../include/provider.h"
#include "../include/sqlengine.h"
#include "../include/utils.h"
#include <regex>

//--------------------------------------------------------------------------
/* provider functions start */



Provider::Provider() : name(""), id(""), address(""), city(""), state(""), zip("")
{
}

Provider::Provider(const std::string &passed_name, const std::string &passed_address, const std::string &passed_city,
				   const std::string &passed_zip)
	: name(passed_name), id(""), address(passed_address), city(passed_city), zip(passed_zip)
{
}

bool
Provider::add_provider()
{
	return false;
}

bool
Provider::update_provider()
{
	return false;
}

bool
delete_provider()
{
	return false;
}

std::string &
Provider::get_name()
{
	return name;
}

std::string &
Provider::set_name(const std::string &to_set)
{
	name = to_set;
	return name;
}

std::string &
Provider::get_ID()
{
	return id;
}

std::string &
Provider::set_ID(const std::string &to_set)
{
	id = to_set;
	return id;
}

std::string &
Provider::get_address()
{
	return address;
}

std::string &
Provider::set_address(const std::string &to_set)
{
	address = to_set;
	return address;
}

std::string &
Provider::get_city()
{
	return city;
}

std::string &
Provider::set_city(const std::string &to_set)
{
	city = to_set;
	return city;
}

std::string &
Provider::get_state()
{
	return state;
}

std::string &
Provider::set_state(const std::string &to_set)
{
	state = to_set;
	return state;
}

std::string &
Provider::get_zip()
{
	return zip;
}

std::string &
Provider::set_zip(const std::string &to_set)
{
	zip = to_set;
	return zip;
}

bool
Provider::GET_PROVIDER_FROM_DB(const std::string &PROVIDER_ID)
{
	return false;
}

//----------------------------------------------------------------------
// Provider SQL

// Add Provider to db
// @PARAM: Provider &provider : Populated Provider Object
// provider.provider_id WILL BE OVERWRITTEN
bool
Provider::add_provider_DB(Provider &provider)
{
	// Check Connection
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());

		try {
			auto exists = transaction.query_value<int>(
				pqxx::zview("SELECT 1 FROM chocan.providers WHERE provider_id = $1"), pqxx::params{provider.get_ID()});

			std::cerr << "Provider with ID: " << provider.get_ID() << "already exists\n";
			return false;
		}
		catch (const pqxx::unexpected_rows &) {
			// provider doesnt exist
		}
		// Run Query
		std::string new_provider_id = transaction.query_value<std::string>(
			pqxx::zview("INSERT INTO chocan.providers (name, address, city, state_abbrev, zip, active_status))"
						"VALUES ($1, $2, $3, $4, $5, $6)"),
			pqxx::params{get_name(), get_address(), get_city(), get_state(), get_zip(), true});

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
Provider::update_provider_DB(Provider &provider)
{
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());
		pqxx::result res = transaction.exec_params(
			R"(UPDATE chocan.providers 
			SET name = $1, address = $2, city = $3, zip = $4, state_abbrev = $5 
			WHERE provider_id = $6)",

			// variables being passed to $#
			pqxx::params{get_name(), get_address(), get_city(), get_zip(), get_state(), get_ID()});

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
Provider::delete_provider_DB(const std::string &id)
{
	if (!My_DB || !My_DB->is_connected()) {
		std::cerr << "db My_DBection not open\n";
		return false;
	}

	try {
		// Start a transaction
		pqxx::work transaction(My_DB->get_connection());
		pqxx::result res;

		// Run Query
		res = transaction.exec_params("DELETE FROM chocan.providers WHERE provider_id = $1", id);

		// Ensure a row was deleted
		if (res.affected_rows() == 0) {
			std::cerr << "No member found with ID: " << id << "\n";
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

// End of Provider Functions----------------------------------------------------

//----------------------------------------------------------------------
