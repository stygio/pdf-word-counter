#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <string>
#include <sqlite3.h>


class Dictionary {
    public:
        Dictionary();
        virtual ~Dictionary();

        void Init(const char* databasePath, std::string language);
        void Callback(int columnCount, char** columnValues, char** columnNames);
        bool AddWord(const char* word);
        bool RemoveWord(const char* word);
        bool Lookup(const char* word);
        void CloseDictionary();

    private:
        std::string language;
        sqlite3* database;
        bool sqlRetVal;
};

#endif  // _DICTIONARY_H