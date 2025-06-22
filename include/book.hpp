#pragma once

#include <format>
#include <map>
#include <string>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    static std::map<std::string_view, Genre> map = {{"Fiction", Genre::Fiction}, {"NonFiction", Genre::NonFiction},
                                                    {"SciFi", Genre::SciFi},     {"Biography", Genre::Biography},
                                                    {"Mystery", Genre::Mystery}, {"Unknown", Genre::Unknown}};
    return map.contains(s) ? map.at(s) : Genre::Unknown;
}

constexpr std::string_view StringFromGenre(Genre genre) {
    static std::map<Genre, std::string_view> map = {{Genre::Fiction, "Fiction"}, {Genre::NonFiction, "NonFiction"},
                                                    {Genre::SciFi, "SciFi"},     {Genre::Biography, "Biography"},
                                                    {Genre::Mystery, "Mystery"}, {Genre::Unknown, "Unknown"}};
    return map.contains(genre) ? map.at(genre) : "Unknown";
}

struct Book {
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;
    constexpr Book(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                   int read_count)
        : author{author}, title{title}, year{year}, genre(genre), rating{rating}, read_count{read_count} {}

    constexpr Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
                   int read_count)
        : author{author}, title{title}, year{year}, genre{GenreFromString(genre)}, rating{rating},
          read_count{read_count} {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", StringFromGenre(g));
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        return format_to(fc.out(), "Author: {} / Title: {} / Year: {} / Genre: {} / Rating: {} / Read count: {}",
                         b.author, b.title, b.year, bookdb::StringFromGenre(b.genre), b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std
