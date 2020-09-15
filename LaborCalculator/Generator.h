#pragma once
#include <string>
#include <vector>
#include "RandHandler.h"

class Generator {
public:
	Generator(void);
	void RefreshGeneratorSettings(void);
	std::string GenerateLabor(const unsigned inHours);
	std::string GetGenSettingsLoc(void) const { return m_GenSettingsLoc; }

private:
	const std::string m_GenSettingsLoc = "Settings\\Generator.txt";
	class LaborEvent {
	public:
		LaborEvent(const std::string& Event, const unsigned base, const unsigned baseRange, const unsigned hourlyIncrease, const unsigned triggerHour, const unsigned triggerBonusBase, const unsigned triggerBonusRange);
		LaborEvent(const std::string& Event, const std::vector<unsigned>& values);
		std::string GetEvent(void) const { return m_Event; }
		unsigned EstimatedLabor(const unsigned hours, RandHandler& rand);
	private:
		const std::string m_Event;
		const unsigned m_Base;
		const unsigned m_BaseRange;
		const unsigned m_HourlyIncrease;
		const unsigned m_TriggerHour;
		const unsigned m_BaseTriggerBonus;
		const unsigned m_BaseTriggerBonusRange;
	};
	RandHandler m_Rand;
	std::vector<LaborEvent> m_LEs; 
	std::vector<unsigned> SplitValues(const std::string& str);
};