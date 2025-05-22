
#pragma once
#include <string.h>
#include <iostream>


class SQLEngine;

//address struct to replace redundant getters and setters
struct Address {
    std::string street;
    std::string city;
    std::string state;
    std::string zip;

    Address() = default;
    Address(const std::string &street,
            const std::string &city,
            const std::string &state,
            const std::string &zip)
        : street(street), city(city), state(state), zip(zip) {}
};

class Member{
    public:
        Member();
        ~Member();
        Member(const std::string & passed_member_name,
                const Address & passed_address,
                const bool & status);

        bool add_member_DB();
        bool update_member_DB();
        bool delete_member_DB();
        void Display_Member_Info();

        std::string & get_name();
        std::string & set_name(const std::string & to_set);

        std::string & get_id();
        std::string & set_id(const std::string & to_set);

        Address & get_address();
        Address & set_address(const Address & to_set);
        
        bool & get_status();
        bool & set_status(const bool & switcher);

        bool GET_MEMBER_FROM_DB(const std::string & MEMBER_ID);


    private:
        std::string member_name;
        std::string member_id;
        Address address;
        bool status;
        SQLEngine * My_DB;
};


class Provider{
    public:
        Provider();
        ~Provider();
        Provider(const std::string & passed_name,
                const Address &passed_address);

        bool add_provider_DB();
        bool update_provider_DB();
        bool delete_provider_DB();
        void Display_Provider_Info();
        
        std::string & get_name();
        std::string & set_name(const std::string & to_set);

        std::string & get_id();
        std::string & set_id(const std::string & to_set);
        
        Address & get_address();
        Address & set_address(const Address & to_set);

        /*
        std::string & get_city();
        std::string & set_city(const std::string & to_set);

        std::string & get_state();
        std::string & set_state(const std::string & to_set);

        std::string & get_zip();
        std::string & set_zip(const std::string & to_set);
        */

        bool GET_PROVIDER_FROM_DB(const std::string & PROVIDER_ID);


    private:
        std::string name;
        std::string provider_id;
        Address address;
        /*
        std::string address;
        std::string city;
        std::string State;
        std::string zip;
        */
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
        Service(const std::string & service_name,
                const std::string & service_code,
                const std::string & service_fee,
                const std::string & service_description);
        
        bool add_service();
        bool delete_service(const std::string & service_code);
        bool update_service(const std::string & service_code);

        std::string get_name();
        std::string get_fee();

    private:
        std::string service_name;
        std::string service_code;
        std::string service_fee;
        std::string service_description;   
};



