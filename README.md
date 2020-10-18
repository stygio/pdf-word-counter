## PDF Word Counter

A tool for text analysis. Currently working towards offering word distribution analysis and other options for text extracted from pdf files.


### Motivation

In a few weeks I will defend my master's thesis and finish my studies in system modelling and data analysis. This course of study has further ignited my interest in data science and is currently motivating me to look for interesting ways to look at data, interpret it and make visualizations. A few years I go I read about [Zipf's Law](https://en.wikipedia.org/wiki/Zipf%27s_law "Zipf's Law - Wikipedia"), which states that the most frequent word in a sufficiently large corpus of text "will occur approximately twice as often as the second most frequent word, three times as often as the third most frequent word, etc. (...)", irregardless of the language. I spoke with a friend about creating a tool to analyze files locally and see if this is true. I recently remembered this idea and decided to implement it, possibly expanding upon this concept after its completion.


### Implementation

The goal of this project is to offer a variety of interesting insights into text. The tool is intended for use on a variety of documents, such as books, resumes or articles. As such, I decided that developing it for PDF files would be the most relevant.

I use the PoDoFo library to extract text from PDF files. This works better on some files and others due to the specifics of encoding. For example, some files tend to use spaces and encode entire words, while others instead have encoded information about spacing between characters. For the latter case, this spacing is used both in words and for word spacing, so it can be hard to tell where to actually seperate words. I plan to use a dictionary, possibly in the form of a database, to check for words in a semi-intelligent manner to improve the dependability of this tool.

Later, I intend to analyze the text and find some interesting relations or verify certain principles, such as the above mentioned Zipf's law, as well as visualizations in the form of graphs and/or word clouds.


### Dependencies

Uses PoDoFo and requires linking to it and all of its dependencies. The PoDoFo build I use is compiled with MinGW as a 32-bit binary for compatibility with precompiled GnuWin32 libraries. I compiled it with support for only its most basic capabilities with the absolutely required libraries (zlib and freetype2). As such, you will need to build a copy of PoDoFo with these dependencies in mind. If at all possible, it is recommended to use the same compiler and configuration for all libs.

* PoDoFo: http://podofo.sourceforge.net/
* Zlib: https://zlib.net/
* FreeType2: https://www.freetype.org/
* GetGnuWin32 (Allows the batch download and installation of all GnuWin32 packages, slightly outdated but still works): https://sourceforge.net/projects/getgnuwin32/

When compiling the project you will need to link at least `-lpodofo -lz -lfreetype` and, in the case of Windows, `-lgdi32 -lws_32`. Feel free to look at [my tasks.json file](.vscode/tasks.json) which gets called when building with Visual Studio Code. This will offer some insights as to my own compile options.


### ToDo

* Support for more ligatures or language symbols (currently only supporting latin ligatures and polish symbols).
* Various analytics with processed text.
* GUI for ease of use (this project was originally intended for a friend with limited knowledge of computer science).
* Graphing? Look into using [matplotlib-cpp](https://github.com/lava/matplotlib-cpp) or [matplot++](https://github.com/alandefreitas/matplotplusplus).