#include "../include/member.h"
#include "../include/sqlengine.h"
#include "../include/utils.h"
#include <regex>

Member::Member() : name(""), id(""), address(), status(false)
{
}

Member::~Member()
{
}

Member::Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city, const std::string &passed_zip, const bool &passed_status)
	: name(passed_member_name), id(""), address(passed_address), city(passed_city), zip(passed_zip), status(passed_status)
{
}

void
Member::Display_Member_Info()
{
	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << "	Member name: " << name << std::endl
			  << "	Member ID: " << id << std::endl
			  << "	Address: " << address << std::endl
			  << "	City: " << city << std::endl
			  << "	State: " << state << std::endl
			  << "	Zip: " << zip << std::endl
			  << "	Status: " << ((status == 1) ? "True\n" : "False\n");
	std::cout << "----------------------------------------------------" << std::endl;
}


/*
bool
Member::delete_member()
{

	int member_id_test = 0;


	if (!My_DB->validate_member(member_id)) {
		return false;
	}


	if (id.length() != 9) {
		return false;
	}


	try {
		member_id_test = stoi(id);
		std::cout << member_id_test << std::endl;
	}
	catch (const std::invalid_argument &e) {
		std::cout << "Invalid argument: " << e.what() << std::endl;
		return false;
	}

	// return My_DB->delete_member(member_id);

	return false;
}
*/


std::string &
Member::get_name()
{
	return name;
}

std::string &
Member::set_name(const std::string &to_set)
{
	name = to_set;
	return name;
}

std::string &
Member::get_ID()
{
	return id;
}

std::string &
Member::set_ID(const std::string &to_set)
{
	id = to_set;
	return id;
}

std::string &
Member::get_address()
{
	return address;
}

std::string &
Member::set_address(const std::string &to_set)
{
	address = to_set;
	return address;
}

std::string &
Member::get_city()
{
	return city;
}

std::string &
Member::set_city(const std::string &to_set)
{
	city = to_set;
	return city;
}

std::string &
Member::get_state()
{
	return state;
}

std::string &
Member::set_state(const std::string &to_set)
{
	state = to_set;
	return state;
}

std::string &
Member::get_zip()
{
	return zip;
}

std::string &
Member::set_zip(const std::string &to_set)
{
	zip = to_set;
	return zip;
}

bool
Member::get_status() const
{
	return status;
}

bool
Member::set_status(bool status)
{
	this->status = status;
    return this->status;
}
