#include "../include/service.h"
#include "../include/utils.h"
#include <regex>

// ---------------------------------------------------------------------------------
/*Service functions start*/

Service::Service() : code(""), fee(0.0), description("")
{
	std::cout << "initializing Service object..." << std::endl;
}

Service::Service(const std::string &code, float fee, const std::string &description)
	: code(code), fee(fee), description(description)
{
}

const std::string &
Service::get_code()
{
	return code;
}

std::string &
Service::set_code(std::string &code)
{
	this->code = code;
	return this->code;
}

const float &
Service::get_fee()
{
	return fee;
}

float &
Service::set_fee(float &fee)
{
	this->fee = fee;
	return this->fee;
}

const std::string &
Service::get_description()
{
	return description;
}

std::string &
Service::set_description(std::string &description)
{
	this->description = description;
	return this->description;
}
