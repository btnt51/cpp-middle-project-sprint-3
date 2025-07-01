#include "book_database.hpp"
#include <format>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <streambuf>

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    BookDatabase<> db;

    void SetUp() override {
        db.EmplaceBack("Solaris", "Lem", 1961, Genre::SciFi, 4.5, 15);
        db.EmplaceBack("To Kill a Mockingbird", "Lee", 1960, Genre::Fiction, 4.9, 8);
    }
};

TEST_F(BookDatabaseTest, EmplaceBack_AddsCorrectBooks) {
    EXPECT_EQ(db.size(), 2);
    auto books = db.GetBooks();
    EXPECT_EQ(books[0].author, "Lem");
    EXPECT_EQ(books[1].author, "Lee");
}

TEST_F(BookDatabaseTest, InitializerConstructor_AddsAuthors) {
    BookDatabase<> db_local = {Book{"Neuromancer", "Gibson", 1984, Genre::SciFi, 4.2, 13},
                               Book{"Pride and Prejudice", "Austen", 1813, Genre::Fiction, 4.7, 20}};
    EXPECT_EQ(db_local.size(), 2);
    EXPECT_TRUE(db_local.GetAuthors().contains("Gibson"));
    EXPECT_TRUE(db_local.GetAuthors().contains("Austen"));
}

TEST_F(BookDatabaseTest, PushBack_ModifiesBackCorrectly) {
    db.push_back(Book{"The Trial", "Kafka", 1925, Genre::Mystery, 4.1, 6});
    EXPECT_EQ(db.back().title, "The Trial");
    EXPECT_TRUE(db.GetAuthors().contains("Kafka"));
}

TEST_F(BookDatabaseTest, Clear_ResetsAllState) {
    db.Clear();
    EXPECT_TRUE(db.empty());
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST_F(BookDatabaseTest, Iterators_ForwardTraversal) {
    std::vector<std::string_view> titles;
    for (const auto &b : db)
        titles.push_back(b.title);
    ASSERT_EQ(titles.size(), 2);
    EXPECT_EQ(titles[0], "Solaris");
    EXPECT_EQ(titles[1], "To Kill a Mockingbird");
}

TEST_F(BookDatabaseTest, ConstIterators_ConsistentAccess) {
    const auto &cdb = db;
    auto it = cdb.cbegin();
    EXPECT_EQ(it->title, "Solaris");
    ++it;
    EXPECT_EQ(it->title, "To Kill a Mockingbird");
}

TEST_F(BookDatabaseTest, Spans_HaveCorrectContent) {
    const auto span = db.GetBooks();
    ASSERT_EQ(span.size(), 2);
    EXPECT_EQ(span[0].genre, Genre::SciFi);
    EXPECT_EQ(span[1].genre, Genre::Fiction);
}

TEST_F(BookDatabaseTest, Back_ReturnsLatestBook) { EXPECT_EQ(db.back().title, "To Kill a Mockingbird"); }

TEST(BookDatabaseStandalone, EmptyStateBehavesCorrectly) {
    BookDatabase<> empty;
    EXPECT_TRUE(empty.empty());
    empty.EmplaceBack("Test", "T", 2000, Genre::Unknown, 3.0, 1);
    EXPECT_FALSE(empty.empty());
}

TEST(BookDatabaseStandalone, AuthorsUpdateCorrectly) {
    BookDatabase<> db;
    db.EmplaceBack("Book", "Author", 2024, Genre::NonFiction, 3.5, 1);
    EXPECT_TRUE(db.GetAuthors().contains("Author"));
}

TEST(BookDatabaseStandalone, Formatter_OutputsCorrectText) {
    BookDatabase<> db;
    db.EmplaceBack("X", "Y", 2023, Genre::Fiction, 5.0, 3);

    std::ostringstream expected;
    expected << "BookDatabase (size = 1): ";
    expected << "Books:\n";
    expected << "- Author: Y / Title: X / Year: 2023 / Genre: Fiction / Rating: 5 / Read count: 3\n";
    expected << "Authors:\n";
    expected << "- Y\n";

    std::ostringstream actual;
    actual << std::format("{}", db);

    EXPECT_EQ(actual.str(), expected.str());
}
