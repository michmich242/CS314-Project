#include "../include/service_record.h"
#include "../include/sqlengine.h"
#include "../include/utils.h"
#include <regex>

//----------------------------------------------------------------------

/*ServiceRecord functions start*/

ServiceRecord::ServiceRecord()
	: id(""), timestamp(""), date_of_service(""), provider_id(""), member_id(""), service_code(""), comment("")
{
}

ServiceRecord::ServiceRecord(std::string date_of_service, std::string timestamp, std::string provider_id,
							 std::string member_id, std::string service_code, std::string comment)
	: id(""), timestamp(timestamp), date_of_service(date_of_service), provider_id(provider_id), member_id(member_id),
	  service_code(service_code), comment(comment)
{
}

//
// Accessors ---------------------------------------------------------------------------
//
std::string &
ServiceRecord::get_ID()
{
	return id;
}

std::string &
ServiceRecord::get_date()
{
	return date_of_service;
}

std::string &
ServiceRecord::get_timestamp()
{
	return timestamp;
}

std::string &
ServiceRecord::get_provider()
{
	return provider_id;
}

std::string &
ServiceRecord::get_member()
{
	return member_id;
}

std::string &
ServiceRecord::get_service_code()
{
	return service_code;
}

std::string &
ServiceRecord::get_comment()
{
	return comment;
}

//
// Mutators ---------------------------------------------------------------------------
//
std::string &
ServiceRecord::set_ID(const std::string &id)
{
	this->id = id;
	return this->id;
}

std::string &
ServiceRecord::set_date(const std::string &date)
{
	this->date_of_service = date;
	return this->date_of_service;
}

std::string &
ServiceRecord::set_timestamp(const std::string &timestamp)
{
	this->timestamp = timestamp;
	return this->timestamp;
}

std::string &
ServiceRecord::set_provider(const std::string &provider_id)
{
	this->provider_id = provider_id;
	return this->provider_id;
}

std::string &
ServiceRecord::set_member(const std::string &member_id)
{
	this->member_id = member_id;
	return this->member_id;
}

std::string &
ServiceRecord::set_service_code(const std::string &service_code)
{
	this->service_code = service_code;
	return this->service_code;
}

std::string &
ServiceRecord::set_comment(const std::string &comment)
{
	this->comment = comment;
	return this->comment;
}
