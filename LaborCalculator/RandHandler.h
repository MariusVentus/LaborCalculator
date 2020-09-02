#pragma once
#include <random>

class RandHandler {
public:
	RandHandler();
	unsigned Generate(unsigned base, unsigned range);

	//Getters
	double PollEntropy(void) const { return rd.entropy(); }
	unsigned PollDeviceSeed(void) const { return m_DeviceSeed; }
	unsigned long long PollTimeSeed(void) const { return m_TimeSeed; }

private:
	//Vars
	unsigned int m_DeviceSeed;
	unsigned long long m_TimeSeed;
	std::random_device rd;
	std::mt19937 rng;
};