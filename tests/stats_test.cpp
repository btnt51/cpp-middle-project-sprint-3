#include "book_database.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

class BookDatabaseStatsTest : public ::testing::Test {
protected:
    BookDatabase<std::vector<Book>> db;

    void SetUp() override {
        db.EmplaceBack("Dune", "Herbert", 1965, Genre::SciFi, 4.8, 10);
        db.EmplaceBack("1984", "Orwell", 1949, Genre::Fiction, 4.6, 12);
        db.EmplaceBack("Shining", "King", 1977, Genre::Mystery, 3.9, 8);
        db.EmplaceBack("Misery", "King", 1987, Genre::Mystery, 4.2, 7);
        db.EmplaceBack("Brief History", "Hawking", 1988, Genre::NonFiction, 4.2, 11);
    }
};

// buildAuthorHistogramFlat
TEST_F(BookDatabaseStatsTest, AuthorHistogram_CorrectCounts) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram["King"], 2);
    EXPECT_EQ(histogram["Herbert"], 1);
}

TEST_F(BookDatabaseStatsTest, AuthorHistogram_EmptyDB) {
    BookDatabase<> empty;
    auto histogram = buildAuthorHistogramFlat(empty);
    EXPECT_TRUE(histogram.empty());
}

TEST_F(BookDatabaseStatsTest, AuthorHistogram_CaseSensitivity) {
    db.EmplaceBack("The Stand", "king", 1978, Genre::Mystery, 4.0, 5);
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram["king"], 1);
    EXPECT_EQ(histogram["King"], 2);
}

// calculateGenreRatings
TEST_F(BookDatabaseStatsTest, GenreRatings_CorrectAverage) {
    auto histogram = calculateGenreRatings(db.begin(), db.end());
    EXPECT_NEAR(histogram["Mystery"], (3.9 + 4.2) / 2.0, 1e-6);
}

TEST_F(BookDatabaseStatsTest, GenreRatings_EmptyRange) {
    auto histogram = calculateGenreRatings(db.end(), db.end());
    EXPECT_TRUE(histogram.empty());
}

TEST_F(BookDatabaseStatsTest, GenreRatings_UnknownGenreHandled) {
    db.EmplaceBack("Mystery Unknown", "Ghost", 2000, Genre::Unknown, 3.0, 1);
    auto histogram = calculateGenreRatings(db.begin(), db.end());
    EXPECT_NEAR(histogram["Unknown"], 3.0, 1e-6);
}

// calculateAverageRating
TEST_F(BookDatabaseStatsTest, AverageRating_CorrectValue) {
    double expected = (4.8 + 4.6 + 3.9 + 4.2 + 4.2) / 5.0;
    EXPECT_NEAR(calculateAverageRating(db), expected, 1e-6);
}

TEST_F(BookDatabaseStatsTest, AverageRating_EmptyDatabaseThrows) {
    BookDatabase<> empty;
    EXPECT_THROW(calculateAverageRating(empty), std::invalid_argument);
}

TEST_F(BookDatabaseStatsTest, AverageRating_SingleElement) {
    BookDatabase<> single;
    single.EmplaceBack("Test", "A", 2020, Genre::Fiction, 5.0, 1);
    EXPECT_DOUBLE_EQ(calculateAverageRating(single), 5.0);
}

// getTopNBy
struct RatingDescending {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating > rhs.rating; }
};

TEST_F(BookDatabaseStatsTest, TopNByRating_Valid) {
    auto top = getTopNBy(db, 2, RatingDescending{});
    ASSERT_EQ(top.size(), 2);
    EXPECT_EQ(top[0].get().title, "Dune");
    EXPECT_EQ(top[1].get().title, "1984");
}

TEST_F(BookDatabaseStatsTest, TopNByRating_LimitGreaterThanSize) {
    auto top = getTopNBy(db, 10, RatingDescending{});
    EXPECT_EQ(top.size(), db.size());
}

TEST_F(BookDatabaseStatsTest, TopNByRating_EmptyDB) {
    BookDatabase<> empty;
    auto top = getTopNBy(empty, 3, RatingDescending{});
    EXPECT_TRUE(top.empty());
}

// sampleRandomBooks
TEST_F(BookDatabaseStatsTest, SampleRandomBooks_CorrectSize) {
    auto sample = sampleRandomBooks(db, 3);
    EXPECT_EQ(sample.size(), 3);
}

TEST_F(BookDatabaseStatsTest, SampleRandomBooks_RequestMoreThanAvailable) {
    auto sample = sampleRandomBooks(db, 100);
    EXPECT_EQ(sample.size(), db.size());
}

TEST_F(BookDatabaseStatsTest, SampleRandomBooks_EmptyDB) {
    BookDatabase<> empty;
    auto sample = sampleRandomBooks(empty, 3);
    EXPECT_TRUE(sample.empty());
}