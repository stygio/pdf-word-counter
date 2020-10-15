#include <iostream>
#include <string>
#include <assert.h>
#include <podofo/podofo.h>

int main(int argc, char *argv[]) {

    assert((argc > 1) && "No arguments passed to program.");
    
    PoDoFo::PdfMemDocument pdf(argv[1]);

    try {
        if (pdf.IsLoaded()){
            for (int pn = 0; pn < pdf.GetPageCount(); ++pn) {
                PoDoFo::PdfPage* page = pdf.GetPage(pn);
                std::cout << "Page " << page->GetPageNumber() << std::endl;
            
                PoDoFo::PdfContentsTokenizer tok(page);
                const char* token = nullptr;
                PoDoFo::PdfVariant var;
                PoDoFo::EPdfContentsType type;
                while (tok.ReadNext(type, token, var)) {
                    switch (type) {
                        case PoDoFo::ePdfContentsType_Keyword:
                            // process token: it contains the current command
                            //   pop from var stack as necessary
                            std::cout << token << std::endl;
                            break;
                        case PoDoFo::ePdfContentsType_Variant:
                            // process var: push it onto a stack
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