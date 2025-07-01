#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByTitle {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title < rhs.title; }
};

struct GreaterByTitle {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title > rhs.title; }
};

struct LessByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
};

struct GreaterByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author > rhs.author; }
};

struct LessByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct GreaterByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating > rhs.rating; }
};

struct LessByReadCount {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct GreaterByReadCount {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count > rhs.read_count; }
};

}  // namespace bookdb::comp