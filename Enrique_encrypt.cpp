#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cctype>
#include <algorithm>

using namespace std;

static inline bool is_letter(char c) {
    return isalpha(static_cast<unsigned char>(c));
}

static string build_cipher_alphabet(const string &keyword) {
    string key;
    key.reserve(26);
    vector<bool> seen(26, false);

    // Add keyword letters (A–Z, uppercase, no duplicates)
    for (char c : keyword) {
        if (islower(static_cast<unsigned char>(c)))
            c = static_cast<char>(toupper(c));
        if (isupper(static_cast<unsigned char>(c))) {
            int idx = c - 'A';
            if (!seen[idx]) {
                seen[idx] = true;
                key.push_back(c);
            }
        }
    }

    // Append remaining letters (Z → A)
    for (char c = 'Z'; c >= 'A'; --c) {
        int idx = c - 'A';
        if (!seen[idx]) {
            seen[idx] = true;
            key.push_back(c);
        }
    }

    if (key.size() != 26) {
        throw runtime_error("Internal error: cipher alphabet length != 26");
    }

    return key;
}

static void usage(const char *prog) {
    cerr << "Usage: " << prog << " [-d] -kKEYWORD <input> <output>\n"
         << "  -d           Decrypt (default: encrypt)\n"
         << "  -kKEYWORD    Keyword for substitution (required)\n"
         << "Example:\n"
         << "  " << prog << " -kFEATHER plain.txt cipher.txt\n"
         << "  " << prog << " -d -kFEATHER cipher.txt plain_out.txt\n";
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool decrypt = false;
    string keyword;
    vector<string> positional;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d") {
            decrypt = true;
        } else if (arg.rfind("-k", 0) == 0) {
            keyword = arg.substr(2);
        } else if (!arg.empty() && arg[0] == '-') {
            cerr << "Unknown option: " << arg << "\n";
            usage(argv[0]);
            return EXIT_FAILURE;
        } else {
            positional.push_back(arg);
        }
    }

    if (keyword.empty() || positional.size() != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    string cipher;
    try {
        cipher = build_cipher_alphabet(keyword);
    } catch (const exception &e) {
        cerr << "Error building cipher: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Forward and reverse mappings
    array<char, 26> forward{};
    array<char, 26> reverse{};
    for (int i = 0; i < 26; ++i) {
        char plain = char('A' + i);
        char coded = cipher[i];
        forward[i] = coded;
        reverse[coded - 'A'] = plain;
    }

    ifstream in(positional[0]);
    if (!in) {
        cerr << "Error: could not open input file: " << positional[0] << "\n";
        return EXIT_FAILURE;
    }

    ofstream out(positional[1]);
    if (!out) {
        cerr << "Error: could not open output file: " << positional[1] << "\n";
        return EXIT_FAILURE;
    }

    char ch;
    while (in.get(ch)) {
        if (is_letter(ch)) {
            bool lower = islower(static_cast<unsigned char>(ch));
            char up = static_cast<char>(toupper(ch));
            char sub = decrypt ? reverse[up - 'A'] : forward[up - 'A'];
            if (lower) sub = static_cast<char>(tolower(sub));
            out.put(sub);
        } else {
            out.put(ch);
        }
    }

    return EXIT_SUCCESS;
}
