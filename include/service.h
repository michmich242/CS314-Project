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

	const std::string &get_code();
	const float &get_fee();
	const std::string &get_description();

	std::string &set_code(std::string &code);
	float &set_fee(float &fee);
	std::string &set_description(std::string &description);

	bool get_service();

  private:

	std::string code;
	float fee;
	std::string description;
};
