//member tests
#include "manager.h"
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

   
     EXPECT_EQ(m.get_name(), "test name");   // Or whatever the default is
     EXPECT_EQ(m.get_address(), "test address");
     EXPECT_EQ(m.get_city(), "test city");
     EXPECT_EQ(m.get_zip(), "test zip");
     EXPECT_TRUE(m.get_status());
   }