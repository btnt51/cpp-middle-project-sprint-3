#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
using VectorOfRefToConstBooks = std::vector<std::reference_wrapper<const Book>>;

template <BookIterator It, BookSentinel<It> Sentinel, BookPredicate Predicate>
VectorOfRefToConstBooks filterBooks(It first, Sentinel last, Predicate predicate) {
    VectorOfRefToConstBooks res;
    std::copy_if(first, last, std::back_inserter(res), predicate);
    return res;
}

template <BookPredicate... Predicate>
auto all_of(Predicate &&...predicates) {
    return [preds_tuple = std::make_tuple(std::forward<Predicate>(predicates)...)](const Book &book) -> bool {
        return std::apply([&](auto &&...predicate) { return (predicate(book) && ...); }, preds_tuple);
    };
}

template <BookPredicate... Predicate>
auto any_of(Predicate &&...predicates) {
    return [preds_tuple = std::make_tuple(std::forward<Predicate>(predicates)...)](const Book &book) -> bool {
        return std::apply([&](auto &&...predicate) { return (predicate(book) || ...); }, preds_tuple);
    };
}

inline auto GenreIs(const std::string_view genre) {
    return [genre](const Book &book) { return book.genre == GenreFromString(genre); };
}

inline auto GenreIs(const bookdb::Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <typename... GenresAsSV>
inline auto GenreAnyOf(GenresAsSV &&...genres) {
    return any_of(GenreIs(std::forward<GenresAsSV>(genres))...);
}

inline auto AboveRating(const double rat) {
    return [rat](const Book &book) { return book.rating > rat; };
}

inline auto BelowRating(const double rat) {
    return [rat](const Book &book) { return book.rating < rat; };
}

inline auto AboveOrEqualRating(const double rat) {
    return [rat](const Book &book) { return book.rating >= rat; };
}

inline auto BelowOrEqualRating(const double rat) {
    return [rat](const Book &book) { return book.rating <= rat; };
}

inline auto RatingBetween(const double rating1, const double rating2) {
    if (rating1 > rating2)
        throw std::invalid_argument("First rating must be smaller then second rating");
    return [rating1, rating2](const Book &book) {
        return AboveOrEqualRating(rating1)(book) && BelowOrEqualRating(rating2)(book);
    };
}

inline auto AboveYear(const int year) {
    return [year](const Book &book) { return book.year > year; };
}

inline auto BelowYear(const int year) {
    return [year](const Book &book) { return book.year < year; };
}

inline auto AboveOrEqualYear(const int year) {
    return [year](const Book &book) { return book.year >= year; };
}

inline auto BelowOrEqualYear(const int year) {
    return [year](const Book &book) { return book.year <= year; };
}

inline auto YearBetween(const int year1, const int year2) {
    if (year1 > year2)
        throw std::invalid_argument("First year must be smaller then second year");
    return [year1, year2](const Book &book) { return AboveOrEqualYear(year1)(book) && BelowOrEqualYear(year2)(book); };
}

}  // namespace bookdb