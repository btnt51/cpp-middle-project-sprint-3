#pragma once

#include "book.hpp"
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentTitleLess {
    using is_transparent = void;
    bool operator()(const std::string_view title, const Book &book) { return title < book.title; }
    bool operator()(const Book &book, const std::string_view title) { return book.title < title; }
};

struct TransparentTitleEqual {
    using is_transparent = void;  // поддержка гетерогенного поиска

    bool operator()(const std::string_view title, const Book &book) const { return title == book.title; }

    bool operator()(const Book &book, std::string title) const { return title == book.title; }
};

struct TransparentTitleHash {
    using is_transparent = void;  // поддержка гетерогенного поиска

    std::size_t operator()(const Book &book) const { return std::hash<std::string_view>{}(book.title); }

    std::size_t operator()(const std::string &title) const { return std::hash<std::string_view>{}(title); }
};

}  // namespace bookdb
