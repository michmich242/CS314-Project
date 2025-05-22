//Utility functions for input validation

namespace utils {
    bool is_valid_num(int num) {
        if (num < 100000000 || num > 999999999) {
            return false;
        }
        return true;
    }

    std::string get_member_number() {
        //prompt user for 9 digit member number
        //perform checks
        //return number
    }

    std::string get_date_of_service() {
        //prompt user for date of service
        //sanitize and perform validation
        //return date
    }

    std::string get_current_date() {
        //return the current date
    }

    std::string get_service_code() {
        //prompt user for service code
        //validate user input
        //return service code
    }

    std::string get_comments() {
        //Enter comment?
                //validate input
        //if yes, enter comment and return comment
                //validate input
        //if no, return empty string
    }

}