#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <exception>
#include <iomanip>

using namespace std;

//const regex COMMENT("#");

const regex CHAR("'.'");

//�� ������� 16 ��������! �� ������ ����������.
const regex IDENTIFIER("[a-zA-Z]|[a-zA-Z][a-zA-Z0-9]{1,15}");

const regex OPERATOR("\\+|-|\\*|/|;|\\(|\\)|:=");