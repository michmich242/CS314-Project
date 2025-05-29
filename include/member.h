// member.h
//
// This file contains the classes for:
//
//      - Member
//
#pragma once
#include <cstring>
#include <iostream>

class Member {
  public:

	Member();
	~Member();

	Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city,
		   const std::string &passed_zip, const bool &status);

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

	bool get_status() const;
	bool set_status(bool status);


  private:

	std::string name;
	std::string id;

	std::string address;
	std::string city;
	std::string state;
	std::string zip;

	bool status;


};
