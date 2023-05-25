#pragma once

#include <string>
#include <vector>
#include <initializer_list>

int count(std::string, char);

class Word {
public:

	Word(std::string);
	Word();
	static void setAnswer(std::string);
	bool isCorrect() const;
	double expectedInfo(std::vector<std::string>) const;
	std::vector<std::string>& filterWords(std::vector<std::string>&, double);
	void updateHints();
	operator bool() const;
	friend std::ostream& operator<<(std::ostream&, const Word&);
private:
	static std::string answer;
	std::string word;
	std::vector<int> hints;
};