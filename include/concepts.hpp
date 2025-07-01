#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename Container>
concept BookContainerLike = requires(Container cont, typename Container::value_type v) {
    typename Container::value_type;
    { cont.begin() } -> std::input_or_output_iterator;
    { cont.end() } -> std::sentinel_for<decltype(cont.begin())>;
    { cont.push_back(v) };
    { cont.emplace_back(v) };
    { cont.size() };
    { cont.empty() };
} && std::is_same_v<Book, typename Container::value_type>;

template <typename T>
concept BookIterator = std::contiguous_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &>;

template <typename C>
concept BookComparator = requires(C comparator, const Book &lhs, const Book &rhs) {
    { comparator(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb