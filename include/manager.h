// manager.h
//
// This file contains the classes for:
//
//      - Manager
//      - Provider
//      - Services
//      - ServiceRecords
//
#pragma once
#include <cstring>
#include <iostream>
#include <pqxx/pqxx>

#include "sqlengine.h"

class Member {
  public:

	Member();
	~Member();
	Member(const std::string &member_name, const std::string &address, const std::string &city,
		   const std::string &state, const std::string &zip, const bool &status);

	bool add_member();
	bool update_member();
	bool delete_member();
	void Display_Member_Info();

	std::string &get_name();
	bool set_name(const std::string &to_set);

	std::string &get_ID();
	bool set_ID(const std::string &to_set);

	std::string &get_address();
	bool set_address(const std::string &to_set);

	std::string &get_city();
	bool set_city(const std::string &to_set);

	std::string &get_state();
	bool set_state(const std::string &to_set);

	std::string &get_zip();
	bool set_zip(const std::string &to_set);


	bool &get_status();
	bool set_status();

	bool GET_MEMBER_FROM_DB(const std::string &MEMBER_ID);


  private:

	std::string name;
	std::string id;
	std::string address;
	std::string city;
	std::string state;
	std::string zip;
	bool status;

	SQLEngine *My_DB;

	// Member interactions
	bool add_member_DB(Member &member);
	bool update_member_DB(Member &member);
	bool delete_member_DB(const std::string &id);
	bool validate_member_DB(const std::string &id);
};

class Provider {
  public:

	Provider();
	~Provider();
	Provider(const std::string &passed_name, const std::string &address, const std::string &city,
			 const std::string &state, const std::string &zip);

	bool add_provider();
	bool update_provider();
	bool delete_provider();
	void Display_Provider_Info();

	std::string &get_name();
	bool set_name(const std::string &to_set);

	std::string &get_ID();
	bool set_ID(const std::string &to_set);

	std::string &get_address();
	bool set_address(const std::string &to_set);


	std::string &get_city();
	bool set_city(const std::string &to_set);

	std::string &get_state();
	bool set_state(const std::string &to_set);

	std::string &get_zip();
	bool set_zip(const std::string &to_set);


	bool GET_PROVIDER_FROM_DB(const std::string &PROVIDER_ID);


  private:

	std::string name;
	std::string id;
	std::string address;
	std::string city;
	std::string state;
	std::string zip;


	// SQL Interactions
	bool add_provider_DB(Provider &provider);
	bool update_provider_DB(Provider &provider);
	bool delete_provider_DB(const std::string &id);
	bool validate_provider_DB(const std::string &id);
};

class Service {
  public:

	Service();
	~Service();
	Service(const std::string &service_code, const std::string &service_fee, const std::string &service_description);

	bool add_service();
	bool delete_service(const std::string &service_code);
	bool update_service(const std::string &service_code);

	Service get_service(const std::string &service_code);


  private:

	std::string service_code;
	std::string service_fee;
	std::string service_description;

	// SQL Service list retrieval
	std::vector<Service> get_all_services();
};

class ServiceRecord {
  private:

	std::string timestamp;
	std::string date_of_service;
	std::string provider_id;
	std::string member_id;
	std::string service_code;
	std::string comment;

	pqxx::connection *conn;

  public:

	// Constructor and Destructor
	ServiceRecord();
	~ServiceRecord();

	// DB Operation
	bool save_service_record(ServiceRecord &record);
};
