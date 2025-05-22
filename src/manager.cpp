#include "../include/manager.h"




Member::Member(): member_name(""), member_id(""), address(""), city(""), State(""), zip(""), status(false), My_DB(".dbinfo"){

}

Member::~Member(){

}

Member::Member(const std::string & passed_member_name,
    const std::string & passed_address,
    const std::string & passed_city,
    const std::string & passed_zip,
    const bool & passed_status): member_name(passed_member_name), member_id(""), address(passed_address), city(passed_city), zip(passed_zip), status(passed_status){

}


bool Member::add_member_DB(){

    return My_DB.update_member(*this);
}

bool Member::update_member_DB(){
    return false;
}

bool delete_member_DB(){
    return false;
}


std::string & Member::get_mem_name(){
    return member_name;
}

std::string & Member::set_mem_name(const std::string & to_set){
    member_name = to_set;
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







Provider::Provider(): name(""), provider_id(""), address(""), city(""), State(""), zip(""){

}

Provider::~Provider(){

}

Provider::Provider(const std::string & passed_name,
    const std::string & passed_address,
    const std::string & passed_city,
    const std::string & passed_zip): name(passed_name), provider_id(""), address(passed_address), city(passed_city), zip(passed_zip){

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

std::string & Provider::get_provider_name(){
    return name;
}

std::string & Provider::set_provider_name(const std::string & to_set){
    name = to_set;
    return name;
}

std::string & Provider::get_provider_id(){
    return provider_id;
}

std::string & Provider::set_provider_id(const std::string & to_set){
    provider_id = to_set;
    return provider_id;
}

std::string & Provider::get_provider_address(){
    return address;
}

std::string & Provider::set_provider_address(const std::string & to_set){
    address = to_set;
    return address;
}

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



bool Provider::GET_PROVIDER_FROM_DB(const std::string & PROVIDER_ID){

    return false;
}
