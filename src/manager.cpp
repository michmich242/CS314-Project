#include "../include/manager.h"
#include "../include/sqlengine.h"
#include "utils.cpp"




Member::Member(): member_name(""), member_id(""), address(""), city(""), State(""), zip(""), status(false), My_DB(nullptr){
    My_DB = new SQLEngine;
}

Member::~Member(){
    delete My_DB;
    My_DB = nullptr;
}

Member::Member(const std::string & passed_member_name,
    const std::string & passed_address,
    const std::string & passed_city,
    const std::string & passed_zip,
    const bool & passed_status): member_name(passed_member_name), member_id(""), address(passed_address), city(passed_city), zip(passed_zip), status(passed_status){

    My_DB = new SQLEngine;
}


void Member::Display_Member_Info(){
    std::cout << "Member name: " << member_name << std::endl
    << "Member ID: " << member_id << std::endl
    << "Address " << address << std::endl
    << "City " << city << std::endl
    << "State " << State << std::endl
    << "Zip " << zip << std::endl
    << "Status " << ((status == 1) ? "True\n" : "False\n");
}


bool Member::add_member_DB(){

    return My_DB->add_member(*this);
}

bool Member::update_member_DB(){

    int member_id_test = 0;


    if(!My_DB->validate_member(member_id)){
        return false;
    }

    if(member_id.length() != 9){
        return false;
    }


    try{
        member_id_test = stoi(member_id);
        std::cout << member_id_test << std::endl;
    }
    catch (const std::invalid_argument & e){
        std::cout << "Invalid argument: " << e.what() << std::endl;
        return false;
    }

/*
    if(!utils::is_valid_num(member_id_test)){
        return false;
    }
*/
    
    return My_DB->update_member(*this);
}

bool Member::delete_member_DB(){
    
  int member_id_test = 0;


    if(!My_DB->validate_member(member_id)){
        return false;
    }

    if(member_id.length() != 9){
        return false;
    }


    try{
        member_id_test = stoi(member_id);
        std::cout << member_id_test << std::endl;
    }
    catch (const std::invalid_argument & e){
        std::cout << "Invalid argument: " << e.what() << std::endl;
        return false;
    }


    return My_DB->delete_member(member_id);

}







std::string & Member::get_mem_name(){
    return member_name;
}

std::string & Member::set_mem_name(const std::string & to_set){
    member_name = to_set;
    return member_name;
}

std::string & Member::get_member_ID(){
    return member_id;
}

std::string & Member::set_member_ID(const std::string & to_set){
    member_id = to_set;
    return member_id;
}


std::string & Member::get_member_address(){
    return address;
}

std::string & Member::set_member_address(const std::string & to_set){
    address = to_set;
    return address;
}

std::string & Member::get_member_city(){
    return city;
}

std::string & Member::set_member_city(const std::string & to_set){
    city = to_set;
    return city;
}

std::string & Member::get_member_state(){
    return State;
}

std::string & Member::set_member_state(const std::string & to_set){
    State = to_set;
    return State;
}

std::string & Member::get_member_zip(){
    return zip;
}

std::string & Member::set_member_zip(const std::string & to_set){
    zip = to_set;
    return zip;
}

bool & Member::get_status(){
    return status;
}

bool & Member::set_status(const bool & switcher){
    status = switcher;
    return status;
}

bool Member::GET_MEMBER_FROM_DB(const std::string & MEMBER_ID){

    return false;
}

/* provider functions start */

Provider::Provider(): name(""), provider_id(""), address() {

}

Provider::~Provider(){

}

Provider::Provider(const std::string & passed_name,
    const Address & passed_address)
    : name(passed_name), provider_id(""), address(passed_address) {

}

bool Provider::add_provider_DB(){
    return false;
}

bool Provider::update_provider_DB(){
    return false;
}

bool delete_provider_DB(){
    return false;
}

std::string & Provider::get_name(){
    return name;
}

std::string & Provider::set_name(const std::string & to_set){
    name = to_set;
    return name;
}

std::string & Provider::get_id(){
    return provider_id;
}

std::string & Provider::set_id(const std::string & to_set){
    provider_id = to_set;
    return provider_id;
}

Address & Provider::get_address(){
    return address;
}


Address & Provider::set_address(const Address & to_set){
    address = to_set;
    return address;
}

/*
std::string & Provider::get_provider_city(){
    return city;
}

std::string & Provider::set_provider_city(const std::string & to_set){
    city = to_set;
    return city;
}

std::string & Provider::get_provider_State(){
    return State;
}

std::string & Provider::set_provider_State(const std::string & to_set){
    State = to_set;
    return State;
}

std::string & Provider::get_provider_zip(){
    return zip;
}

std::string & Provider::set_provider_zip(const std::string & to_set){
    zip = to_set;
    return zip;
}
*/

bool Provider::GET_PROVIDER_FROM_DB(const std::string & PROVIDER_ID){

    return false;
}

/*ServiceRecord functions start*/

ServiceRecord::ServiceRecord(): timestamp(""), 
                                date_of_service(""), 
                                provider_id(""),
                                member_id(""),
                                comment("")
{
    std::cout << "initializing ServiceRecord object..." << std::endl;
}

ServiceRecord::~ServiceRecord() {

} 

/*Service functions start*/

Service::Service(): service_code(""),
                    service_fee(""),
                    service_description("")
{
    std::cout << "initializing Service object..." << std::endl;
}