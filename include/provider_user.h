#pragma once
#include <string>
#include <iostream>

#include "provider.h"
#include "member.h"
#include "service_record.h"
#include "service.h"

#include "sqlengine.h"

class Provider_User {
  private:
    
    SQLEngine db;
	Provider user;

  public:
    
    bool start_provider();

	Provider_User();

	bool login(std::string&);
	bool member_validation(std::string &member_ID);
	bool create_service_record();
	void display_service_directory();

	//bool add_db();
};
