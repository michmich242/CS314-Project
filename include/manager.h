
#pragma once
#include <string.h>
#include <iostream>

class Member{
    public:
        Member();
        ~Member();
        Member(const std::string & member_name,
                const std::string & address,
                const std::string & city,
                const std::string & zip,
                const bool & status);

        bool add_member_DB();
        bool update_member_DB();
        bool delete_member_DB();

        std::string & get_mem_name();
        std::string & set_mem_name();

        std::string & get_member_ID();
        std::string & set_member_ID();

        std::string & get_member_address();
        std::string & set_member_address();

        std::string & get_member_city();
        std::string & set_member_city();

        std::string & get_member_zip();
        std::string & set_member_zip();

        bool & get_status();
        bool & set_status();

        bool & GET_MEMBER_FROM_DB(const std::string & MEMBER_ID);


    private:
        std::string member_name;
        std::string member_id;
        std::string address;
        std::string city;
        std::string State;
        std::string zip;
        bool status;
};

class Provider{
    public:
        Provider();
        ~Provider();
        Provider(const std::string & name,
                const std::string & address,
                const std::string & city,
                const std::string & zip);

        bool add_provider_DB();
        bool update_provider_DB();
        bool delete_provider_DB();


        bool & GET_PROVIDER_FROM_DB(const std::string & PROVIDER_ID);


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





