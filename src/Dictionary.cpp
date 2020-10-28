#include "Dictionary.h"


Dictionary::Dictionary() {}
Dictionary::~Dictionary() {}


static int base_callback(void* context, int columnCount, char** columnValues, char** columnNames){
    Dictionary* dictionary = static_cast<Dictionary*>(context);
    dictionary->Callback(columnCount, columnValues, columnNames);
    return 0;
}


void Dictionary::Callback(int columnCount, char** columnValues, char** columnNames) {
    this->sqlRetVal = (bool)atoi(columnValues[0]);
}


void Dictionary::Init(const char* databasePath, std::string language) {
    Dictionary::language = language;
    int returnCode;

    // Opening/creating the database and checking if the query was succesful
    returnCode = sqlite3_open(databasePath, &this->database);
    if(returnCode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    }

    /* Create sql query */
    std::string sqlStatement;
    sqlStatement = "SELECT count(*) FROM sqlite_master WHERE type=\'table\' AND name=\'" + this->language + "\';";

    /* Execute SQL statement */
    char* errorMessage = 0;
    returnCode = sqlite3_exec(this->database, sqlStatement.c_str(), base_callback, this, &errorMessage);
    if(returnCode != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else if (!this->sqlRetVal) {
        fprintf(stderr, "Invalid language choice: %s\n", this->language.c_str());
    }

    fprintf(stdout, "Opened %s dictionary successfully.\n", this->language.c_str());
}


bool Dictionary::AddWord(const char* word) {
    char* errorMessage;
    std::string sqlStatement = "INSERT OR IGNORE INTO " + this->language + " (WORD) VALUES (\'" + word + "\');";

    int returnCode = sqlite3_exec(this->database, sqlStatement.c_str(), 0, 0, &errorMessage);
    if(returnCode != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
        return false;
    } else {
        return true;
    }
}


bool Dictionary::RemoveWord(const char* word) {
    char* errorMessage;
    std::string sqlStatement = "DELETE FROM " + this->language + " WHERE WORD = \'" + word + "\';";

    int returnCode = sqlite3_exec(this->database, sqlStatement.c_str(), 0, 0, &errorMessage);
    if(returnCode != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
        return false;
    } else {
        return true;
    }
}


bool Dictionary::Lookup(const char* word) {
    char* errorMessage;
    std::string sqlStatement = "SELECT EXISTS (SELECT 1 FROM " + this->language + " WHERE WORD = \'" + word + "\');";

    int returnCode = sqlite3_exec(this->database, sqlStatement.c_str(), base_callback, this, &errorMessage);
    if(returnCode != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
        return false;
    } else if (!this->sqlRetVal) {
        return false;
    } else {
        return true;
    }
}


void Dictionary::CloseDictionary() {
    fprintf(stderr, "Closed dictionary.");
    sqlite3_close(this->database);
}