#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <string_view>

#include "book_database.hpp"

#include <flat_map>

namespace bookdb {
using VectorOfRefToConstBooks = std::vector<std::reference_wrapper<const Book>>;

template <BookContainerLike T>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont) {
    std::flat_map<std::string_view, size_t> histogram;
    std::for_each(cont.cbegin(), cont.cend(), [&](const auto &book) {
        auto author = book.author;
        ++histogram[author];
    });
    return histogram;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It begin, Sent end) {
    std::flat_map<std::string_view, std::pair<double, std::size_t>> statistic;
    std::for_each(begin, end, [&](const auto &book) {
        auto &[rating, amount] = statistic[StringFromGenre(book.genre)];
        ++amount;
        rating += book.rating;
    });

    std::flat_map<std::string_view, double> histogram;

    std::for_each(statistic.cbegin(), statistic.cend(), [&histogram](const auto &statisticPerBook) {
        const auto &[genre, bookStatistic] = statisticPerBook;
        auto genre_for_map = genre;
        histogram[genre_for_map] = bookStatistic.first / bookStatistic.second;
    });

    return histogram;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.empty())
        throw std::invalid_argument("empty book db");

    double average = std::accumulate(cont.cbegin(), cont.cend(), 0.0,
                                     [&](const double &rating, const auto &rhs) { return rating + rhs.rating; });
    return average / static_cast<double>(cont.size());
}

template <BookContainerLike T, BookComparator Comparator>
VectorOfRefToConstBooks getTopNBy(BookDatabase<T> &cont, std::size_t topN, Comparator comparator) {
    auto n = std::min(topN, cont.size());
    auto mid = cont.begin() + n;
    std::partial_sort(cont.begin(), mid, cont.end(), comparator);
    VectorOfRefToConstBooks res(cont.begin(), mid);
    return res;
}

template <BookContainerLike T>
VectorOfRefToConstBooks sampleRandomBooks(const BookDatabase<T> &cont, std::size_t amount) {
    VectorOfRefToConstBooks res;
    std::sample(cont.cbegin(), cont.cend(), std::back_inserter(res), amount, std::mt19937{std::random_device{}()});
    return res;
}

}  // namespace bookdb
