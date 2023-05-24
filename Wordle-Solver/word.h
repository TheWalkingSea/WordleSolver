#pragma once

#include <string>
#include <vector>
#include <initializer_list>

class Word {
public:

	Word(std::string);
	static void setAnswer(std::string);
	bool isCorrect() const;
	double expectedInfo(std::vector<std::string>) const;
	void filterWords(std::vector<std::string>&);
	void updateHints();
	friend std::ostream& operator<<(std::ostream&, const Word&);
private:
	static std::string answer;
	std::string word;
	std::vector<int> hints;
};