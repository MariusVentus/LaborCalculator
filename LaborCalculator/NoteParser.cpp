#include "NoteParser.h"
#include <sstream>
#include <fstream>

NoteParser::NoteParser(void)
{
	RefreshIgnoreList();
	//Test();
}

std::string NoteParser::ParseNCalc(const std::string& inStr)
{
	//Initialize and Reset
	std::stringstream inStream(inStr);
	std::string token = "";
	std::string outStr = "";
	m_Hours = 0;
	m_Minutes = 0;
	//Check Ignore List

	RefreshIgnoreList(); // Not as efficient, but safe.

	//Parse Input
	do {
		token.clear();
		std::getline(inStream, token);
		if (InvestigateNumbers(token)) {
			ScrubLine(token);
			outStr.append(token);
			outStr.append("\r\n");
		}
	} while (!inStream.eof());
	
	//Minutes to Minutes and Hours
	HoursAndMinutes(m_Hours, m_Minutes);

	return outStr;
}

void NoteParser::RefreshIgnoreList(void)
{
	m_IgnoreList.clear();
	std::ifstream in(m_IgnoreLoc);
	std::string str = "";
	if (in) {
		do {
			str.clear();
			std::getline(in, str);
			NormalizeLine(str);
			//Add to local ignorelist
			if (!str.empty()) {
				m_IgnoreList.push_back(str);
			}
		} while (!in.eof() && !str.empty());
	}
	else {
		std::ofstream out(m_IgnoreLoc);
	}

}

void NoteParser::Test(void)
{
	unsigned testHour = 0;
	unsigned testMin = 185;
	HoursAndMinutes(testHour, testMin);
	auto str = GetHours();
	str = GetMinutes();
}

void NoteParser::HoursAndMinutes(unsigned& hours, unsigned& minutes)
{
	hours = 0;
	while (minutes >= 60) {
		hours++;
		minutes -= 60;
	}
}

bool NoteParser::InvestigateNumbers(std::string line)
{
	NormalizeLine(line);
	//Remove Ignored items.
	for (unsigned i = 0; i < m_IgnoreList.size(); i++) {
		if (line.find(m_IgnoreList[i]) != std::string::npos) {
			line.erase(line.find(m_IgnoreList[i]), m_IgnoreList[i].size());
		}
	}
	//All Non-numbers to whitespace
	for (unsigned i = 0; i < line.size(); i++) {
		if (line[i] < 48 || line[i] > 57) {
			line[i] = 32;
		}
	}
	NormalizeLine(line);
	if (line.find(" ") != std::string::npos && !line.empty()) {
		line.erase(0, line.find_last_of(" "));
		if (std::stoul(line) != 0) {
			m_Minutes += std::stoul(line);
			return true;
		}
		else {
			return false;
		}
	}
	else if (!line.empty()) {
		if (std::stoul(line) != 0) {
			m_Minutes += std::stoul(line);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void NoteParser::NormalizeLine(std::string& str)
{
	//Lower all Letters
	for (unsigned i = 0; i < str.size(); i++) {
		if (str[i] >= 65 && str[i] <= 90) {
			str[i] = str[i] + 32; //Make into lowercase
		}
	}
	//All non-numbers/non-letters turn to Whitespace
	for (unsigned i = 0; i < str.size(); i++) {
		if ((str[i] < 48 || str[i] > 57) && (str[i] < 97 || str[i]>122)) {
			str[i] = 32;
		}
	}
	//Remove Double whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}
	//Remove Leading Whitespace
	while (str.find(" ") == 0) {
		str.erase(str.find(" "), 1);
	}
	//Remove Trailing Whitespace
	if (str.find_last_of(" ") == str.size() - 1 && !str.empty()) {
		str.pop_back();
	}
}

void NoteParser::ScrubLine(std::string& str)
{
	//All non-numbers/non-letters turn to Whitespace
	for (unsigned i = 0; i < str.size(); i++) {
		if ((str[i] < 48 || str[i] > 57) && (str[i] < 65 || str[i] > 90) && (str[i] < 97 || str[i] > 122)) {
			str[i] = 32;
		}
	}
	if (!str.empty()) {
		//Remove Double whitespace
		while (str.find("  ") != std::string::npos) {
			str.erase(str.find("  "), 1);
		}
		//Remove Leading Whitespace
		while (str.find(" ") == 0) {
			str.erase(str.find(" "), 1);
		}
		//Remove Trailing Whitespace
		if (str.find_last_of(" ") == str.size() - 1 && !str.empty()) {
			str.pop_back();
		}
		//SpaceNumbers
		for (unsigned i = 0; i < str.size(); i++) {
			if (str[i] >= 48 && str[i] <= 57) {
				if (i != str.size() - 1) {
					if (!(str[i + 1] >= 48 && str[i + 1] <= 57) && str[i + 1] != 32) {
						str.insert(i + 1, " ");
					}
				}
				if (i > 0) {
					if (!(str[i - 1] >= 48 && str[i - 1] <= 57) && str[i - 1] != 32) {
						str.insert(i, " ");
					}
				}
			}
		}
	}
}


