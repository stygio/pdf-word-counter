﻿#ifndef _WORD_EXTRACTOR_H
#define _WORD_EXTRACTOR_H

#include <podofo/podofo.h>


class WordExtractor {
    public:
        WordExtractor();
        virtual ~WordExtractor();

        void Init(const char* documentPath);

    private:
        std::string UnicodePdfString_to_StdString(PoDoFo::PdfString unicodeDataString);
        void ExtractWords(PoDoFo::PdfMemDocument* pdfDocument, PoDoFo::PdfPage* pdfPage);

};

#endif  // _WORD_EXTRACTOR_H