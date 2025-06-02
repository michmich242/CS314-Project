#pragma once
#include <iostream>
#include <string>

#include "member.h"
#include "provider.h"
#include "service.h"
#include "service_record.h"

#include "sqlengine.h"

class Provider_User {
  private:

	SQLEngine &db;
	Provider user;

  public:

	Provider_User() = delete;
	Provider_User(SQLEngine &db_ref);

	bool start_provider();

	bool login(std::string &);
	bool member_validation_wrapper();
	bool member_validation(std::string &member_ID);
	bool create_service_record();
	bool generate_service_directory();
	void display_service_directory();
};
