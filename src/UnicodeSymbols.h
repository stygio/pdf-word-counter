#ifndef _UNICODE_SYMBOLS_H
#define _UNICODE_SYMBOLS_H

#include <map>

const std::map <const std::string, const std::string> symbolMap = {
    // Ligatures
    {"c4b3", "ij"},         // ij  - Latin Small Ligature IJ
    {"c4b2", "IJ"},         // ij  - Latin Capital Ligature IJ
    {"c593", "oe"},         // oe  - Latin Small Ligature OE
    {"c592", "OE"},         // oe  - Latin Capital Ligature OE
    {"efac80", "ff"},       // ff  - Latin Small Ligature FF
    {"efac81", "fi"},       // fi  - Latin Small Ligature FI
    {"efac82", "fl"},       // fl  - Latin Small Ligature FL
    {"efac83", "ffi"},      // ffi - Latin Small Ligature FFI
    {"efac84", "ffl"},      // ffl - Latin Small Ligature FI
    {"efac85", "ft"},       // ft  - Latin Small Ligature FT
    {"efac86", "st"},       // st  - Latin Small Ligature ST

    // Text symbols, punctuation
    {"e28099", "\u0027"},   // ' - RIGHT SINGLE QUOTATION MARK (mapped to apostrophe)
    {"e280b2", "\u0027"},   // ' - PRIME (mapped to apostrophe)

    // Spaces
    {"c2a0", " "},      // No-Break Space (NBSP)
    {"e28080", " "},    // En Quad
    {"e28081", " "},    // Em Quad
    {"e28082", " "},    // En Space
    {"e28083", " "},    // Em Space
    {"e28084", " "},    // Three-Per-Em Space
    {"e28085", " "},    // Four-Per-Em Space
    {"e28086", " "},    // Six-Per-Em Space
    {"e28087", " "},    // Figure Space
    {"e28088", " "},    // Punctuation Space
    {"e28089", " "},    // Thin Space
    {"e2808a", " "},    // Hair Space
    {"e280af", " "},    // Narrow No-Break Space (NNBSP)
    {"e2819f", " "},    // Medium Mathematical Space (MMSP)
    {"e38080", " "},    // Ideographic Space
    

    // Polish symbols
    {"c485", "\u0105"},     // ą - LATIN SMALL LETTER A WITH OGONEK
    {"c484", "\u0104"},     // Ą - LATIN LARGE LETTER A WITH OGONEK
    {"c487", "\u0107"},     // ć - LATIN SMALL LETTER C WITH ACUTE
    {"e2809a", "\u0106"},   // Ć - LATIN LARGE LETTER C WITH ACUTE
    {"c499", "\u0119"},     // ę - LATIN SMALL LETTER E WITH OGONEK
    {"e280a0", "\u0118"},   // Ę - LATIN LARGE LETTER E WITH OGONEK
    {"c582", "\u0142"},     // ł - LATIN SMALL LETTER L WITH STROKE
    {"c5a0", "\u0141"},     // Ł - LATIN LARGE LETTER L WITH STROKE
    {"c584", "\u0144"},     // ń - LATIN SMALL LETTER N WITH ACUTE
    {"e280b9", "\u0143"},   // Ń - LATIN LARGE LETTER N WITH ACUTE
    {"c3b3", "\u00f3"},     // ó - LATIN SMALL LETTER O WITH ACUTE
    {"c393", "\u00d3"},     // Ó - LATIN LARGE LETTER O WITH ACUTE
    {"c59b", "\u015b"},     // ś - LATIN SMALL LETTER S WITH ACUTE
    {"c59a", "\u015a"},     // Ś - LATIN LARGE LETTER S WITH ACUTE
    {"e28098", "\u015a"},   // Ś - LATIN LARGE LETTER S WITH ACUTE
    {"c5ba", "\u017a"},     // ź - LATIN SMALL LETTER Z WITH ACUTE
    {"e284a2", "\u0179"},   // Ź - LATIN LARGE LETTER Z WITH ACUTE
    {"c5bc", "\u017c"},     // ż - LATIN SMALL LETTER Z WITH DOT
    {"e280ba", "\u017b"}    // Ż - LATIN LARGE LETTER Z WITH DOT
};

#endif  // _UNICODE_SYMBOLS_H