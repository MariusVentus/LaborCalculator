#pragma once
#include <string>
#include <vector>

class NoteParser {
public:
	NoteParser(void);
	std::string ParseNCalc(const std::string& inStr);
	std::string GetHours(void) const { return std::to_string(m_Hours); }
	std::string GetMinutes(void) const { return std::to_string(m_Minutes); }
	std::string GetIgnoreLoc(void) const { return m_IgnoreLoc; }
	void SetIgnoreOpened(bool val) { m_IgnoreOpened = val; }
	void RefreshIgnoreList(void);
	void Test(void);
private:
	//Variables and Constants
	const std::string m_IgnoreLoc = "Settings\\IgnoreList.txt";
	std::vector<std::string> m_IgnoreList;
	bool m_IgnoreOpened = false;
	unsigned m_Hours = 0;
	unsigned m_Minutes = 0;
	//Funcs
	void HoursAndMinutes(unsigned& hours, unsigned& minutes); //Resets hours, breaks up minutes into hours. 
	bool InvestigateNumbers(std::string line); // Checks a line for numbers, and adds to minutes. 
	void NormalizeLine(std::string& str);
	void ScrubLine(std::string& str);
};