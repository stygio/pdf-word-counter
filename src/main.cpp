#include <assert.h>

#include "WordExtractor.h"


int main(int argc, char* argv[]) {

    assert((argc > 1) && "No arguments passed to program.");
    assert((argc < 3) && "Too many arguments given to program.");
    
    WordExtractor wordExtractor;
    const char* pdfFilePath = argv[1];
    
    try {
        wordExtractor.Init(pdfFilePath);
    } catch(PoDoFo::PdfError & eCode) {
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