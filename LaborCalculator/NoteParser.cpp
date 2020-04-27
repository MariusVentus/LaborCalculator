#include "NoteParser.h"

NoteParser::NoteParser(void)
{
//	Test();
}

void NoteParser::HoursAndMinutes(unsigned& hours, unsigned& minutes)
{
	hours = 0;
	while (minutes >= 60) {
		hours++;
		minutes -= 60;
	}
}

void NoteParser::Test(void)
{
	unsigned testHour = 0;
	unsigned testMin = 185;
	HoursAndMinutes(testHour, testMin);
}
