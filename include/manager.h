// manager.h
//
// This file contains the driver for:
//
//      - Manager Terminal
//
#pragma once
#include "member.h"
#include "provider.h"
#include "service.h"
#include "service_record.h"

#include "sqlengine.h"

class Manager {
  private:

	SQLEngine db;

	// Member input functions
	// Consider moving to util
	void get_valid_member_input(Member &member);


  public:

	bool start_manager();

	bool add_member();
	bool update_member();
	bool delete_member();

	void display_Member_Menu();
};
