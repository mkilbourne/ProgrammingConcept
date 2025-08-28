#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

// Create the cypher using keyword
string createCipherAlphabet(const string& keyword) {
	string cipher = "";
	//add the keyword
	for (char c : keyword) {
		if (isalpha(c) && cipher.find(toupper(c)) == string::npos) {
			;
			cipher += toupper(c);
		}
	}
	// Append remaining letters in reverse
	string reversedAlphabet = "";
	for (char c = 'Z'; c >= 'A'; --c) {
		if (cipher.find(c) == string::npos) {
			reversedAlphabet += c;
		}
	}
	return cipher + reversedAlphabet;
}
//function to encrypt a file
void encryptFile(const string& inputFile, const string& outputFile, const string& keyword) {
	string cipherAlphabet = createCipherAlphabet(keyword);
	map<char, char> substitutionMap;
	for (int i = 0; i < 26; i++) {
		substitutionMap['A' + i] = cipherAlphabet[i];
		substitutionMap['a' + i] = tolower(cipherAlphabet[i]);
	}
	ifstream inFile(inputFile);
	ofstream outFile(outputFile);
	if (!inFile.is_open() || !outFile.is_open()) {
		cerr << "Error opening files." << endl;
		return;
	}
	char ch;
	while (inFile.get(ch)) {
		if (isalpha(ch)) {
			if (isupper(ch)) {
				outFile << substitutionMap[ch];
			}
			else {
				outFile << substitutionMap[ch];
			}
		}
		else {
			outFile << ch;
		}
	}
	inFile.close();
	outFile.close();
	cout << "File encrypted sucessfully" << endl;
}
// Main function for demonstration
int main() {
	string keyword = "FEATHER";
	encryptFile("encrypt.txt", "output.txt", keyword);
	return 0;
}
