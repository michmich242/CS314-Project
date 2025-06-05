// File to test manager

#include "../include/manager.h"
#include <gtest/gtest.h>

TEST(MemberAddTest, AddMemberToDB) {

  SQLEngine eng;
  Manager m(eng);


  std::streambuf* original = std::cin.rdbuf();
  
  //add correct data for a member into the cin stream
  //name, address, city, state
  std::stringstream input("Stewie\n4258 Main St\nDetroit\nMI\n32574");
  std::cin.rdbuf(input.rdbuf());

  bool result = m.add_member();
  EXPECT_EQ(result, true);
}
