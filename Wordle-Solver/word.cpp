#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "word.h"
#include <Windows.h>
#define DEBUG

const double MINBITS = 5.0; // Min number of bits to start trying to guess the word, MAX is 7.92

using namespace std;

static int count(string str, string letter) {
	int cnt = 0;
	for (char l : str) {
		if (l + "" == letter) cnt++;
	}
	return cnt;
}

static int count(string str, char letter) {
	return count(str, letter + "");
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
	for (int i = 0; i < word.length(); i++) {
		string cword = word.substr(0, i + 1);
		string cletter = word.substr(i, 1); // The current words letter @ index i
		string aletter = answer.substr(i, 1); // The answers letter @ index i
		if (cletter == aletter) { hints.push_back(1); }
		else if (answer.find(cletter) > -1 &&
			count(answer, cletter) >= count(cword.substr(0, i + 1), cletter)) {
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
		int cnter = 5; // Start at 5th letter
		int i = 0;
		for (int i = 0; i < cword.length(); i++) {
			string cletter = cword.substr(i, 1);
			string aletter = word.substr(i, 1); // The actual word
			if (cletter == aletter) { // Green
				i += cnter * 5 * 3; // Third part is green
			}
			else if (word.find(cletter) > -1 &&
				count(word, cletter) >= count(cword.substr(0, i+1), cletter)) {
				i += cnter * 5 * 2; // Second part is yellow
			}
			// First part is red, no need to add to index
			cnter--;
		}
		matches[i]++;
	}
	double sum = 0;
	for (int p : matches) {
		double px = 0.0 * p / words.size();
		sum += px * -log2(px);
	}
	return sum;
}


ostream& operator<<(std::ostream& os, const Word& word) {
	os << word.word << " ";
	#ifndef DEBUG
	for (int i : word.hints) {
		os << i << " ";
	}
	#endif
	os << endl;
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
	for (int i = 0; i < words.size();i++) {
		for (int j = 0; j < words[i].length();j++) {
			char cletter = words[i][j];
			char aletter = word[j];
			if ((hints[j] == 1 && bits >= MINBITS &&
				!(cletter == aletter)) ||
				(hints[j] == 0 && word.find(cletter) > -1 &&
				count(word, cletter) >= count(words[i].substr(0, j + 1), cletter)) ||
				(hints[j] == -1 &&
				!(cletter != aletter)
				)) {
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