#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cctype> // for tolower

using namespace std;

// Convert a string to lowercase in place
static inline void to_lower_inplace(string &s) {
    for (char &c : s) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
}

// Load dictionary into an unordered_set for fast lookup
static unordered_set<string> load_dictionary(const string &dict_path) {
    ifstream in(dict_path);
    if (!in) {
        cerr << "Error: could not open dictionary file: " << dict_path << "\n";
        exit(EXIT_FAILURE);
    }

    unordered_set<string> dict;
    string line;
    dict.reserve(300000 * 2); // Pre-reserve for performance

    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Handle Windows line endings
        }
        to_lower_inplace(line);
        if (!line.empty()) {
            dict.insert(move(line));
        }
    }
    return dict;
}

static inline bool is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Normalize a token: lowercase, keep only letters and valid apostrophes
static string normalize_token(const string &token) {
    string out;
    out.reserve(token.size());
    for (size_t i = 0; i < token.size(); ++i) {
        char ch = token[i];
        if (is_letter(ch)) {
            out.push_back(static_cast<char>(tolower(static_cast<unsigned char>(ch))));
        } 
        // Keep apostrophe if it’s surrounded by letters (e.g., don't, it's)
        else if (ch == '\'' && i > 0 && i + 1 < token.size() 
                 && is_letter(token[i-1]) && is_letter(token[i+1])) {
            out.push_back(ch);
        }
    }
    // Remove leading or trailing apostrophes
    if (!out.empty() && (out.front() == '\'' || out.back() == '\'')) {
        out.erase(out.begin()); // Remove leading apostrophe
        if (!out.empty() && out.back() == '\'') {
            out.pop_back(); // Remove trailing apostrophe
        }
    }
    return out;
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <textfile> [dictionary]\n"
             << "Default dictionary: /usr/share/dict/words\n";
        return EXIT_FAILURE;
    }
    string text_path = argv[1];
    string dict_path = (argc >= 3) ? argv[2] : "/usr/share/dict/words";

    unordered_set<string> dict = load_dictionary(dict_path);

    ifstream in(text_path); // no need for binary mode
    if (!in) {
        cerr << "Error: could not open text file: " << text_path << "\n";
        return EXIT_FAILURE;
    }

    unordered_set<string> unknown;
    string token;
    token.reserve(64);
    char ch;

    while (in.get(ch)) {
        if (is_letter(ch) || ch == '\'') {
            token.push_back(ch);
        } else {
            if (!token.empty()) {
                string w = normalize_token(token);
                if (!w.empty() && dict.find(w) == dict.end()) {
                    unknown.insert(move(w));
                }
                token.clear();
            }
        }
    }

    // Handle last token at EOF
    if (!token.empty()) {
        string w = normalize_token(token);
        if (!w.empty() && dict.find(w) == dict.end()) {
            unknown.insert(move(w));
        }
    }

    // Move into vector and sort for output
    vector<string> out(make_move_iterator(unknown.begin()), make_move_iterator(unknown.end()));
    sort(out.begin(), out.end());

    for (const string &w : out) {
        cout << w << '\n';
    }

    return 0;
}
