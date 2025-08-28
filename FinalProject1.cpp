#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cctype>
// Mary Kilbourne, Anthony Sessoms, and Enrique Gutierrez collaborated on a spell checker based off a dictionary
using namespace std;

// --- From Enrique: Lowercase conversion utility ---
string to_lower_str(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// --- From Anthony: Dictionary loader using unordered_set for fast lookup ---
// --- From Mary: Also stores dictionary in a vector to match pseudocode ---
void loadDictionary(const string& filename, unordered_set<string>& dict, vector<string>& wordsVec) {
    ifstream dictFile(filename);
    string word;
    if (!dictFile) {
        cerr << "Error: Could not open dictionary file: " << filename << endl;
        exit(1);
    }

    while (dictFile >> word) {
        word = to_lower_str(word);
        dict.insert(word);
        wordsVec.push_back(word); // --- Mary’s pseudocode: Append to vector ---
    }
    dictFile.close();
}

// --- From Anthony & Enrique: Word sanitization ---
string sanitizeWord(string word) {
    // Remove leading/trailing non-alpha characters
    while (!word.empty() && !isalpha(word.front())) word.erase(0, 1);
    while (!word.empty() && !isalpha(word.back())) word.pop_back();
    return to_lower_str(word);
}

int main() {
    unordered_set<string> dictionary;
    vector<string> words; // --- From Mary: vector of strings called words ---

    string dictPath = "dictionary.txt"; // --- From Mary: Open dictionary file ---
    string inputPath = "input.txt";     // --- From Mary: Open file to be checked ---

    // --- From Mary: Load dictionary words into vector ---
    loadDictionary(dictPath, dictionary, words);

    // --- From Mary: Open the file to be checked ---
    ifstream inputFile(inputPath);
    if (!inputFile) {
        cerr << "Error: Could not open input file: " << inputPath << endl;
        return 1;
    }

    string word;
    while (inputFile >> word) {
        string sanitized = sanitizeWord(word);

        // --- From Mary: If word not in dictionary, print it ---
        if (!sanitized.empty() && dictionary.find(sanitized) == dictionary.end()) {
            cout << "Misspelled word: " << word << endl; // --- From Anthony: labeled output ---
        }
    }

    inputFile.close();
    return 0;
}
