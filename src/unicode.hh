#ifndef unicode_hh_INCLUDED
#define unicode_hh_INCLUDED

#include <cwctype>
#include <cwchar>
#include <locale>

#include "units.hh"

namespace Kakoune
{

using Codepoint = char32_t;

inline bool is_eol(Codepoint c) noexcept
{
    return c == '\n';
}

inline bool is_horizontal_blank(Codepoint c) noexcept
{
    return c == ' ' or c == '\t';
}

inline bool is_blank(Codepoint c) noexcept
{
    return c == ' ' or c == '\t' or c == '\n';
}

enum WordType { Word, WORD };

template<WordType word_type = Word>
inline bool is_word(Codepoint c) noexcept
{
    return c == '_' or iswalnum((wchar_t)c);
}

template<>
inline bool is_word<WORD>(Codepoint c) noexcept
{
    return not is_blank(c);
}

inline bool is_punctuation(Codepoint c) noexcept
{
    return not (is_word(c) or is_blank(c));
}

inline bool is_basic_alpha(Codepoint c) noexcept
{
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z');
}

inline ColumnCount codepoint_width(Codepoint c) noexcept
{
    return c == '\n' ? 1 : wcwidth((wchar_t)c);
}

enum class CharCategories
{
    Blank,
    EndOfLine,
    Word,
    Punctuation,
};

template<WordType word_type = Word>
inline CharCategories categorize(Codepoint c) noexcept
{
    if (is_eol(c))
        return CharCategories::EndOfLine;
    if (is_horizontal_blank(c))
        return CharCategories::Blank;
    if (word_type == WORD or is_word(c))
        return CharCategories::Word;
    return CharCategories::Punctuation;
}

inline Codepoint to_lower(Codepoint cp) noexcept { return towlower((wchar_t)cp); }
inline Codepoint to_upper(Codepoint cp) noexcept { return towupper((wchar_t)cp); }

inline char to_lower(char c) noexcept { return c >= 'A' and c <= 'Z' ? c - 'A' + 'a' : c; }
inline char to_upper(char c) noexcept { return c >= 'a' and c <= 'z' ? c - 'a' + 'A' : c; }

}

#endif // unicode_hh_INCLUDED
