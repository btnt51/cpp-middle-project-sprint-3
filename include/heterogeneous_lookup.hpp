#pragma once

#include "book.hpp"
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentAuthorLess {
    using is_transparent = void;

    bool operator()(const std::string_view a, const Book &b) const noexcept { return a < b.author; }
    bool operator()(const Book &b, const std::string_view a) const noexcept { return b.author < a; }
    bool operator()(const Book &x, const Book &y) const noexcept { return x.author < y.author; }
};

struct TransparentAuthorEqual {
    using is_transparent = void;

    bool operator()(std::string_view a, const Book &b) const noexcept { return a == b.author; }
    bool operator()(const Book &b, std::string_view a) const noexcept { return b.author == a; }

    bool operator()(const std::string &a, const Book &b) const noexcept { return a == b.author; }
    bool operator()(const Book &b, const std::string &a) const noexcept { return b.author == a; }

    bool operator()(const Book &x, const Book &y) const noexcept { return x.author == y.author; }

    bool operator()(std::string_view a, std::string_view b) const noexcept { return a == b; }
    bool operator()(const std::string &a, const std::string &b) const noexcept { return a == b; }
    bool operator()(std::string_view a, const std::string &b) const noexcept { return a == b; }
    bool operator()(const std::string &a, std::string_view b) const noexcept { return a == b; }

    bool operator()(const char *a, const std::string &b) const noexcept { return std::string_view(a) == b; }
    bool operator()(const std::string &a, const char *b) const noexcept { return a == std::string_view(b); }

    bool operator()(const char *a, std::string_view b) const noexcept { return std::string_view(a) == b; }
    bool operator()(std::string_view a, const char *b) const noexcept { return a == std::string_view(b); }
};

struct TransparentAuthorHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view s) const noexcept { return std::hash<std::string_view>{}(s); }

    std::size_t operator()(const std::string &s) const noexcept { return std::hash<std::string_view>{}(s); }

    std::size_t operator()(const char *s) const noexcept { return std::hash<std::string_view>{}(s); }

    std::size_t operator()(const Book &b) const noexcept { return std::hash<std::string_view>{}(b.author); }
};

}  // namespace bookdb
