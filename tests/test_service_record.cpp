//service record tests
#include "manager.h"
#include <gtest/gtest.h>

TEST(ServiceRecordInitTest, InitSuccess) {
    EXPECT_NO_THROW(ServiceRecord s_record);
}