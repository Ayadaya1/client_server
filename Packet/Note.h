#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include "../Packet/Packet.h"
#include <locale.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include "../Packet/Note.h"
#include <ios>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
class Date {
public:
	uint16_t year;
	uint8_t month;
	uint8_t day;


	Date()
	{
		year = 0;
		month = 0;
		day = 0;
	}
	Date(uint16_t y, uint8_t m, uint8_t d)
	{
		year = y;
		month = m;
		day = d;
	}
	char* toString(bool numeric = false)
	{
		char res[30];
		char d[15];
		_itoa((int)day, d, 10);
		d[14] = '\n';
		char y[15];
		_itoa((int)year, y, 10);
		y[14] = '\n';
		char m[10];
		if (!numeric)
		{
			switch (month) {
			case 1:
				strcpy(m, "€нвар€");
				break;
			case 2:
				strcpy(m, "феврал€");
				break;
			case 3:
				strcpy(m, "марта");
				break;
			case 4:
				strcpy(m, "апрел€");
				break;
			case 5:
				strcpy(m, "ма€");
				break;
			case 6:
				strcpy(m, "июн€");
				break;
			case 7:
				strcpy(m, "июл€");
				break;
			case 8:
				strcpy(m, "августа");
				break;
			case 9:
				strcpy(m, "сент€бр€");
				break;
			case 10:
				strcpy(m, "окт€бр€");
				break;
			case 11:
				strcpy(m, "но€бр€");
				break;
			case 12:
				strcpy(m, "декабр€");
				break;
			default:
				strcpy(m, "ошибка");
				break;
			}
			strcpy(res, d);
			strcat(res, " ");
			strcat(res, m);
			strcat(res, " ");
			strcat(res, y);
			strcat(res, "года");
			strcat(res, "\n");
		}
		else
		{
			_itoa(month, m, 10);
			strcpy(res, d);
			strcat(res, ".");
			strcat(res, m);
			strcat(res, ".");
			strcat(res, y);
			strcat(res, "\n");
		}
		return res;
	}
	bool write(std::ostream& os) {
		os.write((char*)this, sizeof(Date));
		return os.good();
	}
	bool operator == (const Date& date)
	{
		return year == date.year && month == date.month && day == date.day;
	}
};

class Note 
{
public:
	Date date;
	char note[240] = {0};

	Note()
	{
		date = Date();
	}
	Note(uint16_t y, uint8_t m, uint8_t d, char p_note[240])
	{
		date = Date(y, m, d);
		strcpy(note, p_note);
	}
	Note(std::stringstream& s)
	{
		char word[30] = { 0 };
		uint16_t y;
		uint8_t m;
		uint8_t d;
		s >> word;
		y = atoi(word);
		s >> word;
		m = atoi(word);
		s >> word;
		d = atoi(word);
		char ss[240] = "";
		while (s >> word) {
			strcat(ss, word);
			strcat(ss, " ");
		};
		date = Date(y, m, d);
		strcpy(note, ss);

	}

	bool write(std::ostream& os) {
		os.write((char*)this, sizeof(Note));
		return os.good();
	}
	bool read(std::istream& is) {
		is.read((char*)this, sizeof(Note));
		return is.good();
	}
	bool operator==(const Note& p_note)
	{
		return(date == p_note.date&&strcmp(p_note.note,note) == 0);
	}
	
};

class Log {
public:
	time_t time;
	char call[30];

	Log(char p_call[30])
	{
		strcpy(call, p_call);
		time = std::time(0);
	}
	Log()
	{
		strcpy(call, "");
		time = std::time(0);
	}
	char* getTime()
	{
			struct tm* timeinfo;

			std::time(&time);
			timeinfo = localtime(&time);
			char* time = asctime(timeinfo);

			char* newLine = strstr(time, "\n");
			if (newLine)
			{
				*newLine = 0;
			}

			return time;
	}
	bool write(std::ostream& os) {
		os.write((char*)this, sizeof(Log));
		return os.good();
	}
};

