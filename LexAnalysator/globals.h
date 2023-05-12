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

//Не длиннее 16 символов! На стадии добавления.
const regex IDENTIFIER("[a-zA-Z]|[a-zA-Z][a-zA-Z0-9]{1,15}");

const regex OPERATOR("\\+|-|\\*|/|;|\\(|\\)|:=");