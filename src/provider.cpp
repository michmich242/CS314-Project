#include "../include/provider.h"
#include "utils.cpp"




bool Provider_User::login()
{
    SQLEngine db; //used to validate provider number
    int provider_number;
    std::string provider_str = "";

    std::cout << "Please enter your 9 digit provider number: ";
    std::cin >> provider_number;

    //TODO: Implement util class
    while (!utils::is_valid_num(provider_number)) {
        std::cout << "Invalid input. Please try again: ";
        std::cin >> provider_number;
    }

    // check if provider exists in db
    provider_str = std::to_string(provider_number);
    if(db.validate_provider(provider_str)) { return true; };
    
    return false;
}


void Provider_User::display_service_directory()
{
    return;
}

