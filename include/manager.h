// manager.h
//
// This file contains the driver for:
//
//      - Manager Terminal
//
#pragma once
#include <chrono>

#include "member.h"
#include "provider.h"
#include "service.h"
#include "service_record.h"
#include "sqlengine.h"

class Manager {
  private:

	SQLEngine &db;

	// Member input functions
	// Consider moving to util
	void get_valid_member_input(Member &member);
	void get_valid_provider_input(Provider &provider);
	void get_valid_service_input(Service &service);


  public:

	Manager(SQLEngine &db_ref) : db(db_ref)
	{
	}

	bool start_manager();

	bool add_member();
	bool update_member();
	bool delete_member();

	void display_Member_Menu();


	bool P_add_provider();
	bool P_update_provider();
	bool P_delete_provider();

	void display_Provider_Menu();


	bool S_add_service();
	bool S_update_service();
	bool S_delete_service();

	void display_Service_Menu();


	bool generate_manager_summary();
	bool generate_provider_report();
	bool generate_member_report();
	bool generate_EFT_Data();

	void display_Report_Menu();

	std::string gen_timestamp();
};
