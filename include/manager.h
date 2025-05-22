
#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <string.h>

#include "manager.h"
#include "provider.h"
#include "sqlengine.h"

// address struct to replace redundant getters and setters
struct Address {
	std::string street;
	std::string city;
	std::string state;
	std::string zip;

	Address() = default;

	Address(const std::string &street, const std::string &city, const std::string &state, const std::string &zip) : street(street), city(city), state(state), zip(zip)
	{
	}
};

class Member {
  public:

	Member();
	~Member();
	Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city, const std::string &passed_zip, const bool &status);

	bool add_member_DB();
	bool update_member_DB();
	bool delete_member_DB();
	void Display_Member_Info();

	std::string &get_name();
	std::string &set_name(const std::string &to_set);

	std::string &get_ID();
	std::string &set_ID(const std::string &to_set);

	std::string &get_address();
	std::string &set_address(const std::string &to_set);

	std::string &get_city();
	std::string &set_city(const std::string &to_set);

	std::string &get_state();
	std::string &set_state(const std::string &to_set);

	std::string &get_zip();
	std::string &set_zip(const std::string &to_set);

	bool &get_status();
	bool &set_status(const bool &switcher);

	bool GET_MEMBER_FROM_DB(const std::string &MEMBER_ID);


  private:

	std::string member_name;
	std::string member_id;
	std::string address;
	std::string city;
	std::string State;
	std::string zip;
	bool status;

	SQLEngine *My_DB;

	// Member interactions
	bool add_member(const Member &member);
	bool update_member(const Member &member);
	bool delete_member(const std::string &id);
	bool validate_member(const std::string &id);
};

class Provider {
  public:

	Provider();
	~Provider();
	Provider(const std::string &passed_name, const Address &passed_address);

	bool add_provider_DB();
	bool update_provider_DB();
	bool delete_provider_DB();
	void Display_Provider_Info();

	std::string &get_name();
	std::string &set_name(const std::string &to_set);

	std::string &get_id();
	std::string &set_id(const std::string &to_set);

	Address &get_address();
	Address &set_address(const Address &to_set);

	/*
	std::string & get_city();
	std::string & set_city(const std::string & to_set);

	std::string & get_state();
	std::string & set_state(const std::string & to_set);

	std::string & get_zip();
	std::string & set_zip(const std::string & to_set);
	*/

	bool GET_PROVIDER_FROM_DB(const std::string &PROVIDER_ID);


  private:

	std::string name;
	std::string provider_id;
	Address address;
	/*
	std::string address;
	std::string city;
	std::string State;
	std::string zip;
	*/


	// SQL Interactions
	bool add_provider(const Provider &provider);
	bool update_provider(const Provider &provider);
	bool delete_provider(const std::string &id);
	bool validate_provider(const std::string &id);
};

class Service {
  public:

	Service();
	~Service();
	Service(const std::string &service_code, const std::string &service_fee, const std::string &service_description);

	bool add_service();
	bool delete_service(const std::string &service_code);
	bool update_service(const std::string &service_code);


  private:

	std::string service_code;
	std::string service_fee;
	std::string service_description;

	// SQL Service list retrieval
	std::vector<Service> get_all_services();
};
