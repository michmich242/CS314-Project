// provider.h
//
// This file contains the class for:
//
//      - Provider
//
#pragma once
#include <cstring>
#include <iostream>

class Provider {
  public:

	Provider();

	Provider(const std::string &passed_name, const std::string &passed_address, 
			 const std::string &passed_city, const std::string &passed_state, const std::string &passed_zip);

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

	bool & get_status(const bool & to_set);
	bool & set_status(const bool & to_set);


	
	void Display_Provider_Info();

  private:

	std::string name;
	std::string id;

	std::string address;
	std::string city;
	std::string state;
	std::string zip;
	bool active_status;

};
