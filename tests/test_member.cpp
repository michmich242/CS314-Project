//member tests
#include "../include/member.h"
#include <gtest/gtest.h>

//default constructor
TEST(MemberConstructorTest, DefaultConstructorInitializesFields) {
    EXPECT_NO_THROW(Member m);    
} 
//constructor
TEST(MemberConstructorTest, ConstructorInitializesFields) {
    Member m("test name",
             "test address",
             "test city",
             "test state",
             "test zip",
              true,
              true);

   
     EXPECT_EQ(m.get_name(), "test name");   
     EXPECT_EQ(m.get_address(), "test address");
     EXPECT_EQ(m.get_city(), "test city");
     EXPECT_EQ(m.get_state(), "test state");
     EXPECT_EQ(m.get_zip(), "test zip");
     EXPECT_TRUE(m.get_status());
     EXPECT_TRUE(m.get_subscription());
   }

//getters/setters
TEST(MemberSettersAndGetters, SetValuesSuccess) {
    Member m;

    m.set_name("test name");
    m.set_address("test address");
    m.set_city("test city");
    m.set_state("test state");
    m.set_zip("test zip");
    m.set_status(true);
    m.set_subscription(true);

    EXPECT_EQ(m.get_name(), "test name");
    EXPECT_EQ(m.get_address(), "test address");
    EXPECT_EQ(m.get_city(), "test city");
    EXPECT_EQ(m.get_state(), "test state");
    EXPECT_EQ(m.get_zip(), "test zip");
    EXPECT_TRUE(m.get_status());
    EXPECT_TRUE(m.get_subscription());
}



