
#pragma once
#include <string.h>
#include <iostream>


class SQLEngine;


class Member{
    public:
        Member();
        ~Member();
        Member(const std::string & passed_member_name,
                const std::string & passed_address,
                const std::string & passed_city,
                const std::string & passed_zip,
                const bool & status);

        bool add_member_DB();
        bool update_member_DB();
        bool delete_member_DB();
        void Display_Member_Info();

        std::string & get_mem_name();
        std::string & set_mem_name(const std::string & to_set);

        std::string & get_member_ID();
        std::string & set_member_ID(const std::string & to_set);

        std::string & get_member_address();
        std::string & set_member_address(const std::string & to_set);

        std::string & get_member_city();
        std::string & set_member_city(const std::string & to_set);

        std::string & get_member_state();
        std::string & set_member_state(const std::string & to_set);

        std::string & get_member_zip();
        std::string & set_member_zip(const std::string & to_set);

        bool & get_status();
        bool & set_status(const bool & switcher);

        bool GET_MEMBER_FROM_DB(const std::string & MEMBER_ID);


    private:
        std::string member_name;
        std::string member_id;
        std::string address;
        std::string city;
        std::string State;
        std::string zip;
        bool status;
        SQLEngine * My_DB;
};

class Provider{
    public:
        Provider();
        ~Provider();
        Provider(const std::string & passed_name,
                const std::string & passed_address,
                const std::string & passed_city,
                const std::string & passed_zip);

        bool add_provider_DB();
        bool update_provider_DB();
        bool delete_provider_DB();
        void Display_Provider_Info();
        
        std::string & get_provider_name();
        std::string & set_provider_name(const std::string & to_set);

        std::string & get_provider_id();
        std::string & set_provider_id(const std::string & to_set);
        
        std::string & get_provider_address();
        std::string & set_provider_address(const std::string & to_set);

        std::string & get_provider_city();
        std::string & set_provider_city(const std::string & to_set);

        std::string & get_provider_State();
        std::string & set_provider_State(const std::string & to_set);

        std::string & get_provider_zip();
        std::string & set_provider_zip(const std::string & to_set);

        bool GET_PROVIDER_FROM_DB(const std::string & PROVIDER_ID);


    private:
        std::string name;
        std::string provider_id;
        std::string address;
        std::string city;
        std::string State;
        std::string zip;
};


class ServiceRecord{
    public:
        ServiceRecord();
        ~ServiceRecord();

        bool Member_Report(const Member & Member_Info);
        bool Provider_Report(const Provider & Provider_Info);
        bool Manager_Report(const Provider & Provider_Info);

    private:
        std::string timestamp;
        std::string date_of_service;
        std::string provider_id;
        std::string member_id;
        std::string comment;
};



class Service{
    public:
        Service();
        ~Service();
        Service(const std::string & service_code,
                const std::string & service_fee,
                const std::string & service_description);
        
        bool add_service();
        bool delete_service(const std::string & service_code);
        bool update_service(const std::string & service_code);


    private:
        std::string service_code;
        std::string service_fee;
        std::string service_description;   
};



