#pragma once

#include <print>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = BookContainer::value_type;
    using pointer = BookContainer::pointer;
    using const_pointer = BookContainer::const_pointer;
    using reference = BookContainer::reference;
    using const_reference = BookContainer::const_reference;
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using const_reverse_iterator = BookContainer::const_reverse_iterator;
    using reverse_iterator = BookContainer::reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using allocator_type = BookContainer::allocator_type;
    // Ваш код здесь

    using AuthorContainer = std::unordered_set<std::string>;

    constexpr BookDatabase() = default;

    constexpr BookDatabase(std::initializer_list<Book> lst) : books_(lst) {
        std::for_each(books_.begin(), books_.end(), [&](auto &book) { add_author(std::string{book.author}); });
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    constexpr void push_back(const Book &book) { books_.push_back(book); }

    constexpr void push_back(Book &&book) {
        books_.push_back(std::forward<Book>(book));
        add_author(std::string{books_.back().author});
    }

    template <typename... Args>
    constexpr reference EmplaceBack(Args &&...args) {
        auto &book = books_.emplace_back(std::forward<Args>(args)...);
        add_author(std::string{book.author});
        return book;
    }

    constexpr size_type size() { return books_.size(); }

    constexpr size_type size() const { return books_.size(); }

    constexpr reference back() { return books_.back(); }

    constexpr const_reference back() const { return books_.back(); }

    constexpr iterator begin() noexcept { return books_.begin(); }

    constexpr const_iterator begin() const noexcept { return books_.begin(); }

    constexpr const_iterator cbegin() const noexcept { return books_.cbegin(); }

    constexpr reverse_iterator rbegin() noexcept { return books_.rbegin(); }

    constexpr const_reverse_iterator rbegin() const noexcept { return books_.rbegin(); }

    constexpr const_reverse_iterator rcbegin() const noexcept { return books_.crbegin(); }

    constexpr iterator end() noexcept { return books_.end(); }

    constexpr const_iterator end() const noexcept { return books_.end(); }

    constexpr const_iterator cend() const noexcept { return books_.cend(); }

    constexpr reverse_iterator rend() noexcept { return books_.rend(); }

    constexpr const_reverse_iterator rend() const noexcept { return books_.rend(); }

    constexpr const_reverse_iterator rcend() const noexcept { return books_.crend(); }

    constexpr bool empty() const noexcept { return books_.empty(); }

    auto GetBooks() { return std::span<Book>(books_.begin(), books_.end()); }

    const auto GetBooks() const { return std::span<const Book>(books_.begin(), books_.end()); }

    auto GetAuthors() { return authors_; }

    const auto GetAuthors() const { return authors_; }

private:
    constexpr void add_author(const std::string &author) {
        if (not author.empty())
            authors_.emplace(author);
    }
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
