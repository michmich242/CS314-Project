// service_record.h
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

class ServiceRecord {
  private:

	std::string id;
	std::string timestamp; // current date
	std::string date_of_service;
	std::string provider_id;
	std::string member_id;
	std::string service_code;
	std::string comment;


  public:

	// Constructor and Destructor
	ServiceRecord();
	ServiceRecord(std::string date_of_service, 
				  std::string timestamp,
				  std::string provider_id, 
				  std::string member_id, 
				  std::string service_code,
				  std::string comment);

	std::string &get_ID();
	std::string &get_date();
	std::string &get_timestamp();
	std::string &get_provider();
	std::string &get_member();
	std::string &get_service_code();
	std::string &get_comment();

	std::string &set_ID(const std::string &id);
	std::string &set_date(const std::string &date);
	std::string &set_timestamp(const std::string &timestamp);
	std::string &set_provider(const std::string &provider_id);
	std::string &set_member(const std::string &member_id);
	std::string &set_service_code(const std::string &service_code);
	std::string &set_comment(const std::string &comment);
};
