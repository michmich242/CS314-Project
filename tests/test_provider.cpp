// File to test provider
#include "../include/provider.h"
#include <gtest/gtest.h>

//default constructor
TEST(ProviderDefaultConstructor, DefaultConstructorSuccess) {
    Provider p;

    EXPECT_EQ(p.get_name(), "");
    EXPECT_EQ(p.get_ID(), "");
    EXPECT_EQ(p.get_address(), "");
    EXPECT_EQ(p.get_city(), "");
    EXPECT_EQ(p.get_state(), "");
    EXPECT_EQ(p.get_zip(), "");
}
//constructor
TEST(ProviderConstructor, ConstructorSuccess) {
    Provider p("name", "address", "city", "ST", "zip");
  

    EXPECT_EQ(p.get_name(), "name");
    EXPECT_EQ(p.get_ID(), "");
    EXPECT_EQ(p.get_address(), "address");
    EXPECT_EQ(p.get_city(), "city");
    EXPECT_EQ(p.get_state(), "ST");
    EXPECT_EQ(p.get_zip(), "zip");
}
//getters/setters
TEST(ProviderSetName, SetNameSuccess) {
    Provider p;

    p.set_name("test name");
    p.set_ID("test id");
    p.set_address("test address");
    p.set_city("test city");
    p.set_state("test state");
    p.set_zip("test zip");

    EXPECT_EQ(p.get_name(), "test name");
    EXPECT_EQ(p.get_ID(), "test id");
    EXPECT_EQ(p.get_address(), "test address");
    EXPECT_EQ(p.get_city(), "test city");
    EXPECT_EQ(p.get_state(), "test state");
    EXPECT_EQ(p.get_zip(), "test zip");
}
