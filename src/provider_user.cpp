#include "../include/provider_user.h"
#include "../include/utils.h"


Provider_User::Provider_User(): 
      provider_id(""),
      service_code(""),
      service_fee(""),
      db(nullptr),
      provider()
{
    std::cout << "Provider_User initialized..." << std::endl;
}

Provider_User::~Provider_User() {

}

bool Provider_User::login(std::string& input)
{
  //  std::cout << "Please enter your 9 digit provider number: ";
  //  std::getline(std::cin, input);

    while (!utils::is_valid_num(input)) {
        std::cout << "Invalid input. Please try again: ";
        std::getline(std::cin, input);
    }

    // check if provider exists in db
    //if(db->validate_provider(num_as_string)) { return true; };

    //std::cout << "setting provider id: " << provider.set_id(num_as_string) << std::endl;
    
    return true;
}

bool Provider_User::member_validation(std::string& member_number) {


    while(!utils::is_valid_num(member_number)){
        std::cout << "Invalid input. Please try again: ";
        std::getline(std::cin, member_number);
    }

    //return db->validate_member(std::to_string(member_number));
    return true;
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
    
    std::string member_number   = utils::get_member_number();
    std::string date_of_service = utils::get_date_of_service();
    std::string service_code    = utils::get_service_code();

    /*
    Service service = db->get_service(service_code);
    */

    //std::cout << "Service name: " << service.get_name() << std::endl;
    std::cout << "Service name: " << "not connected to db!!" << std::endl;

    std::string comment = utils::get_comments();

    //std::cout << "Total fee: " << service.get_fee() << std::endl;
    std::cout << "Total fee: " << "not connected to db!!" << std::endl;

    if (update_service_records(date_of_service, member_number, service_code, comment)) {
        return true;
    }

    return false;
}

bool Provider_User::update_service_records(std::string date_of_service, 
                                           std::string member_number, 
                                           std::string service_code, 
                                           std::string comment)
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

    std::string current_date = utils::get_current_date();
   


    //save service record takes a ServiceRecord object

    /*
    if(db.save_service_record(record)) {
        return true;
    }
    */

    std::cout << "update_service_records called. Not connected to db!!" << std::endl;

    return false;
}

void Provider_User::display_service_directory()
{
}

