#include "book.hpp"
#include "filters.hpp"
#include <gtest/gtest.h>
using namespace bookdb;

class BookFilterTest : public ::testing::Test {
protected:
    std::vector<Book> books = {{"Dune", "Frank Herbert", 1965, "SciFi", 4.8, 10},
                               {"1984", "George Orwell", 1949, "Fiction", 4.6, 12},
                               {"Brief History", "Stephen Hawking", 1988, "NonFiction", 4.2, 8},
                               {"The Shining", "Stephen King", 1977, "Mystery", 3.9, 7},
                               {"Becoming", "Michelle Obama", 2018, "Biography", 4.4, 11},
                               {"Unknown Book", "John Doe", 2022, "Unknown", 2.5, 2}};

    VectorOfRefToConstBooks run_filter(auto predicate) { return filterBooks(books.begin(), books.end(), predicate); }
};

// Теперь сами тесты:

TEST_F(BookFilterTest, GenreIs_SciFi) {
    auto filtered = run_filter(GenreIs("SciFi"));
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Dune");
}

TEST_F(BookFilterTest, GenreAnyOf_FictionAndBiography) {
    auto filtered = run_filter(GenreAnyOf("Fiction", "Biography"));
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "1984");
    EXPECT_EQ(filtered[1].get().title, "Becoming");
}

TEST_F(BookFilterTest, AboveRating) {
    auto filtered = run_filter(AboveRating(4.5));
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "Dune");
    EXPECT_EQ(filtered[1].get().title, "1984");
}

TEST_F(BookFilterTest, BelowRating) {
    auto filtered = run_filter(BelowRating(3.0));
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Unknown Book");
}

TEST_F(BookFilterTest, RatingBetween_ValidRange) {
    auto filtered = run_filter(RatingBetween(3.9, 4.4));
    ASSERT_EQ(filtered.size(), 3);
    EXPECT_EQ(filtered[0].get().title, "Brief History");
    EXPECT_EQ(filtered[1].get().title, "The Shining");
    EXPECT_EQ(filtered[2].get().title, "Becoming");
}

TEST_F(BookFilterTest, RatingBetween_InvalidRangeThrows) {
    EXPECT_THROW(RatingBetween(4.5, 2.0), std::invalid_argument);
}

TEST_F(BookFilterTest, YearBetween_Valid) {
    auto filtered = run_filter(YearBetween(1960, 2000));
    ASSERT_EQ(filtered.size(), 3);
    EXPECT_EQ(filtered[0].get().title, "Dune");
    EXPECT_EQ(filtered[1].get().title, "Brief History");
    EXPECT_EQ(filtered[2].get().title, "The Shining");
}

TEST_F(BookFilterTest, AllOf_GenreAndRating) {
    auto pred = all_of(GenreIs("SciFi"), AboveRating(4.0));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Dune");
}

TEST_F(BookFilterTest, AnyOf_LowRatingOrUnknownGenre) {
    auto pred = any_of(BelowRating(3.0), GenreIs("Unknown"));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Unknown Book");
}

TEST_F(BookFilterTest, AllOf_GenreAndYear) {
    auto pred = all_of(GenreIs("Biography"), AboveOrEqualYear(2015));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Becoming");
}

TEST_F(BookFilterTest, AllOf_MultipleConditions) {
    auto pred = all_of(GenreAnyOf("Fiction", "SciFi"), AboveOrEqualYear(1950), AboveRating(4.5));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Dune");
}

TEST_F(BookFilterTest, AnyOf_MultipleGenresAndLowRating) {
    auto pred = any_of(GenreAnyOf("Mystery", "Biography"), BelowRating(3.0));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 3);
    EXPECT_EQ(filtered[0].get().title, "The Shining");
    EXPECT_EQ(filtered[1].get().title, "Becoming");
    EXPECT_EQ(filtered[2].get().title, "Unknown Book");
}

TEST_F(BookFilterTest, NestedAllOfInsideAnyOf) {
    auto pred = any_of(all_of(GenreIs("Fiction"), AboveRating(4.5)), all_of(GenreIs("Mystery"), BelowRating(4.0)));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "1984");
    EXPECT_EQ(filtered[1].get().title, "The Shining");
}

TEST_F(BookFilterTest, ComplexCombination_AllOfAnyOf) {
    auto pred = all_of(any_of(GenreIs("SciFi"), GenreIs("Biography")), AboveOrEqualRating(4.4));
    auto filtered = run_filter(pred);
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "Dune");
    EXPECT_EQ(filtered[1].get().title, "Becoming");
}