#include "../include/service.h"
#include "../include/utils.h"
#include <regex>

// ---------------------------------------------------------------------------------
/*Service functions start*/

Service::Service() : code(""), fee(0.0), description("")
{
}

Service::Service(const std::string &code, float fee, const std::string &description)
	: code(code), fee(fee), description(description)
{
}


std::string & Service::get_code(){
	return code;
}


std::string & Service::set_code(const std::string & to_set){
	code = to_set;
	return code;
}

float & Service::get_fee(){
	return fee;
}

float & Service::set_fee(const float & to_set){
	fee = to_set;
	return fee;
}

std::string & Service::get_description(){
	return description;
}
std::string & Service::set_description(const std::string & to_set){
	description = to_set;
	return description;
}

bool & Service::set_active_status(const bool & to_set){
	active_status = to_set;
	return active_status;
}

bool & Service::get_active_status(){
	return active_status;
}




void Service::Display_Service_Info(){
	std::cout << "----------------------------------------------------\n";
	std::cout << "	Service Description: " << description << "\n"
			  << "	Service Code: " << code << "\n"
			  << "	Fee: " << fee << "\n";
	std::cout << "----------------------------------------------------\n";
}
