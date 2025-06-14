/*
#ifndef UTILS_H
#define UTILS_H


#include <string>

namespace utils {
	bool is_valid_num(const std::string& num);
	std::string get_member_number();

	bool is_valid_date(const std::string& date);
	std::string get_date_of_service();

	std::string get_current_date();

	bool is_valid_service_code(const std::string& service_code);
	std::string get_service_code();

	std::string get_comments();
}

#endif // UTILS_H

*/

// Utility functions for input validation
#include <ctime>
#include <iostream>
#include <regex>
#include <string>

#include <algorithm> // for std::transform

#include <iomanip>
#include <sstream>

// #include "../include/utils.h"

namespace utils {
inline bool
is_valid_num(const std::string &num)
{

	// check for non-numeric characters
	for (char c : num) {
		if (!std::isdigit(static_cast<unsigned char>(c))) {
			std::cout << "Member number must be numeric (ex: 123456789)" << std::endl;
			return false;
		}
	}

	// check for correct length
	if (num.length() != 9) {
		std::cout << "Member number must be 9 digits.\n";
		return false;
	}
	return true;
}

inline std::string
get_member_number()
{
	// prompt user for 9 digit member number
	std::string num;

	std::cout << "enter your 9 digit member number: ";
	std::getline(std::cin, num);

	while (!is_valid_num(num)) {
		std::cout << "invalid: please enter a 9 digit number: ";
		std::getline(std::cin, num);
	}

	return num;
}

inline bool
is_valid_date(const std::string &date)
{
	std::regex date_format(R"(^(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])-\d{4}$)");
	if (!std::regex_match(date, date_format)) {
		return false;
	}

	int month = std::stoi(date.substr(0, 2));
	int day = std::stoi(date.substr(3, 2));
	int year = std::stoi(date.substr(6, 4));

	if (year < 1900 || year > 2100) {
		return false;
	}

	int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Check for leap year
	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
		days_in_month[1] = 29;
	}

	return day >= 1 && day <= days_in_month[month - 1];
}

inline std::string
get_date_of_service()
{
	std::string date;
	std::cout << "Enter the date of service (MM-DD-YYYY): ";
	std::getline(std::cin, date);

	while (!is_valid_date(date)) { // Match the function name
		std::cout << "Invlid date. Please use MM-DD-YYYY: ";
		std::getline(std::cin, date);
	}
	return date;
}

// test this
inline std::string
get_current_date()
{
	std::time_t t = std::time(nullptr);
	std::tm *now = std::localtime(&t);

	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << now->tm_mday << "-" << std::setw(2) << std::setfill('0')
		<< now->tm_mon + 1 << "-" << (now->tm_year + 1900);

	return oss.str();
}

inline bool
is_valid_service_code(const std::string &service_code)
{

	if (service_code.length() != 6) {
		return false;
	}
	return true;
}

inline std::string
get_service_code()
{
	// prompt user for service code

	std::string service_code;

	std::cout << "Enter 6 digit service code: ";
	std::getline(std::cin, service_code);

	while (!is_valid_service_code(service_code)) {
		std::cout << "Invalid input. Enter a 6 digit service code: ";
		std::getline(std::cin, service_code);
	}

	return service_code;
}

inline std::string
get_comments()
{
	std::string response;
	std::string comment;

	while (true) {
		std::cout << "Would you like to enter a comment? (yes/no): ";
		std::getline(std::cin, response);

		// Convert to lowercase
		std::transform(response.begin(), response.end(), response.begin(), ::tolower);

		if (response == "yes") {
			std::cout << "Enter your comment (max 100 characters): ";
			std::getline(std::cin, comment);

			if (comment.length() > 100) {
				std::cout << "Comment too long. Please enter up to 100 characters.\n";
				continue;
			}

			return comment;
		}
		else if (response == "no") {
			return "";
		}
		else {
			std::cout << "Invalid response. Please enter 'yes' or 'no'.\n";
		}
	}
}

} // namespace utils
