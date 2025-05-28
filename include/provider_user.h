#pragma once
#include "manager.h"
#include "sqlengine.h"

class Provider_User {
  public:

	Provider_User();
	~Provider_User();

	bool login(std::string&);
	bool member_validation(std::string&);
	bool member_service_billing();
	void display_service_directory();
	bool update_service_records(std::string date_of_service, std::string member_number, std::string service_code, std::string comment);

	//bool add_db();

  private:

	std::string provider_id;
	std::string service_code;
	std::string service_fee;
	SQLEngine *db;
	Provider provider;
};
