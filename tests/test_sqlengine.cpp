// File to test sqlengine
#include "sqlengine.h"
#include <gtest/gtest.h>


TEST(SQLEngineInitTest, InitSuccess){
    EXPECT_NO_THROW(SQLEngine eng;);
}