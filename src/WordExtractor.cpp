#include <iostream>
#include <stdio.h>
#include <stack>
#include <codecvt>
#include <locale>
#include <uchar.h>

#include "WordExtractor.h"
#include "UnicodeSymbols.h"


WordExtractor::WordExtractor() {}
WordExtractor::~WordExtractor() {}


void WordExtractor::Init(const char* documentPath) {
    if(!documentPath)
        throw "No file path passed to WordExtractor::Init.";

    PoDoFo::PdfMemDocument pdfDocument(documentPath);

    size_t documentPageCount = pdfDocument.GetPageCount();

    for(size_t pageNumber = 0; pageNumber < documentPageCount; ++pageNumber) {
        PoDoFo::PdfPage* p_pdfPage = pdfDocument.GetPage(pageNumber);
        this->ExtractWords(&pdfDocument, p_pdfPage);
    }
}


std::string WordExtractor::UnicodePdfString_to_StdString(PoDoFo::PdfString unicodeDataString) {
    // Convert to UTF8 and initialize string of unsigned char code representations
    std::string dataString = unicodeDataString.GetStringUtf8().c_str();
    std::string charCodes;

    // Creating charCodes array of unsigned char byte hex codes to verify text symbols
    for(size_t i = 0; i < dataString.length(); ++i) {
        char hexCode[5];
        sprintf(hexCode, "%02x", static_cast<unsigned char>(dataString[i]));
        charCodes.append(hexCode);
        // printf("%02x", static_cast<unsigned char>(dataString[i]));
    }

    // Replacing erroneous symbols with correct unicode values using UnicodeSymbols::symbolMap
    size_t index;
    for(auto it_symbolMap = symbolMap.begin(); it_symbolMap != symbolMap.end(); ++it_symbolMap) {
        const std::string hexCodeString = it_symbolMap->first;
        const std::string characterString = it_symbolMap->second;
        while((index = charCodes.find(hexCodeString)) != std::string::npos) {
            std::string zeroReplacement(characterString.length(), '0');
            charCodes.replace(index, hexCodeString.length(), zeroReplacement);
            dataString.replace(index/2, hexCodeString.length()/2, characterString);
        }
    }

    // Defining codeconverter type for wstring_convert
    typedef std::codecvt_utf8_utf16<char16_t> ccvt_c16t;
    // Constructs an object that returns byte_err or wide_err on failure to convert, instead of throwing an exception.
    std::wstring_convert<ccvt_c16t, char16_t> converter ("", u"");
    // Get u16string version of dataString with char16_t encoding (inherently 16bit/2bytes)
    std::u16string u16_DataString = converter.from_bytes(dataString);

    // Constant character definitions
    const char16_t capital_A = u'A';
    const char16_t capital_Z = u'Z';
    const char16_t small_A = u'a';
    const char16_t small_Z = u'z';
    const char16_t capitalSmallDifference = small_A - capital_A; 
    const char16_t apostrophe = u'\'';
    const char16_t latin_Start = u'\u00c0';
    const char16_t latin_End = u'\u024f';

    // Process characters in u16_DataString
    for(char16_t& c16: u16_DataString) {
        // Cast standard english alphabet to lower case characters
        if(capital_A <= c16 and c16 <= capital_Z)
            c16 += capitalSmallDifference;
        // Check if the character belongs to one of the allowed ranges (alphabetic). If not, replace with a space
        else if(not ((small_A <= c16 and c16 <= small_Z) or (c16 == apostrophe) or (latin_Start <= c16 and c16 <= latin_End)))
            c16 = u' ';
    }

    // Return std::string version of processed u16_DataString
    return converter.to_bytes(u16_DataString);
}


