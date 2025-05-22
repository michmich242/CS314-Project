#pragma once
#include "sqlengine.h"



class Provider_User{
    public:
        Provider_User();
        ~Provider_User();
        
        bool login();
        bool member_validation();
        bool member_service_billing();
        void display_service_directory();
        bool update_service_records(std::string date_of_service, 
                                    std::string member_number, 
                                    std::string service_code, 
                                    std::string comment); 

    private:
        std::string provider_id;
        std::string service_code;
        std::string service_fee;
        SQLEngine * db;
        Provider provider;

};