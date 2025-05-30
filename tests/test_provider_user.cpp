#include "../include/provider_user.h"
#include "../include/sqlengine.h"
#include <gtest/gtest.h>


TEST(ProviderUserTest, LoginAcceptsValidInputImmediately) {
    SQLEngine eng;
    Provider_User provider(eng);

    std::string input = "123456789";
    bool result = provider.login(input);

    EXPECT_TRUE(result);
}

TEST(ProviderUserTest, MemberValidationAcceptsInput) {
    SQLEngine eng;
    Provider_User provider(eng);

    std::string input = "123456789";
    bool result = provider.member_validation(input);
    EXPECT_FALSE(result);
}
