#include "RandHandler.h"
#include <chrono>

RandHandler::RandHandler()
	:
	m_DeviceSeed(rd()),
	m_TimeSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
	rng(m_DeviceSeed)
{
	if (rd.entropy() == 0.0) {
		rng.seed((unsigned)m_TimeSeed);
	}
}

unsigned RandHandler::Generate(unsigned base, unsigned range)
{
	if (range > 0) {
		std::uniform_int_distribution<unsigned> Dist(0, range);
		return base + Dist(rng);
	}
	else {
		return base;
	}
}
