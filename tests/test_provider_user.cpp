#include "../include/provider_user.h"
#include "../include/sqlengine.h"
#include <gtest/gtest.h>


TEST(ProviderUserTest, LoginAcceptsValidInputImmediately) {
    SQLEngine eng;
    Provider_User provider(eng);

    std::string input = "123456789"; // any valid 9-digit string
    bool result = provider.login(input);

    EXPECT_TRUE(result);
    EXPECT_EQ(input, "123456789");
}
