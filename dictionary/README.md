## Dictionary directory

I compiled a dictionary of english words for the verification of correct word extraction. The dictionary uses the word list provided by [dwyl/english-words](https://github.com/dwyl/english-words). A [database file](words-alpha.db) is generated using sqlite3 for C++. You can generate the database yourself using the [source](CreateWordDatabase.cpp).