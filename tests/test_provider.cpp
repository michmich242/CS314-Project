// File to test provider
#include "provider.h"
#include <gtest/gtest.h>

TEST(ProviderUserTest, LoginAcceptsValidInputImmediately) {
    Provider_User provider;

    std::string input = "123456789"; // any valid 9-digit string
    bool result = provider.login(input);

    EXPECT_TRUE(result);
    EXPECT_EQ(input, "123456789");
}