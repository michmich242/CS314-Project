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

Member::Member(const std::string &passed_member_name, const std::string &passed_address, const std::string &passed_city,
			   const std::string &passed_state, const std::string &passed_zip, const bool &passed_status)
	: name(passed_member_name), id(""), address(passed_address), city(passed_city), state(passed_state),
	  zip(passed_zip), status(passed_status)
{
}

void
Member::Display_Member_Info()
{
	std::cout << "----------------------------------------------------\n";
	std::cout << "	Member name: " << name << "\n"
			  << "	Member ID: " << id << "\n"
			  << "	Address: " << address << "\n"
			  << "	City: " << city << "\n"
			  << "	State: " << state << "\n"
			  << "	Zip: " << zip << "\n"
			  << "	Status: " << ((status) ? "True\n" : "False\n");
	std::cout << "----------------------------------------------------\n";
}

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
