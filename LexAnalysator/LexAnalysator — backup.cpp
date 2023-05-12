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
	while (getline(*iFile, line)) {
		while (line == "") {	//Отбрасываем пустые строки
			getline(*iFile, line);
		}



//Вставка пробелов между лексемами---------------------------------------------------------------------------------------------
		n = 0;
		do {	//Обрабатываем случай отсутствия пробелов рядом с константой
			pos1 = line.find("'", n);
			pos2 = line.find("'", pos1 + 1);

			//Заглушка:
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
			//Обрабатываем случай отсутствия пробелов рядом с оператором (кроме :=), комментарием
			line.insert(pos1, " ");
			line.insert(pos1 + 2, " ");
			n = pos1 + 3;
		}

		n = 0;
		do {	//Обрабатываем случай отсутствия пробелов рядом с оператором :=
			pos1 = line.find(":", n);
			pos2 = line.find("=", pos1 + 1);

			//Заглушка:
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


//Чтение строки по слову, пробел(ы) - разделитель
		istringstream iss(line);
		while (!iss.eof()) {
			word = "";	//Если пустое слово, то мы не будем ничего вставлять в таблицу и выводить;
						//также помогает справиться с созданием повторной записи в файле, когда
						//происходили лишние итерации при пустой строке (проверка !iss.eof() не срабатывала)
			iss >> word;



//COMMENT----------------------------------------------------------------------------------------------------------------------
			if (word[0] == '#') {	//Если это комментарий, пропуск строки
				break;
			}
//-----------------------------------------------------------------------------------------------------------------------------



//CHAR-------------------------------------------------------------------------------------------------------------------------
			else if (word[0] == '\'') {
				if (regex_match(word, CHAR)) {
					bool flag = true;
					int id = 0;
					int curTablesize = tableSize;
					for (int i = 0; i < curTablesize; i++) {			//Есть ли такая лексема?
						if (table[i].type == "Character constant") {	//Ищем последнюю лексему такого типа
							if (table[i].lexem == word) {				//Такая лексема уже была
								table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
								flag = false;
							}
							id = table[i].id;
						}
					}
					if (flag) {	//Такой лексемы еще не было
						table[tableSize] = LexTableItem{ ++id, word, "Character constant",
							string("C(").append(to_string(id)).append(")") };
					}
					tableSize++;
				}
			}
//-----------------------------------------------------------------------------------------------------------------------------



//IDENTIFIER-------------------------------------------------------------------------------------------------------------------
			else if (regex_match(word, IDENTIFIER)) {
				bool flag = true;
				int id = 0;
				int curTablesize = tableSize;
				for (int i = 0; i < curTablesize; i++) {	//Есть ли такая лексема?
					if (table[i].type == "Identifier") {	//Ищем последнюю лексему такого типа
						if (table[i].lexem == word) {		//Такая лексема уже была
							table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
							flag = false;
						}
						id = table[i].id;
					}
				}
				if (flag) {	//Такой лексемы еще не было
					if (word.size() <= 16) {
						table[tableSize] = LexTableItem{ ++id, word, "Identifier",
							string("I(").append(to_string(id)).append(")") };
					}
					else {
						table[tableSize] = LexTableItem{ ++id, word, "Incorrect token", "" };
					}
				}
				tableSize++;
			}
//-----------------------------------------------------------------------------------------------------------------------------
			else {
				string str;
				for (int i = 0; i < word.size(); i++) {
					str = word[i];



//OPERATOR:=-------------------------------------------------------------------------------------------------------------------
					if (str == ":") {
						str += word[++i];
						if (str == ":=") {
							bool flag = true;
							int id = 0;
							int curTablesize = tableSize;
							for (int i = 0; i < curTablesize; i++) {	//Есть ли такая лексема?
								if (table[i].type == "Operator") {		//Ищем последнюю лексему такого типа
									if (table[i].lexem == word) {		//Такая лексема уже была
										table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
										flag = false;
									}
									id = table[i].id;
								}
							}
							if (flag) {	//Такой лексемы еще не было
								table[tableSize] = LexTableItem{ ++id, word, "Operator",
									string("O(").append(to_string(id)).append(")") };
							}
							tableSize++;
						}
						else {
							str = word[i];
						}
					}
//-----------------------------------------------------------------------------------------------------------------------------



//OTHEROPERATORS---------------------------------------------------------------------------------------------------------------
					else if (regex_match(str, OPERATOR)) {
						bool flag = true;
						int id = 0;
						int curTablesize = tableSize;
						for (int i = 0; i < curTablesize; i++) {	//Есть ли такая лексема?
							if (table[i].type == "Operator") {		//Ищем последнюю лексему такого типа
								if (table[i].lexem == word) {		//Такая лексема уже была
									table[tableSize] = LexTableItem{ table[i].id, table[i].lexem, table[i].type, table[i].value };
									flag = false;
								}
								id = table[i].id;
							}
						}
						if (flag) {	//Такой лексемы еще не было
							table[tableSize] = LexTableItem{ ++id, word, "Operator",
								string("O(").append(to_string(id)).append(")") };
						}
						tableSize++;
					}
					else {
						word = "";
					}
				}
//-----------------------------------------------------------------------------------------------------------------------------
			}


//Запись в файл----------------------------------------------------------------------------------------------------------------
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
