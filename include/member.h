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

class Member {
  public:

	Member();
	~Member();

	Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city,
		   const std::string &passed_zip, const bool &status);

	bool add_member();
	bool update_member();
	bool delete_member();

	void Display_Member_Info();
	void display_Member_Menu();

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


	bool get_status() const;
	bool set_status();

	bool GET_MEMBER_FROM_DB(pqxx::work * transaction);


  private:

	std::string name;
	std::string id;

	std::string address;
	std::string city;
	std::string state;
	std::string zip;

	bool status;


	// Member interactions
	bool add_member_DB(Member &member);
	bool update_member_DB(Member &member);
	bool delete_member_DB(const std::string &id);
	bool validate_member_DB(const std::string &id);

	void get_valid_member_input(int check);
};
