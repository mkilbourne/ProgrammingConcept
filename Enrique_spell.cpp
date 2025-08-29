#include <bits/stdc++.h>
using namespace std;

static inline string to_lower_str(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

static unordered_set<string> load_dictionary(const string &dict_path) {
    ifstream in(dict_path);
    if (!in) {
        cerr << "Error: could not open dictionary file: " << dict_path << "\n";
        exit(EXIT_FAILURE);
    }
    vector<string> words; // as required by pseudocode
    words.reserve(300000);

    string line;
    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        string w = to_lower_str(line);
        if (!w.empty()) words.push_back(w);
    }

    unordered_set<string> dict;
    dict.reserve(words.size() * 2);
    for (const string &w : words) dict.insert(w);
    return dict;
}

static inline bool is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static string normalize_token(const string &token) {
    string out;
    out.reserve(token.size());
    for (size_t i = 0; i < token.size(); ++i) {
        char ch = token[i];
        if (is_letter(ch)) {
            out.push_back((char)tolower((unsigned char)ch));
        } else if (ch == '\'' && i > 0 && i + 1 < token.size() && is_letter(token[i-1]) && is_letter(token[i+1])) {
            // drop interior apostrophes to match typical dictionary entries
            continue;
        } else {
            // ignore other characters
        }
    }
    return out;
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <textfile> [dictionary=/usr/share/dict/words]\n";
        return EXIT_FAILURE;
    }
    string text_path = argv[1];
    string dict_path = (argc >= 3) ? argv[2] : string("/usr/share/dict/words");

    unordered_set<string> dict = load_dictionary(dict_path);

    ifstream in(text_path, ios::binary);
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
                if (!w.empty() && dict.find(w) == dict.end()) unknown.insert(w);
                token.clear();
            }
        }
    }
    if (!token.empty()) {
        string w = normalize_token(token);
        if (!w.empty() && dict.find(w) == dict.end()) unknown.insert(w);
    }

    vector<string> out(unknown.begin(), unknown.end());
    sort(out.begin(), out.end());
    for (const string &w : out) cout << w << '\n';
    return 0;
}

