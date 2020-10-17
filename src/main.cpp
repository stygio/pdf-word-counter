#include <iostream>
#include <string>
#include <stack>
#include <assert.h>
#include <podofo/podofo.h>

int main(int argc, char* argv[]) {

    assert((argc > 1) && "No arguments passed to program.");
    PoDoFo::PdfMemDocument pdf(argv[1]);

    try {
        if (pdf.IsLoaded()) {
            for (int pn = 0; pn < pdf.GetPageCount(); ++pn) {
                // Retrieve the current page and initialize a PdfContentsTokenizer
                PoDoFo::PdfPage* page = pdf.GetPage(pn);
                std::cout << std::endl << "Page " << page->GetPageNumber() << std::endl;
                PoDoFo::PdfContentsTokenizer tok(page);

                // Initialize a stack of PdfVariants
                std::stack<PoDoFo::PdfVariant> stack;

                // trio of (type, token, var) to read stream from page
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
                while (tok.ReadNext(type, token, var)) {
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

                                // textBlockFlag = false;
                            }

                            // Text processing
                            if (textBlockFlag) {
                                if (strcmp(token, "Tf") == 0) {
                                    currentFontSize = stack.top().GetReal();
                                    stack.pop();
                                    PoDoFo::PdfName fontName = stack.top().GetName();
                                    stack.pop();

                                    PoDoFo::PdfObject* font = page->GetFromResources(PoDoFo::PdfName("Font"), fontName);
                                    if(!font)
                                        throw "Cannot create font!";

                                    currentFont = pdf.GetFont(font);
                                    if(!currentFont)
                                        throw "WARNING: Unable to create font!";

                                    // std::cout << "Current Font:" << fontName.GetName() << std::endl;
                                    // std::cout << "Current Encoding:" << currentFont->GetEncoding() << std::endl;
                                }
                                else if (strcmp(token, "Tj") == 0 || strcmp(token, "\'") == 0 || strcmp(token, "\"") == 0 || strcmp(token, "\\") == 0) {
                                    PoDoFo::PdfString tmpString = var.GetString();

                                    PoDoFo::PdfString unicode = currentFont->GetEncoding()->ConvertToUnicode(tmpString, currentFont);
                                    std::cout << unicode.GetStringUtf8().c_str(); 
                                    // std::cout << " " << token << std::endl;

                                }
                                else if (strcmp(token, "TJ") == 0) {
                                    PoDoFo::PdfArray& pdfArray = var.GetArray();

                                    for(size_t i = 0; i < pdfArray.GetSize(); ++i) {
                                        if (pdfArray[i].IsString() || pdfArray[i].IsHexString()) {
                                            PoDoFo::PdfString tmpString = pdfArray[i].GetString();

                                            // const PoDoFo::PdfEncoding* pEncoding = new PoDoFo::PdfMacRomanEncoding();
                                            // PoDoFo::PdfFont *pFont = pdf.CreateFont("LiberationSerif", false, false, pEncoding );
                                            // currentFont->GetEncoding()->ConvertToEncoding(tmpString, pFont);
                                            
                                            PoDoFo::PdfString unicode = currentFont->GetEncoding()->ConvertToUnicode(tmpString, currentFont);
                                            std::cout << unicode.GetStringUtf8().c_str();
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
        }

    } catch( PoDoFo::PdfError & eCode ) {
        /*
         * We have to check if an error has occurred.
         * If yes, we return and print an error message
         * to the commandline.
         */
        eCode.PrintErrorMsg();
        return eCode.GetError();
    }

    return 0;
}