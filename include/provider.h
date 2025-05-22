#pragma once
#include "sqlengine.h"



class Provider_User{
    public:
        Provider_User();
        ~Provider_User();
        
        bool login();
        void display_service_directory();
        bool Post_Service_Billing();



    private:
        std::string provider_id;
        std::string service_code;
        std::string service_fee;

};