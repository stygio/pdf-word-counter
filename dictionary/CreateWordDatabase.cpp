#include <stdio.h>
#include <fstream>

#include "../src/Dictionary.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sqlStatement;

    // Opening/creating the database
    rc = sqlite3_open("dictionary/words-alpha.db", &db);
    // Checking if the query was succesful
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Creating the table
    sqlStatement = "CREATE TABLE IF NOT EXISTS ENGLISH (WORD TEXT PRIMARY KEY NOT NULL);";
    rc = sqlite3_exec(db, sqlStatement.c_str(), callback, 0, &zErrMsg);
    // Checking if the query was succesful
    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    // Open text file with words
    // Each word is proceeded by a newline character with no other whitespace or formatting
    std::ifstream textFile("dictionary/words_alpha.txt");
    std::string line;
    // Write words into the database file
    if(textFile.is_open()) {
        int n = 1;
        char* buffer;
        while(std::getline(textFile, line)) {
            sprintf(buffer, "INSERT OR IGNORE INTO ENGLISH (WORD) VALUES (\'%s\');", line.c_str());
            sqlStatement = buffer;
            rc = sqlite3_exec(db, sqlStatement.c_str(), callback, 0, &zErrMsg);
            if(rc != SQLITE_OK){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                printf("\rInserted word nr. %d", n);
                ++n;
            }
        }
        textFile.close();
    } else {
        fprintf(stderr, "Unable to open file.");
    }

    sqlite3_close(db);
    return 0;
}