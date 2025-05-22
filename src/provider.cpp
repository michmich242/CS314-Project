#include "../include/provider.h"
#include "utils.cpp"




bool Provider_User::login()
{
    int provider_number;
    std::string num_as_string = "";

    std::cout << "Please enter your 9 digit provider number: ";
    std::cin >> provider_number;

    while (!utils::is_valid_num(provider_number)) {
        std::cout << "Invalid input. Please try again: ";
        std::cin >> provider_number;
    }

    num_as_string = std::to_string(provider_number);

    // check if provider exists in db
    if(db.validate_provider(num_as_string)) { return true; };
    
    return false;
}

bool Provider_User::member_validation() {

//    SQLEngine db;

    int member_number;
    std::cout << "Enter member number: ";
    std::cin >> member_number;

    while(!utils::is_valid_num(member_number)){
        std::cout << "Invalid input. Please try again: ";
        std::cin >> member_number;
    }

    return db.validate_member(std::to_string(member_number));
}

bool Provider_User::member_service_billing()
{

    /*
     This builds a Provider Class Object and passes it to another function to save the information

     Re-Validate member number.
     Input date of service (MM-DD-YYYY).
     Input 6-digit service code

     Display and confirm service
     Optionally enter comments (max 100 characters).
     Display fee for the selected service.
    */
    
    Provider * provider = new Provider();
    Service * service   = new ServiceRecord();

    std::string member_number   = utils::get_member_number();
    std::string date_of_service = utils::get_user_date();
    std::string service_code    = utils::get_service_code();

    //get service returns a service object
    service = db.get_service(service_code);
    std::cout << "service name: " << service.get_name(); << endl; 

   std::string comment = utils::get_comments(); 

    std::cout << "Total fee: " << service.get_fee() << endl;

    if (update_service_records(service)) { return true; }

    return false;
}

bool Provider_User::update_service_records(const Service &record)
{
    /*
    Save a record with:
    Current date and time (MM-DD-YYYY HH:MM:SS)
    Date of service
    Provider number
    Member number
    Service code
    Previously supplied Optional Comment
    */

    if(db.save_service_record(record)) {
        return true;
    }
    return false;
}

bool Provider_User::display_service_directory()
{
    return false;
}

