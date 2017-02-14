#include "ConfigManager.h"

#include <fstream>
#include <string>
#include <vector>

#include <algorithm>
#include <cctype>

/*******************
* TOKEN FUNCTIONS *
*******************/

Token::Token() { }
Token::Token(const std::string &tokenID, const std::string tokenStringData) {
	type = STRING_VAL;

	id = tokenID;

	stringData = tokenStringData;
}

Token::Token(const std::string &tokenID, const double tokenDoubleData) {
	type = DOUBLE_VAL;

	id = tokenID;

	doubleData = tokenDoubleData;
}

Token::Token(const std::string &tokenID, const int tokenIntegerData) {
	type = INT_VAL;

	id = tokenID;

	integerData = tokenIntegerData;
}

std::string Token::getTokenID() {
	return id;
}

TOKENTYPE Token::getTokenType() {
	return type;
}

void Token::getTokenData(std::string &readData) {
	readData = stringData;
}
void Token::getTokenData(double &readData) {
	readData = doubleData;
}
void Token::getTokenData(int &readData) {
	readData = integerData;
}

/*********************
* SECTION FUNCTIONS *
* *******************/

CONFIGELEMENT Section::getConfigLineType(const std::string &line) {
	CONFIGELEMENT elementType = UNKNOWN;

	/* If string has only spaces */
	if (line.find_first_not_of(' ') == std::string::npos) {
		elementType = BLANK_LINE;

		/* if the line starts with `;' or #, then it is a comment */
	}
	else if (line[0] == ';' || line[0] == '#') {
		elementType = COMMENT;

		/* If the line starts with `[', then it is probably a section name */
	}
	else if (line[0] == '[') {

		size_t sectionEndIndex = -1;
		sectionEndIndex = line.find(']');

		if (sectionEndIndex != -1) {
			elementType = SECTION;
		}

		/* Or it can be a key or garbage */
	}
	else {
		size_t equalSignPos = -1;
		equalSignPos = line.find('=');

		if (equalSignPos != -1) {
			elementType = KEY;
		}
	}

	return elementType;

}


std::string Section::getKeyName(const std::string &line) {
	std::string keyName = "";

	size_t keyNameEndIndex = line.find('=') - 1;
	if (std::isspace(line[keyNameEndIndex])) {
		keyNameEndIndex -= 1;
	}

	/* (+1) because second param is len */
	keyName = line.substr(0, keyNameEndIndex + 1);

	return keyName;
}

static bool alpha_test(char c) {
	return std::isalpha(c);
}
static bool num_test(char c) {
	return std::isdigit(c);
}
TOKENTYPE Section::getKeyType(const std::string &line) {
	TOKENTYPE keyValueType = INVALID_TYPE;
	std::string keyValue;

	size_t equalSignPos = line.find('=');
	/* AAA = BBB */
	/*    ^ ^    */
	if (std::isspace(line[equalSignPos + 1])) {
		equalSignPos++;
	}

	keyValue = line.substr(equalSignPos + 1);

	/* Check if there are double quotes present */
	if ((keyValue.find('"') != std::string::npos) && (keyValue.rfind('"') != std::string::npos)) {
		keyValueType = STRING_VAL;
	}
	else {

		/* If there are no quotes -- continue checkings */
		bool containsNums
			= std::find_if(keyValue.begin(), keyValue.end(), num_test) != keyValue.end();
		bool containsDot = false;
		if (keyValue.find('.') != std::string::npos) {
			containsDot = true;
		}

		if (containsNums && containsDot) {
			keyValueType = DOUBLE_VAL;
		}
		else if (containsNums && !containsDot) {
			keyValueType = INT_VAL;
		}
	}

	return keyValueType;
}

std::string Section::getValueLine(const std::string &line) {
	std::string keyValue;

	size_t equalSignPos = line.find('=');
	/* AAA = BBB */
	/*    ^ ^    */
	if (std::isspace(line[equalSignPos + 1])) {
		equalSignPos++;
	}

	keyValue = line.substr(equalSignPos + 1);

	size_t firstQuote = keyValue.find('"');
	size_t lastQuote = keyValue.rfind('"');
	if ((firstQuote != std::string::npos) && (lastQuote != std::string::npos)) {
		keyValue = keyValue.substr(firstQuote + 1, (lastQuote - firstQuote - 1));
	}

	return keyValue;
}

