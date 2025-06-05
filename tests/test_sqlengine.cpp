// File to test sqlengine
#include "sqlengine.h"
#include <gtest/gtest.h>


TEST(SQLEngineInitTest, InitSuccess){
    EXPECT_NO_THROW(SQLEngine eng;);
}

TEST(SQLEngineValidateProvider, ValidationSuccess) {
    SQLEngine db;
    EXPECT_TRUE(db.validate_provider("200000000"));
}

TEST(SQLEngineValidateProvider, ValidationFail) {
    SQLEngine db;
    EXPECT_FALSE(db.validate_provider("20000"));
}

TEST(SQLEngineValidateService, ValidationSuccess){
    SQLEngine db;
    EXPECT_TRUE(db.validate_service("100003"));
}
