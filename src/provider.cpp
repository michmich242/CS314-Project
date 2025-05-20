#include "../include/provider.h"
#include "utils.cpp";

bool Provider::login()
{
    SQLEngine db; //used to validate provider number
    int provider_number;

    std::cout << "Please enter your 9 digit provider number: ";
    std::cin >> provider_number;

    //TODO: Implement util class
    while (!utils::is_valid_num(provider_number)) {
        std::cout << "Invalid input. Please try again: ";
        std::cin >> provider_number;
    }

    // check if provider exists in db
    if(db.validate_provider(provider_number)) { return true; };
    
    return false;
}

bool Provider::input_new_service()
{
    return false;
}

bool Provider::update_service_records()
{
    return false;
}

bool Provider::display_service_directory()
{
    return false;
}
