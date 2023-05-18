#include <iostream>;
#include <fstream>;
#include <string>;
#include <json/json.h>
using namespace std;

string guessWord(string& word) {
}


int main() {
	Json::Value words;
	ifstream f("words.json");
	f >> words;
	
	string answer = "flank";
	string currentw = "crane";
	for (int i = 0; i < 5; i++) {
		string guess = guessWord(currentw);
		cout << guess;
		if (guess == answer) {
			cout << " was the right answer" << endl;
			return 0;
		}
		cout << endl;
	}
	cout << "Did not get the right word" << endl;
	return 1;
}