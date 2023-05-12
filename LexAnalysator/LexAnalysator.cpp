#include "LexAnalysator.h"

LexAnalysator::LexAnalysator() : tableSize{ 0 }, maxTableSize{ 50 } {
		table = new LexTableItem[maxTableSize];
}

LexAnalysator::~LexAnalysator() {
	delete[] table;
}

void LexAnalysator::ResizeTable() {
	LexTableItem* tTable = new LexTableItem[tableSize];
	for (int i = 0; i < tableSize; i++) {
		tTable[i] = table[i];
	}
	delete[] table;

	maxTableSize = tableSize + 50;
	table = new LexTableItem[maxTableSize];
	for (int i = 0; i < tableSize; i++) {
		table[i] = tTable[i];
	}
	
	delete[] tTable;
}

void LexAnalysator::AnalyseFile(ifstream* iFile, ofstream* oFile) {
	string line, word;
	size_t pos1, pos2;
	int n;
	while (getline(*iFile, line)) {	//������ ���� ���������
		while (line == "") {	//����������� ������ ������
			getline(*iFile, line);
		}



//������� �������� ����� ���������---------------------------------------------------------------------------------------------
		n = 0;
		do {	//������������ ������ ���������� �������� ����� � ����������
			pos1 = line.find("'", n);
			pos2 = line.find("'", pos1 + 1);

			//��������:
			if (pos1 > line.size()) {
				break;
			}

			if ((pos2 - pos1) == 2) {
				line.insert(pos2 + 1, " ");
				line.insert(pos1, " ");
				n = pos2 + 2;
			}
			else {
				n = pos1 + 1;
			}
		} while (pos2 != string::npos);

		n = 0;
		string opncom = "+-*/;()#";
		while ((pos1 = line.find_first_of(opncom, n)) != string::npos) {
			//������������ ������ ���������� �������� ����� � ���������� (����� :=), ������������
			line.insert(pos1, " ");
			line.insert(pos1 + 2, " ");
			n = pos1 + 3;
		}

		n = 0;
		do {	//������������ ������ ���������� �������� ����� � ���������� :=
			pos1 = line.find(":", n);
			pos2 = line.find("=", pos1 + 1);

			//��������:
			if (pos1 > line.size()) {
				break;
			}

			if ((pos2 - pos1) == 1) {
				line.insert(pos2 + 1, " ");
				line.insert(pos1, " ");
				n = pos2 + 2;
			}
			else {
				n = pos1 + 1;
			}
		} while (pos2 != string::npos);
//-----------------------------------------------------------------------------------------------------------------------------



//������ ������ �� �����, ������(�) - �����������------------------------------------------------------------------------------
		istringstream iss(line);
		while (!iss.eof()) {
			word = "";	//���� ������ �����, �� �� �� ����� ������ ��������� � ������� � ��������;
						//����� �������� ���������� � ��������� ��������� ������ � �����, �����
						//����������� ������ �������� ��� ������ ������ (�������� !iss.eof() �� �����������)
			iss >> word;
//-----------------------------------------------------------------------------------------------------------------------------



//COMMENT----------------------------------------------------------------------------------------------------------------------
			if (word[0] == '#') {	//���� ��� �����������, ������� ������
				break;
			}
//-----------------------------------------------------------------------------------------------------------------------------



//CHAR-------------------------------------------------------------------------------------------------------------------------
			else if (regex_match(word, CHAR)) {
				bool flag = true;
				int id = 0;
				int curTablesize = tableSize;
				for (int i = 0; i < curTablesize; i++) {			//���� �� ����� �������?
					if (table[i].type == "Character constant") {	//���� ��������� ������� ������ ����
						if (table[i].lexem == word) {				//����� ������� ��� ����
							table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
							flag = false;
						}
						id = table[i].id;
					}
				}
				if (flag) {	//����� ������� ��� �� ����
					table[tableSize] = LexTableItem{ ++id, word, "Character constant",
						string("C(").append(to_string(id)).append(")") };
				}
				tableSize++;
			}
//-----------------------------------------------------------------------------------------------------------------------------

			

//IDENTIFIER-------------------------------------------------------------------------------------------------------------------
			else if (regex_match(word, IDENTIFIER)) {
				bool flag = true;
				int id = 0;
				int curTablesize = tableSize;
				for (int i = 0; i < curTablesize; i++) {	//���� �� ����� �������?
					if (table[i].type == "Identifier") {	//���� ��������� ������� ������ ����
						if (table[i].lexem == word) {		//����� ������� ��� ����
							table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
							flag = false;
						}
						id = table[i].id;
					}
				}
				if (flag) {	//����� ������� ��� �� ����
					if (word.size() <= 16) {
						table[tableSize] = LexTableItem{ ++id, word, "Identifier",
							string("I(").append(to_string(id)).append(")") };
					}
				}
				tableSize++;
			}
//-----------------------------------------------------------------------------------------------------------------------------



//OPERATOR---------------------------------------------------------------------------------------------------------------------
			else if (regex_match(word, OPERATOR)) {
				bool flag = true;
				int id = 0;
				int curTablesize = tableSize;
				for (int i = 0; i < curTablesize; i++) {	//���� �� ����� �������?
					if (table[i].type == "Operator") {		//���� ��������� ������� ������ ����
						if (table[i].lexem == word) {		//����� ������� ��� ����
							table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
							flag = false;
						}
						id = table[i].id;
					}
				}
				if (flag) {	//����� ������� ��� �� ����
					table[tableSize] = LexTableItem{ ++id, word, "Operator",
						string("O(").append(to_string(id)).append(")") };
				}
				tableSize++;
			}
			else {
				table[tableSize] = LexTableItem{ NULL, word, "Incorrect token", "" };
				tableSize++;
			}
//-----------------------------------------------------------------------------------------------------------------------------



//������ � ����----------------------------------------------------------------------------------------------------------------
			if (word != "") {
				*oFile << setw(18) << table[tableSize - 1].lexem << " | " << 
					setw(18) << table[tableSize - 1].type << " | " << table[tableSize - 1].value << endl;
			}	
//-----------------------------------------------------------------------------------------------------------------------------
			
			

			if (tableSize == (maxTableSize - 1)) {
				ResizeTable();
			}
		}
	}
}
