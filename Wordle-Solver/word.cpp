#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "word.h"
#include <Windows.h>

const double MINBITS = 5.0; // Min number of bits to start trying to guess the word, MAX is 7.92

using namespace std;

int count(string str, char letter) {
	int cnt = 0;
	for (char& l : str) {
		if (l == letter) cnt++;
	}
	return cnt;
}

string Word::answer;

Word::Word(string w) : word(w) {}

Word::Word() : word("") {}

bool Word::isCorrect() const { return word == answer; }

void Word::setAnswer(string ans) { answer = ans; }

/* Updates the hints list from the words and answer
* @pre (answer != NULL && answer.length() == 5)
*/
void Word::updateHints() {
	if (answer == "") {
		for (int i = 0; i < 5; i++) {
			cout << "Hint " << (i + 1) << ": " << flush;
			int j;
			cin >> j;
			hints.push_back(j);
		}
		return;
	}

	for (int i = 0; i < word.length(); i++) {
		string cword = word.substr(0, i + 1);
		char cletter = word[i]; // The current words letter @ index i
		char aletter = answer[i]; // The answers letter @ index i
		if (cletter == aletter) { hints.push_back(1); }
		else if (answer.find(cletter) != string::npos && count(answer, cletter) >= count(cword, cletter)) {
			hints.push_back(0);
		}
		else { hints.push_back(-1); }
	}
}

/*
* Finds the amount of bits of information, E[Info] that the word provides
* 
* @param words the words to compare word with
* 
* @returns double the expected information E[I]
*/
double Word::expectedInfo(vector<string> words) const {
	// 243, 81, 27, 9, 3
	vector<int> matches(243, 0); // 3^5
	for (string cword : words) {
		if (cword == word) continue; // Skip if the words match
		int cnter = 81; // Start at 5th letter
		int i = 0;
		for (int j = 0; j < cword.length(); j++) {
			char cletter = cword[j];
			char aletter = word[j]; // The actual word
			if (cletter == aletter) { // Green
				i += cnter * 2; // Third part is green
			}
			// If cletter is in word and the number of cletters in word is greater than or equal to
			// number of cletters in cword up to index j inclusive
			else if (word.find(cletter) != string::npos &&
				count(word, cletter) >= count(cword.substr(0, j+1), cletter)) {
				i += cnter; // Second part is yellow
			}
			// First part is red, no need to add to index
			cnter /= 3;
		}
		matches[i]++;
	}
	double sum = 0;
	for (int p : matches) {
		double px = 1.0 * p / words.size();
		if (px != 0) { // log2 is undefined
			sum += px * -log2(px); // Formula for calculating average number of bits ; sum of p(x) * -log2(p(x))
		}
	}
	return sum;
}


ostream& operator<<(std::ostream& os, const Word& word) {
	os << word.word;
	return os;
}

/*
* Filters out any words in words with the current word
* 
* @param words the words to filter through
* @param double if bits is greater than MINBITS then greens are filtered, 
*	if it is less than, then greens are not filtered (not enough information to make an actual guess)
* 
* @returns vector<string>& a reference to the words vector passed in
* 
* @pre (hints.size() > 0)
*/
vector<string>& Word::filterWords(vector<string>& words, double bits) {
	for (size_t i = words.size(); i-->0 ; ) {
		for (int j = 0; j < words[i].length();j++) {
			char cletter = words[i][j];
			char aletter = word[j];
			/* If cletter and aletter arent equal, erase IF bits >= MINBITS.
			Also if cletter doesnt equals aletter and cletter is in word, erase
			To sum this up if cletter doesnt equal aletter, 
			then erase if either bits >= MINBITS or if cletter is in word
			*/
			if (hints[j] == 1 && cletter != aletter && 
				(word.find(cletter) != string::npos || bits >= MINBITS) || // Green
				/* If equal, erase OR if aletter not in words[i], erase
				Add a feature that basically works with duplicates, so if theres one yellow, 
				one grey, same letter, filter out any words with duplicates*/
				(hints[j] == 0 && (cletter == aletter || words[i].find(aletter) == string::npos)) || //Yellow
				(hints[j] == -1 && // Grey
				/* If equal, erase.
				Also if the first hint in word with letter cletter is -1 and cletter is in words[i], 
				also erase*/
				cletter == aletter || (word.find(cletter) != string::npos &&
				hints[word.find(cletter)] == -1 && words[i].find(cletter) != string::npos
				))) {
				words.erase(words.begin() + i);
				break;
			}
		}
	}
	return words;
}

Word::operator bool() const {
	return word != "";
}