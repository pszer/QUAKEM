#include "Cmd.hpp"

std::map<std::string, COM_FUNC> COMMANDS;
std::map<std::string, Argument> CVARS;

Argument * GetCVar(const std::string& id) {
	auto f = CVARS.find(id);
	if (f == CVARS.end()) return nullptr;
	else return &(f->second);
}

long long GetCVarInt(const std::string& id) {
	auto a = GetCVar(id);
	if (a == nullptr) return 0;
	else return a->ToInt();
}

double GetCVarFloat(const std::string& id) {
	auto a = GetCVar(id);
	if (a == nullptr) return 0.0;
	else return a->ToFloat();
}

std::string GetCVarString(const std::string& id) {
	auto a = GetCVar(id);
	if (a == nullptr) return "";
	else return a->ToString();
}

long long INumber::ToInt() {
	if (type == NUM_INT)
		return __int__;
	return static_cast<long long>(__double__);
}

double INumber::ToFloat() {
	if (type == NUM_INT)
		return static_cast<double>(__int__);
	return __double__;
}

Argument::Argument(ARG_TYPE _type, const std::string& str) {
	if (_type == ARG_STRING) {
		type = ARG_STRING;
	} else type = ARG_CVAR;

	// copies str into string pointer
	std::size_t length = str.length();
	string = new char[length + 1];
	string[length] = '\0';
	for (char * c = string; c != string + length; ++c) {
		*c = str[c - string];
	}
}

Argument::~Argument() {
	if (type != ARG_NUMBER) {
		delete string;
	}
}

long long Argument::ToInt() {
	if (type == ARG_NUMBER) return num.ToInt();
	else if (type == ARG_CVAR) return GetCVarInt(std::string(cvar));
	else return 0.0;
}

double Argument::ToFloat() {
	if (type == ARG_NUMBER) return num.ToFloat();
	else if (type == ARG_CVAR) return GetCVarFloat(std::string(cvar));
	else return 0.0;
}

std::string Argument::ToString() {
	if (type == ARG_STRING) return std::string(string);
	else if (type == ARG_CVAR) return GetCVarString(std::string(cvar)); 
	else if (num.type == NUM_INT)
		return std::to_string(num.__int__);
	else
		return std::to_string(num.__double__);
}

COM_FUNC GetCommand(const std::string& id) {
	auto f = COMMANDS.find(id);
	if (f == COMMANDS.end()) return nullptr; // command not found
	else return f->second;
} 

std::string ExecuteCommand(const struct Command& com) {
	auto func = GetCommand(com.command);
	if (func == nullptr) return "ERR";	
	return func(com.args);
}
