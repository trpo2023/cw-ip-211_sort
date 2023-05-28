#include "gtest/gtest.h"
#include <string>
#include <vector>
#include "../src/libsort/sort.h"

TEST(TestGroupFlags, NoFlags) {
    std::vector<std::string> expectedList = { "Apple", "Robot", "Zebra" };
    std::vector<std::string> list = { "Robot", "Zebra", "Apple" };
    sortVector(list);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, RandomFlag) {
    std::vector<std::string> expectedList = { "Apple", "Robot", "Zebra" };
    std::vector<std::string> list = { "Zebra", "Robot", "Apple" };
    randomSort(list);
    ASSERT_NE(expectedList, list);
}

TEST(TestGroupFlags, ReverseFlag) {
    std::vector<std::string> expectedList = { "Apple", "Robot", "Zebra" };
    std::vector<std::string> list = {"Zebra", "Robot", "Apple" };
    reverseSort(list);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, NumberFlag) {
    std::vector<std::string> expectedList = { "96", "56748", "1231546" };
    std::vector<std::string> list = { "1231546", "96", "56748" };
    sortVector(list, numericOrderCompare);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, CheckFlag) {
    std::vector<std::string> list = { "96", "56748", "1231546" };
    ASSERT_EQ(checkSorted(list), false);
}

TEST(TestGroupFlags, VersionFlag) {
    std::vector<std::string> expectedList = { "1.0.3", "1.5.2", "2.1.0", "3.0.1" };
    std::vector<std::string> list = { "1.0.3", "2.1.0", "1.5.2", "3.0.1" };
    sortVector(list, versionCompare);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, DictionaryFlags) {
    std::vector<std::string> expectedList = { "Apple", "Robot", "Zebra" };
    std::vector<std::string> list = { "Robot", "Zebra", "Apple" };
    sortVector(list, dictionaryOrderCompare);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, IgnoreCaseFlags) {
    std::vector<std::string> expectedList = { "apple", "Apple", "Banana", "banana", "Cherry" , "ORANGE" };
    std::vector<std::string> list = { "apple", "Banana", "Cherry", "banana", "ORANGE" , "Apple" };
    sortVector(list, caseInsensitiveCompare);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, GeneralNumericFlags) {
    std::vector<std::string> expectedList = { "Apple", "Banana", "Cherry", "ORANGE", "apple", "banana" };
    std::vector<std::string> list = { "apple", "Banana", "Cherry", "banana", "ORANGE" , "Apple" };
    sortVector(list, ignorePunctuationAndSpaces);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, HumanNumericFlags) {
    std::vector<std::string> expectedList = {"100", "1K", "500M", "2G"};
    std::vector<std::string> list = {"500M", "2G", "100", "1K"};
    sortVector(list, humanNumericCompare);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, IgnoreNonPrintableFlags) {
    std::vector<std::string> expectedList = { "Apple", "\nRobot", "\tZebra" };
    std::vector<std::string> list = { "\nRobot", "\tZebra", "Apple" };
    sortVector(list, isPrintable);
    ASSERT_EQ(expectedList, list);
}

TEST(TestGroupFlags, ColumnFlags) {
    std::vector<std::string> expectedList = { 
        "1 2 3", 
        "alpha beta gamma", 
        "cat dog bird", 
        "red green blue", 
        "apple orange banana" 
    };
    std::vector<std::string> list = {
        "apple orange banana",
        "cat dog bird",
        "red green blue",
        "1 2 3",
        "alpha beta gamma"
    };
    std::vector<std::string> flags = { "-k2" };
    kSortVector(list, flags);
    ASSERT_EQ(expectedList, list);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
