#include "book_database.hpp"
#include <gtest/gtest.h>
#include <string>
#include <string_view>

using namespace bookdb;

class HeterogeneousLookupTest : public ::testing::Test {
protected:
    BookDatabase<> db;

    void SetUp() override {
        db.EmplaceBack("1984", "Orwell", 1949, Genre::SciFi, 4.8, 200);
        db.EmplaceBack("Animal Farm", "Orwell", 1945, Genre::Fiction, 4.7, 150);
        db.EmplaceBack("Solaris", "Lem",       1961, Genre::SciFi, 4.5,  15);
    }
};

TEST_F(HeterogeneousLookupTest, FindByCharPointer) {
    auto &authors = db.GetAuthors();
    // literal
    EXPECT_TRUE(authors.contains("Orwell"));
    EXPECT_TRUE(authors.contains("Lem"));
    EXPECT_FALSE(authors.contains("UnknownAuthor"));
}

TEST_F(HeterogeneousLookupTest, FindByStdString) {
    auto &authors = db.GetAuthors();
    std::string key1 = "Orwell";
    std::string key2 = "Lem";
    std::string key3 = "Hemingway";

    EXPECT_TRUE(authors.contains(key1));
    EXPECT_TRUE(authors.contains(key2));
    EXPECT_FALSE(authors.contains(key3));
}

TEST_F(HeterogeneousLookupTest, FindByStringView) {
    auto &authors = db.GetAuthors();
    std::string_view key1 = "Orwell";
    std::string_view key2 = "Lem";
    std::string_view key3 = "Twain";

    EXPECT_TRUE(authors.contains(key1));
    EXPECT_TRUE(authors.contains(key2));
    EXPECT_FALSE(authors.contains(key3));
}

TEST_F(HeterogeneousLookupTest, FindByBookKey) {
    auto &authors = db.GetAuthors();
    const Book &b = db.back();
    EXPECT_TRUE(authors.contains(b));

    Book fake{"Invisible Man", "Ellison", 1952, Genre::Fiction, 4.6, 20};
    EXPECT_FALSE(authors.contains(fake));
}

TEST_F(HeterogeneousLookupTest, FindIteratorReturnsValidIterator) {
    auto &authors = db.GetAuthors();


    auto it1 = authors.find("Orwell");
    ASSERT_NE(it1, authors.end());
    EXPECT_EQ(*it1, "Orwell");

    std::string key = "Lem";
    auto it2 = authors.find(key);
    ASSERT_NE(it2, authors.end());
    EXPECT_EQ(*it2, "Lem");

    Book tmp{"Dummy", "Orwell", 0, Genre::Unknown, 0.0, 0};
    auto it3 = authors.find(tmp);
    ASSERT_NE(it3, authors.end());
    EXPECT_EQ(*it3, "Orwell");
}