#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <cctype>
#include <algorithm>
// Mary Kilbourne, Anthony Sessoms, and Enrique Gutierrez collaborated on a program to encrypt a file
using namespace std;

// --- From Mary: Builds cipher alphabet using keyword + reversed remaining letters
string createCipherAlphabet_Mary(const string& keyword) {
    string cipher = "";
    for (char c : keyword) {
        if (isalpha(c) && cipher.find(toupper(c)) == string::npos) {
            cipher += toupper(c);
        }
    }
    for (char c = 'Z'; c >= 'A'; --c) {
        if (cipher.find(c) == string::npos) {
            cipher += c;
        }
    }
    return cipher;
}

// --- From Anthony: Builds cipher alphabet using keyword + forward remaining letters
string createCipherAlphabet_Anthony(const string& keyword) {
    string key = "";
    string seen = "";
    for (char ch : keyword) {
        ch = toupper(ch);
        if (isalpha(ch) && seen.find(ch) == string::npos) {
            key += ch;
            seen += ch;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (key.find(ch) == string::npos) {
            key += ch;
        }
    }
    return key;
}

// --- From Enrique: Builds cipher alphabet using keyword + reversed remaining letters 
string createCipherAlphabet_Enrique(const string& keyword) {
    string key;
    vector<bool> seen(26, false);
    for (char c : keyword) {
        c = toupper(c);
        if (isupper(c) && !seen[c - 'A']) {
            seen[c - 'A'] = true;
            key += c;
        }
    }
    for (char c = 'Z'; c >= 'A'; --c) {
        if (!seen[c - 'A']) {
            key += c;
        }
    }
    return key;
}

// --- From All: Encrypts a character using the cipher map
char encryptChar(char ch, const map<char, char>& encMap) {
    if (isalpha(ch)) {
        char upper = toupper(ch);
        char result = encMap.at(upper);
        return islower(ch) ? tolower(result) : result;
    }
    return ch;
}

// --- From All: Main function: uses fixed input/output and lets you choose cipher mode
int main() {
    string keyword = "FEATHER";
    string mode = "mary";       // Options: mary, anthony, enrique

    // Choose cipher alphabet based on who made what cipher
    string cipherAlphabet;
    if (mode == "mary") {
        cipherAlphabet = createCipherAlphabet_Mary(keyword);
    }
    else if (mode == "anthony") {
        cipherAlphabet = createCipherAlphabet_Anthony(keyword);
    }
    else if (mode == "enrique") {
        cipherAlphabet = createCipherAlphabet_Enrique(keyword);
    }
    else {
        cerr << "Unknown mode: " << mode << endl;
        return 1;
    }

    map<char, char> encMap;
    for (int i = 0; i < 26; ++i) {
        encMap['A' + i] = cipherAlphabet[i];
    }

    ifstream inFile("encrypt.txt");
    ofstream outFile("output.txt");

    if (!inFile || !outFile) {
        cerr << "Error opening files.\n";
        return 1;
    }

    char ch;
    while (inFile.get(ch)) {
        outFile.put(encryptChar(ch, encMap));
    }

    cout << "The file has been encrypted.\n";
    return 0;
}
