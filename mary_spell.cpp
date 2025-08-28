#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    ifstream dictFile("directory.txt");
    if (!dictFile) {
        cerr << "Error: Could not open dictionary file.\n";
        return 1;
    }

    vector<string> words;
    string word;

    // Load dictionary words into the vector
    while (dictFile >> word) {
        words.push_back(word);
    }
    dictFile.close();

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Could not open input file.\n";
        return 1;
    }

    // Check each word in the input file
    while (inputFile >> word) {
        // Optional: Normalize word (e.g., lowercase)
        string normalized = word;
        transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

        // Check if word is in dictionary
        bool found = find(words.begin(), words.end(), normalized) != words.end();
        if (!found) {
            cout << word << '\n'; // Print misspelled word
        }
    }

    inputFile.close();
    return 0;
}
