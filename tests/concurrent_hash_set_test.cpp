#include "gtest/gtest.h"
#include "concurrent_hash_set.h"

TEST(concurrent_hash_set_test, test_something)
{
    concurrent_hash_set<int> hash_set;

    hash_set.insert(6);
    EXPECT_TRUE(hash_set.contains(6));
    
    hash_set.erase(6);
    EXPECT_FALSE(hash_set.contains(6));
}
