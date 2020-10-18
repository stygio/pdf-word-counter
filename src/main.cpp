#include "WordExtractor.h"


void print_usage() {
    printf("Usage: pdf-word-counter [PDF File Path]\n");
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
        print_usage();
        exit(-1);
    }
    
    WordExtractor wordExtractor;
    const char* pdfFilePath = argv[1];
    
    try {
        wordExtractor.Init(pdfFilePath);
    } catch(PoDoFo::PdfError & eCode) {
        /*
         * We have to check if an error has occurred as per PoDoFo specification on error handling.
         * If yes, we return and print an error message
         * to the commandline.
         */
        eCode.PrintErrorMsg();
        return eCode.GetError();
    }

    return 0;
}