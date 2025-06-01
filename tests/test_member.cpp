//member tests
#include "../include/member.h"
#include <gtest/gtest.h>

//default constructor
TEST(MemberConstructorTest, DefaultConstructorInitializesFields) {
    Member m;

   
     EXPECT_EQ(m.get_name(), "");  
     EXPECT_EQ(m.get_address(), "");
     EXPECT_EQ(m.get_city(), "");
     EXPECT_EQ(m.get_zip(), "");
     EXPECT_FALSE(m.get_status());
   }

//constructor
TEST(MemberConstructorTest, ConstructorInitializesFields) {
    Member m("test name",
             "test address",
             "test city",
             "test zip",
              true);

   
     EXPECT_EQ(m.get_name(), "test name");   
     EXPECT_EQ(m.get_address(), "test address");
     EXPECT_EQ(m.get_city(), "test city");
     EXPECT_EQ(m.get_zip(), "test zip");
     EXPECT_TRUE(m.get_status());
   }

//getters/setters
TEST(MemberSettersAndGetters, SetValuesSuccess) {
    Member m;

    m.set_name("test name");
    m.set_ID("test id");
    m.set_address("test address");
    m.set_city("test city");
    m.set_state("test state");
    m.set_zip("test zip");

    EXPECT_EQ(m.get_name(), "test name");
    EXPECT_EQ(m.get_ID(), "test id");
    EXPECT_EQ(m.get_address(), "test address");
    EXPECT_EQ(m.get_city(), "test city");
    EXPECT_EQ(m.get_state(), "test state");
    EXPECT_EQ(m.get_zip(), "test zip");
}



