#include "../include/provider.h"
#include "../include/utils.h"
#include <regex>

//--------------------------------------------------------------------------
/* provider functions start */
Provider::Provider() : name(""), id(""), address(""), city(""), state(""), zip("")
{
}

Provider::Provider(const std::string &passed_name, const std::string &passed_address, 
				   const std::string &passed_city, const std::string &passed_state, const std::string &passed_zip)
	: name(passed_name), id(""), address(passed_address), city(passed_city), state(passed_state), zip(passed_zip)
{
}


std::string &
Provider::get_name()
{
	return name;
}

std::string &
Provider::set_name(const std::string &to_set)
{
	name = to_set;
	return name;
}

std::string &
Provider::get_ID()
{
	return id;
}

std::string &
Provider::set_ID(const std::string &to_set)
{
	id = to_set;
	return id;
}

std::string &
Provider::get_address()
{
	return address;
}

std::string &
Provider::set_address(const std::string &to_set)
{
	address = to_set;
	return address;
}

std::string &
Provider::get_city()
{
	return city;
}

std::string &
Provider::set_city(const std::string &to_set)
{
	city = to_set;
	return city;
}

std::string &
Provider::get_state()
{
	return state;
}

std::string &
Provider::set_state(const std::string &to_set)
{
	state = to_set;
	return state;
}

std::string &
Provider::get_zip()
{
	return zip;
}

std::string &
Provider::set_zip(const std::string &to_set)
{
	zip = to_set;
	return zip;
}


void Provider::Display_Provider_Info(){
	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << "	Member name: " << name << std::endl
			<< "	Member ID: " << id << std::endl
			<< "	Address: " << address << std::endl
			<< "	City: " << city << std::endl
			<< "	State: " << state << std::endl
			<< "	Zip: " << zip << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;
}
