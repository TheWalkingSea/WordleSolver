#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include "Word.h"


using namespace std;
vector<string> words;
Word getBestWord(vector<string> words, double& info) {
	if (words.size() == 1) { return Word(words[0]); }
	double maxInfo = 0;
	Word maxWord;
	for (string w : words) {
		Word word(w);
		double einfo = word.expectedInfo(words);
		if (einfo > maxInfo) {
			maxInfo = einfo;
			maxWord = word;
		}
	}
	info += maxInfo;
	return maxWord;
}

vector<string> getWords() {
	Json::Value w;
	ifstream f("words.json");
	f >> w;
	vector<string> words;
	words.reserve(w.size());
	for (const Json::Value value : w) words.push_back(value.asString());
	f.close();
	return words;
}

int main() {

	vector<string> words = getWords();
	double totalInfo = 0.0; // Total number of bits of information

	Word::setAnswer("slurs");
	Word currentw;

	for (int i = 0; i < 5; i++) {
		if (currentw) { currentw = getBestWord(words, totalInfo); }
		else { currentw = Word("crane"); }
		cout << currentw;
		if ( currentw.isCorrect() ) {
			cout << " was the right answer";
			return 0;
		}
		cout << endl;
		currentw.updateHints();
		currentw.filterWords(words, totalInfo);
	}
	cout << "Did not get the right word" << endl;
	return 0;
}