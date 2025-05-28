// service.h
//
// This file contains the class for:
//

//      - Services
//
#pragma once
#include "sqlengine.h"
#include <cstring>
#include <iostream>

class Service {
  public:

	Service();
	Service(const std::string &code, float fee, const std::string &description);

	bool add_service();
	bool delete_service(const std::string &service_code);
	bool update_service(const std::string &service_code);

	bool get_service(Service &empty_service);

  private:

	std::string code;
	float fee;
	std::string description;

	// SQL Service list retrieval

	Service get_service_DB(const std::string &service_code);
	bool add_service_DB(Service &service);
	bool remove_service_DB(Service &service);
	bool update_service_DB(Service &service);

	bool get_all_services(std::vector<Service> &services);
};
