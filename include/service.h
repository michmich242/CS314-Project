// service.h
//
// This file contains the class for:
//

//      - Services
//
#pragma once
#include <cstring>
#include <iostream>

class Service {
  public:

	Service();
	Service(const std::string &code, float fee, const std::string &description);

	std::string & get_code();
	std::string & set_code(const std::string & to_set);

	float & get_fee();
	float & set_fee(const float & to_set);

	std::string & get_description();
	std::string & set_description(const std::string & to_set);

	bool get_service();

	void Display_Service_Info();

  private:

	std::string code;
	float fee;
	std::string description;
};
