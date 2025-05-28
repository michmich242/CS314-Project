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
#include "sqlengine.h"
#include <cstring>
#include <iostream>

class ServiceRecord {
  private:

	std::string timestamp;
	std::string date_of_service;
	std::string provider_id;
	std::string member_id;
	std::string service_code;
	std::string comment;


	// DB Operation
	bool save_record_DB(ServiceRecord &record);

  public:

	// Constructor and Destructor
	ServiceRecord();

	std::string &get_date();
	std::string &get_provider();
	std::string &get_member();
	std::string &get_service_code();
	std::string &get_comment();


	std::string &set_date(const std::string &date);
	std::string &set_provider(const std::string &provider_id);
	std::string &set_member(const std::string &member_id);
	std::string &set_service_code(const std::string &service_code);
	std::string &set_comment(const std::string &comment);
};