void Section::getKeyValue(const std::string &line, std::string &readData) {
	std::string keyValue = getValueLine(line);

	readData = keyValue;
}
void Section::getKeyValue(const std::string &line, int &readData) {
	std::string keyValue = getValueLine(line);

	int val = std::stoi(keyValue);
	readData = val;

}
void Section::getKeyValue(const std::string &line, double &readData) {
	std::string keyValue = getValueLine(line);

	double val = std::atof(keyValue.c_str());
	readData = val;
}

void Section::addToken(const std::string &line) {
	std::string keyName = getKeyName(line);
	TOKENTYPE type = getKeyType(line);

	switch (type) {
	case INT_VAL:
	{
		int val;
		getKeyValue(line, val);

		sectionTokens.push_back(Token(keyName, val));
		break;
	}
	case STRING_VAL:
	{
		std::string val;
		getKeyValue(line, val);

		sectionTokens.push_back(Token(keyName, val));
		break;
	}
	case DOUBLE_VAL:
	{
		double val;
		getKeyValue(line, val);

		sectionTokens.push_back(Token(keyName, val));
		break;
	}
	}

}

std::string Section::getSectionName() {
	return sectionName;
}

void Section::readSection(const std::string& sectionName, std::stringstream& sstr) {
	this->sectionName = sectionName;

	std::string line;
	while (sstr.good()) {
		std::getline(sstr, line, '\n'); // Get the stringstream contents line-by-line

		CONFIGELEMENT lineType = getConfigLineType(line);
		if (lineType == KEY) {
			addToken(line);
		}
	}
}

bool Section::getToken(const std::string& tokenName, Token& readToken) {
	for (size_t i = 0; i < sectionTokens.size(); ++i) {
		if (sectionTokens[i].getTokenID() == tokenName) {
			readToken = sectionTokens[i];
			return true;
		}
	}

	return false;
}

/********************
* BasicDataSection *
* ******************/

std::string BasicDataSection::getSubstringBetweenBraces(std::string line) {
	std::string result;

	size_t outerCurlyBraceOpen = line.find_first_of('{');
	size_t outerCurlyBraceClose = line.find_last_of('}');
	size_t numberOfCharacters = outerCurlyBraceClose - outerCurlyBraceOpen - 1;

	//TODO: debug substr cutting
	result = line.substr(outerCurlyBraceOpen + 1, numberOfCharacters);

	return result;
}
std::vector<int> BasicDataSection::getNumbersInCurlyBraces(std::string line) {
	std::vector<int> result;

	line = getSubstringBetweenBraces(line);

	std::istringstream iss(line);
	std::string token;
	while (std::getline(iss, token, ',')) {
		// TODO: add checking for token validity
		int convertedToken = std::stoi(token);
		result.push_back(convertedToken);
	}

	return result;

}
bool BasicDataSection::init_neuronsInLayers(std::string line) {
	//neuronsInLayers = {1, 2, 3, 4}

	size_t outerCurlyBraceOpen = line.find('{');
	size_t outerCurlyBraceClose = line.find('}');

	if ((outerCurlyBraceOpen == std::string::npos)
		|| (outerCurlyBraceClose == std::string::npos)) {
		return false;
	}

	neuronsInLayers = getNumbersInCurlyBraces(line);

	return true;
}
bool BasicDataSection::init_mapDims(std::string line) {
	//mapDims = {{1, 2}, {4, 5}, {7, 8}}

	size_t outerCurlyBraceOpen = line.find_first_of('{');
	size_t outerCurlyBraceClose = line.find_last_of('}');

	if ((outerCurlyBraceOpen == std::string::npos)
		|| (outerCurlyBraceClose == std::string::npos)) {
		return false;
	}

	line = getSubstringBetweenBraces(line);
	std::istringstream iss(line);

	std::string token;
	size_t curlyBraceOpen = line.find_first_of('{');
	size_t curlyBraceClose = line.find_first_of('}');
	bool substringsPresent = true;
	while (substringsPresent) {
		size_t substrBracesLen = curlyBraceClose - curlyBraceOpen + 1;
		std::string substrInBraces = line.substr(curlyBraceOpen, substrBracesLen);
		line.erase(curlyBraceOpen, substrBracesLen);

		std::vector<int> vec = getNumbersInCurlyBraces(substrInBraces);
		int firstNum = *(vec.begin());
		int secNum = *(vec.rbegin());

		std::pair<int, int> myPair = std::make_pair(firstNum, secNum);
		mapDims.push_back(myPair);

		curlyBraceOpen = line.find_first_of('{');
		curlyBraceClose = line.find_first_of('}');

		if ((curlyBraceOpen == std::string::npos) ||
			(curlyBraceClose == std::string::npos)) {
			substringsPresent = false;
		}
	}

	return true;

}
void BasicDataSection::readBasicDataSection(std::stringstream& sstr) {
	std::string line;
	bool success = true;
	while (sstr.good()) {
		std::getline(sstr, line, '\n'); // Get the stringstream contents line-by-line

		if (line.find("neuronsInLayers") != std::string::npos) {
			bool success_read = init_neuronsInLayers(line);

			if (!success_read) {
				success = false;
			}
		}
		else if (line.find("mapDims") != std::string::npos) {
			bool success_read = init_mapDims(line);

			if (!success_read) {
				success = false;
			}
		}
	}

	if (success) {
		section_full_init = true;
	}
}

std::vector<int>& BasicDataSection::getNeuronsInLayers() {
	return neuronsInLayers;
}
std::vector<std::pair<int, int>>& BasicDataSection::getMapDimensions() {
	return mapDims;
}

/***************************
* ConfigManager funcitons *
* *************************/

bool ConfigManager::is_section(const std::string& line) {
	if (line.find(']') != std::string::npos && line.find('[') != std::string::npos) {
		return true;
	}
	return false;
}
std::string ConfigManager::getSectionName(const std::string &line) {
	std::string sectionName = "";

	size_t sectionEndIndex = line.find(']');
	sectionName = line.substr(1, (sectionEndIndex - 1));

	return sectionName;
}

void ConfigManager::readConfigFile(const std::string &fileName) {
	configFile.open(fileName, std::ios::in);

	/* Read the whole file and get section blocks */
	std::string line = ""; // Line of config file
	std::string sectionName; //Where to store the section name
	bool inside_section = false; //Are we inside the section?

	std::getline(configFile, line); // Read the first line of a config file

	while (configFile.good()) {
		std::stringstream sstr;

		if (is_section(line)) {
			inside_section = true;
			sectionName = getSectionName(line);

			/* Get the section block to the stringstream */
			while (inside_section && configFile.good()) {
				std::getline(configFile, line);

				if (!is_section(line)) {
					sstr << line << '\n'; // Append `\n' separator to the stringstream
				}
				else {
					inside_section = false;
				}
			}

			if (sectionName == "main") {
				basicSection.readBasicDataSection(sstr);

			}
			else {
				Section tmpSect;
				tmpSect.readSection(sectionName, sstr);
				configFileSections.push_back(tmpSect);
			}

		}
		else {
			/* If it is not a section and we are not inside a section,
			* read next lines. Probably there are some garbage lines here */
			std::getline(configFile, line);
		}
	}

}

size_t ConfigManager::getSectionIndex(std::string sectionName) {
	for (size_t i = 0; i < configFileSections.size(); ++i) {
		if (configFileSections[i].getSectionName() == sectionName) {
			return i;
		}
	}

	return -1;
}

bool ConfigManager::searchToken(const std::string &section, const std::string &tokenName, Token& readToken) {
	size_t findSectionIndex = getSectionIndex(section);

	/* Not found */
	if (findSectionIndex == -1) {
		return false;
	}

	Token foundToken;
	configFileSections[findSectionIndex].getToken(tokenName, foundToken);

	readToken = foundToken;
	return true;


}

void ConfigManager::getVal(const std::string &section, const std::string &id, std::string &readData) {
	Token foundToken;
	bool found = searchToken(section, id, foundToken);

	if (!found) {
		//exception here
	}
	else {
		foundToken.getTokenData(readData);
	}
}

void ConfigManager::getVal(const std::string &section, const std::string &id, double &readData) {
	Token foundToken;
	bool found = searchToken(section, id, foundToken);

	if (!found) {
		//exception here
	}
	else {
		foundToken.getTokenData(readData);
	}
}

void ConfigManager::getVal(const std::string &section, const std::string &id, int &readData) {
	Token foundToken;
	bool found = searchToken(section, id, foundToken);

	if (!found) {
		//exception here
	}
	else {
		foundToken.getTokenData(readData);
	}
}

bool ConfigManager::getBasicData(std::vector<int> &readNeuronsInLayers, std::vector<std::pair<int, int> > &readMapDims) {
	readNeuronsInLayers = basicSection.getNeuronsInLayers();
	readMapDims = basicSection.getMapDimensions();

	if (basicSection.section_full_init) {
		return true;
	}
	else {
		return false;
	}
}