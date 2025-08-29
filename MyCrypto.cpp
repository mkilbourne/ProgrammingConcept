#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <cctype> // For isalpha, toupper, islower

using namespace std;

// Function to generate the cipher key based on a keyword
string generateKey(const string& keyword) {
    string key = "";
    string seenLetters = "";
    for (char ch : keyword) {
        ch = toupper(ch);
        if (ch >= 'A' && ch <= 'Z' && seenLetters.find(ch) == string::npos) {
            key += ch;
            seenLetters += ch;
        }
    }
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char ch : alphabet) {
        if (key.find(ch) == string::npos) {
            key += ch;
        }
    }
    return key;
}

// Function to encrypt or decrypt a character
char encryptDecryptChar(char ch, const map<char, char>& crypt_map, const map<char, char>& decrypt_map, bool decrypt) {
    if (isalpha(ch)) {
        char upper_ch = toupper(ch);
        char processedChar;
        if (decrypt) {
            processedChar = decrypt_map.at(upper_ch);
        } else {
            processedChar = crypt_map.at(upper_ch);
        }
        if (islower(ch)) {
            return tolower(processedChar);
        } else {
            return processedChar;
        }
    }
    return ch;
}

int main(int argc, char* argv[]) {
    string keyword = "";
    string inputFile = "";
    string outputFile = "";
    bool decrypt = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d") {
            decrypt = true;
        } else if (arg == "-k") {
            // Check if the next argument exists and isn't another flag
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                keyword = argv[i + 1];
                i++; // Increment i to skip the next argument (the keyword)
            } else {
                cerr << "Error: The -k flag must be followed by a keyword." << endl;
                return 1;
            }
        } else if (inputFile.empty()) {
            inputFile = arg;
        } else {
            outputFile = arg;
        }
    }

    // Validate arguments
    if (keyword.empty()) {
        cerr << "Error: A keyword must be supplied using the -k<KEYWORD> argument." << endl;
        return 1;
    }
    if (inputFile.empty() || outputFile.empty()) {
        cerr << "Error: Input and output files must be supplied." << endl;
        cerr << "Usage: ./MyCrypto [-d] -k <KEYWORD> inputFile outputFile" << endl;
        return 1;
    }

    // Generate the cipher key
    string cipherKey = generateKey(keyword);
    string normalAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Create the encryption and decryption maps
    map<char, char> crypt_map;
    map<char, char> decrypt_map;
    for (size_t i = 0; i < 26; ++i) {
        crypt_map[normalAlphabet[i]] = cipherKey[i];
        decrypt_map[cipherKey[i]] = normalAlphabet[i];
    }

    // Open input and output files
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error: The file '" << inputFile << "' could not be opened." << endl;
        return 1;
    }
    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "Error: The file '" << outputFile << "' could not be created." << endl;
        return 1;
    }

    // Process the file
    char ch;
    while (inFile.get(ch)) {
        outFile.put(encryptDecryptChar(ch, crypt_map, decrypt_map, decrypt));
    }

    // Close files
    inFile.close();
    outFile.close();

    cout << "Operation completed successfully." << endl;
    return 0;
}