void WordExtractor::ExtractWords(PoDoFo::PdfMemDocument* p_pdfDocument, PoDoFo::PdfPage* p_pdfPage) {
    // Print the current page and initialize a PdfContentsTokenizer
    std::cout << std::endl << "Page " << p_pdfPage->GetPageNumber() << std::endl;
    PoDoFo::PdfContentsTokenizer pdfContentsTokenizer(p_pdfPage);

    // Initialize a stack of PdfVariants
    std::stack<PoDoFo::PdfVariant> stack;

    // Trio of (type, token, var) to read stream from page
    PoDoFo::EPdfContentsType type;
    const char* token = nullptr;
    PoDoFo::PdfVariant var;

    // Text spacing information
    double charSpace = 0.0;
    double wordSpace = 0.0;
    double horizontalScaling = 100;
    
    // Font information
    double currentFontSize = 0.0;
    PoDoFo::PdfFont* currentFont = NULL;

    // Go through the page
    bool textBlockFlag = false;
    while (pdfContentsTokenizer.ReadNext(type, token, var)) {
        switch (type) {
            case PoDoFo::ePdfContentsType_Keyword:
                // process token: it contains the current command
                //   pop from var stack as necessary

                // Check for Tc(CharSpace), Tw(WordSpace) and Th(HorizontalScaling) operators
                if (strcmp(token, "Tc") == 0) {
                    charSpace = var.GetReal();
                    std::cout << "CharSpace (Tc): " << charSpace << std::endl;
                }
                if (strcmp(token, "Tw") == 0) {
                    wordSpace = var.GetReal();
                    std::cout << "WordSpace (Tw): " << wordSpace << std::endl;
                }
                if (strcmp(token, "Th") == 0) {
                    horizontalScaling = var.GetReal();
                    std::cout << "HorizontalScaling (Th): " << horizontalScaling << std::endl;
                }

                // Check for BT(BeginText)/ET(EndText) operators
                if (strcmp(token, "BT") == 0) { 
                    textBlockFlag = true;
                } 
                else if (strcmp(token, "ET") == 0) {
                    if (!textBlockFlag)
                        std::cout << "WARNING: Encountered PoDoFo::EPDFContentsType_Keyword 'ET' without preceeding 'BT' on page!" << std::endl;

                    textBlockFlag = false;
                }

                // Text processing
                if (textBlockFlag) {
                    if (strcmp(token, "Tf") == 0) {
                        currentFontSize = stack.top().GetReal();
                        stack.pop();
                        PoDoFo::PdfName fontName = stack.top().GetName();
                        stack.pop();

                        PoDoFo::PdfObject* font = p_pdfPage->GetFromResources(PoDoFo::PdfName("Font"), fontName);
                        if(!font)
                            throw "Cannot create font!";

                        currentFont = p_pdfDocument->GetFont(font);
                        if(!currentFont)
                            throw "WARNING: Unable to create font!";

                    }
                    else if (strcmp(token, "Tj") == 0 || strcmp(token, "\'") == 0 || strcmp(token, "\"") == 0 || strcmp(token, "\\") == 0) {
                        PoDoFo::PdfString pdfString = var.GetString();

                        std::string dataString = this->UnicodePdfString_to_StdString(currentFont->GetEncoding()->ConvertToUnicode(pdfString, currentFont));
                        std::cout << dataString;

                    }
                    else if (strcmp(token, "TJ") == 0) {
                        PoDoFo::PdfArray& pdfArray = var.GetArray();

                        for(size_t i = 0; i < pdfArray.GetSize(); ++i) {
                            if (pdfArray[i].IsString() || pdfArray[i].IsHexString()) {
                                PoDoFo::PdfString pdfString = pdfArray[i].GetString();
                                
                                std::string dataString = this->UnicodePdfString_to_StdString(currentFont->GetEncoding()->ConvertToUnicode(pdfString, currentFont));
                                std::cout << dataString;
                            } 
                            else if (pdfArray[i].IsNumber()) {
                                // The horizontal displacement between glyphs is calculated based on the following formula:
                                // t_x = ((w0 - (T_j/1000)) * T_fs + T_c + T_w) * T_h
                                // where
                                // w0 denotes the glyph's original horizontal position
                                // T_j denotes a number in a TJ array, specifying the position adjustment in thousandths of a unit of text space
                                // T_fs denotes the current text font size
                                // T_h denotes the horizontal scaling parameter
                                // T_c and T_w denote the current character and and word spacing parameters
                                //
                                // This information is taken from official documentation on the PDF format found here:
                                // https://www.adobe.com/content/dam/acom/en/devnet/pdf/PDF32000_2008.pdf

                                double spacing = pdfArray[i].GetNumber();
                                double t_x = ((0 - spacing/1000) * currentFontSize + charSpace + wordSpace) * horizontalScaling;
                                // std::cout << "Spacing: " << spacing << ", Displacement: " << t_x << std::endl;
                                if (t_x > 50) 
                                    std::cout << " ";
                            }
                        }
                    }
                }
                break;

            case PoDoFo::ePdfContentsType_Variant:
                // process var: push it onto a stack
                stack.push(var);
                break;
            
            default:
                // should not happen!
                break;
        }
    }
}