#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include "Word.h"


using namespace std;
Json::Value words;


void guessWord(Word* word) {
	for (int i = 0;i<words.size();i++) {
		Word w(words[i].asString());
	}
}


int main() {
	ifstream f("words.json");
	f >> words;
	
	Word::setAnswer("flank");
	Word currentw("crane");

	for (int i = 0; i < 5; i++) {
		guessWord(&currentw);
		cout << currentw;
		if ( currentw.isCorrect() ) {
			cout << " was the right answer" << endl;
			return 0;
		}
		cout << endl;
	}
	cout << "Did not get the right word" << endl;
	return 0;
}