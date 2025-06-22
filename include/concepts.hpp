#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename Container>
concept BookContainerLike =
    requires { typename Container::value_type; } && std::is_same_v<Book, typename Container::value_type>;

template <typename T>
concept BookIterator = std::contiguous_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P predicate, const Book &arg) {
    { predicate(arg) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comparator, const Book &lhs, const Book &rhs) {
    { comparator(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb