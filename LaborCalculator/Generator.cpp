#include "Generator.h"
#include <fstream>
#include <sstream>


Generator::Generator(void)
{
//	RefreshGeneratorSettings();
}

void Generator::RefreshGeneratorSettings(void)
{
	std::ifstream in(m_GenSettingsLoc);
	if (in) {
		std::string token = "";
		std::string tempEvent = "";
		std::string eventVals = "";
		m_LEs.clear();

		do {
			token.clear();
			std::getline(in, token);
			//Remove whitespace
			while (token.find(" ") != std::string::npos) {
				token.erase(token.find(" "), 1);
			}
			//Remove "comments"
			if (token.find(";;") != std::string::npos) {
				token.erase(token.find(";;"));
			}
			if (!token.empty()) {
				//Was gonna include complex stuff for different settings types, but right now, just throwing downa quick even grabbers. - Marius
				tempEvent = token;

				token.clear();
				std::getline(in, token);
				//Remove whitespace
				while (token.find(" ") != std::string::npos) {
					token.erase(token.find(" "), 1);
				}
				//Remove "comments"
				if (token.find(";;") != std::string::npos) {
					token.erase(token.find(";;"));
				}
				eventVals = token;
				m_LEs.push_back(LaborEvent(tempEvent, SplitValues(eventVals)));
			}
		} while (!in.eof());
	}
	
}

std::string Generator::GenerateLabor(const unsigned inHours)
{
	RefreshGeneratorSettings(); // Not as efficient, but safe. 
	std::string output = "";
	for (unsigned i = 0; i < m_LEs.size(); i++) {
		output.append(m_LEs[i].GetEvent());
		output.append(" ");
		output.append(std::to_string(m_LEs[i].EstimatedLabor(inHours, m_Rand)));
		output.append("\r\n");
	}
	return output;
}

std::vector<unsigned> Generator::SplitValues(const std::string& str)
{
	std::stringstream stream(str);
	std::string temp = "";
	std::vector<unsigned> outVec;

	for (unsigned i = 0; i < 6; i++) {
		temp.clear();
		std::getline(stream, temp, ',');
		if (!temp.empty()) {
			outVec.push_back(std::stoi(temp));
		}
		else {
			outVec.push_back(0);
		}
	}
	return outVec;
}


Generator::LaborEvent::LaborEvent(const std::string& Event, const unsigned base, const unsigned baseRange, const unsigned hourlyIncrease, const unsigned triggerHour, const unsigned triggerBonusBase, const unsigned triggerBonusRange)
	:
	m_Event(Event),
	m_Base(base),
	m_BaseRange(baseRange),
	m_HourlyIncrease(hourlyIncrease),
	m_TriggerHour(triggerHour),
	m_BaseTriggerBonus(triggerBonusBase),
	m_BaseTriggerBonusRange(triggerBonusRange)
{
}

Generator::LaborEvent::LaborEvent(const std::string& Event, const std::vector<unsigned>& values)
	:
	m_Event(Event),
	m_Base(values[0]),
	m_BaseRange(values[1]),
	m_HourlyIncrease(values[2]),
	m_TriggerHour(values[3]),
	m_BaseTriggerBonus(values[4]),
	m_BaseTriggerBonusRange(values[5])
{
}

unsigned Generator::LaborEvent::EstimatedLabor(const unsigned hours, RandHandler& rand)
{
	unsigned labor;
	labor = rand.Generate(m_Base, m_BaseRange);
	for (unsigned i = 0; i < hours; i++) {
		labor += rand.Generate(0, m_HourlyIncrease);
	}
	if (hours >= m_TriggerHour) {
		labor += rand.Generate(m_BaseTriggerBonus, m_BaseTriggerBonusRange);
	}
	return labor;
}
