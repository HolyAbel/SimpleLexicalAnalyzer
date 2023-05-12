#pragma once
#include "globals.h"

struct LexTableItem {
	int id;
	string lexem;
	string type;
	string value;
};

class LexAnalysator {
	LexTableItem* table;
	int maxTableSize;
	int tableSize;
public:
	LexAnalysator();
	~LexAnalysator();

	void ResizeTable();

	void AnalyseFile(ifstream*, ofstream*);
};