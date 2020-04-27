#include "Document.h"
#include <iostream>
#include <regex>

#define NOT_IN_OPERATOR -1
#define OPERATOR_N 0
#define OPERATOR_P 1
#define OPERATOR_A 2
#define OPERATOR_I 3
#define OPERATOR_C 4
#define OPERATOR_C_ERASED 5
#define OPERATOR_D 6
#define OPERATOR_NEXT_TEXT 7
#define OPERATOR_BACK_TEXT 8
#define OPERATOR_SWAP 9
#define OPERATOR_J 10
#define OPERATOR_Q 11


/*
* Move to prefix line, print line number, print tab, print prefix line. line_pointer will now point to this line.
* @param operator_s Input containing the number of line and the operator, for example "3n".
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_n(std::string operator_s, const std::vector <std::string>& line_vector, int* line_pointer)
{
	std::string num_s = operator_s.substr(0, operator_s.length() - 1);
	int num = std::stoi(num_s);
	std::cout << num << "\t" << line_vector.at(num - 1) << '\n';
	*line_pointer = num;
}

/*
* Print all lines. line_pointer will now point to the last line.
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_p(const std::vector <std::string>& line_vector, int *line_pointer)
{
	for (auto it = begin (line_vector); it != end (line_vector); ++it) {
		std::cout <<  *it << '\n';
	}
	*line_pointer = line_vector.size();
}

/*
* Move to line Prefix. delete it. line_pointer will now point to this line.
* @param operator_s Input containing the number of line and the operator, for example "3d".
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_d(std::string operator_s, std::vector <std::string> &line_vector, int* line_pointer)
{
	if (line_vector.empty()) return;

	std::string num_s = operator_s.substr(0, operator_s.length() - 1);
	int num = std::stoi(num_s);
	line_vector.erase(line_vector.begin() + num - 1);
	*line_pointer = num - 1;
}

/*
* Search forward the first line containing sufix string. line_pointer will point to found line, otherwise will point to last line.
* @param operator_s Input starts with '/' followed by text, for example "/text".
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_next_text(std::string operator_s, const std::vector <std::string>& line_vector, int* line_pointer)
{
	int temp_counter = *line_pointer;
	std::string temp_s = operator_s.substr(1, operator_s.length() - 1);
	for (auto it = begin (line_vector) + *line_pointer - 1; it != end (line_vector); ++it) {
		if (line_vector.at(temp_counter - 1).find(temp_s) != std::string::npos) {
			std::cout << line_vector.at(temp_counter - 1) << '\n';
			*line_pointer = temp_counter;
			return;
		}
		else {
			temp_counter++;
		}
		*line_pointer = line_vector.size();
	}
}

/*
* Search backward the first line containing sufix string. line_pointer will point to found line, otherwise will point to first line.
* @param operator_s Input starts with '?' followed by text, for example "?text".
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_back_text(std::string operator_s, const std::vector <std::string>& line_vector, int* line_pointer)
{
	int temp_counter = *line_pointer;
	std::string temp_s = operator_s.substr(1, operator_s.length() - 1);
	for (auto it = begin (line_vector) + *line_pointer - 1; it != end (line_vector); --it) {
		if (line_vector.at(temp_counter - 1).find(temp_s) != std::string::npos) {
			std::cout << line_vector.at(temp_counter - 1) << '\n';
			*line_pointer = temp_counter;
			return;
		}
		else {
			temp_counter--;
		}
		*line_pointer = 1;
	}
}

/*
* Split string to vector of strings by char delimiter.
* @param str String to be split.
* @param ch Char delimiter.
*/
static std::vector<std::string> split_s_to_vector(const std::string& str, const char& ch) {
	std::string next;
	std::vector<std::string> result;

	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (*it == ch) {
			if (!next.empty()) {
				result.push_back(next);
				next.clear();
			}
		} else {
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}

/*
* Replace current line with a new line.
* @param operator_s Prefix and lines seperated by '/', for example "s/old/new".
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_swap(std::string operator_s, std::vector <std::string> &line_vector, int* line_pointer)
{
	std::vector<std::string> temp_vec = split_s_to_vector(operator_s, '/');
	std::string to_find_s = temp_vec.at(1);
	// std::cout << "to find s= " << to_find_s << '\n';
	std::size_t pos = line_vector.at(*line_pointer - 1).find(to_find_s);
	if (pos == std::string::npos) return;
	line_vector.at(*line_pointer - 1).replace(pos, temp_vec.at(1).length(), temp_vec.at(2));
}

/*
* Join second line to the first line.
* @param operator_s Operator formt: "3,4j". line 3 will equal "3+4" and line_pointer will be set to 3.
* @param line_vector Documents vector of lines.
* @param line_pointer Current line.
*/
static void do_operator_j(std::string operator_s, std::vector <std::string> &line_vector, int* line_pointer)
{
	std::vector<std::string> temp_vec1 = split_s_to_vector(operator_s, ',');
	std::vector<std::string> temp_vec2 = split_s_to_vector(temp_vec1.at(1), 'j');

	int line1 = std::stoi(temp_vec1.at(0));
	int line2 = std::stoi(temp_vec2.at(0));
	line_vector.at(line1 - 1) += line_vector.at(line2 - 1);
	line_vector.erase(line_vector.begin() + line2 - 1);
	*line_pointer = line1;
}

/*
* Quit editor.
*/
static void do_operator_Q(void)
{
	exit(0);
}

/*
* Return the operator found in input.
* @param operator_s Raw input.
*/
static int get_operator(std::string operator_s)
{
	if (!operator_s.compare("%p")) {
		return OPERATOR_P;
	} else if (!operator_s.compare("a")) {
		return OPERATOR_A;
	} else if (!operator_s.compare("i")) {
		return OPERATOR_I;
	} else if (!operator_s.compare("c")) {
		return OPERATOR_C;
	} else if (!operator_s.compare("Q")) {
		return OPERATOR_Q;
	} else if (std::regex_search(operator_s, std::regex(R"(^\d+n$)"))) {
		return OPERATOR_N;
	} else if (std::regex_search(operator_s, std::regex(R"(^\d+d$)"))) {
		return OPERATOR_D;
	} else if (std::regex_search(operator_s, std::regex(R"(^\/.*$)"))) {
		return OPERATOR_NEXT_TEXT;
	} else if (std::regex_search(operator_s, std::regex(R"(^\?.*$)"))) {
		return OPERATOR_BACK_TEXT;
	} else if (std::regex_search(operator_s, std::regex(R"(^s\/.*\/.*)"))) {
		return OPERATOR_SWAP;
	} else if (std::regex_search(operator_s, std::regex(R"(^\d+,\d+j$)"))) {
		return OPERATOR_J;
	}
	return -1;
}

/*
* Add new line acording to our operator state.
* @param line Input line to be processed.
*/
void Document::add_line(std::string line)
{

	switch (state) {
	case NOT_IN_OPERATOR: {
		line_vector.push_back(line);
		line_pointer++;
	}
	break;
	case OPERATOR_A: {
		if (!line.compare(".")) {
			state = NOT_IN_OPERATOR;
			return;
		}
		if (line_vector.empty()) {
			line_vector.push_back(line);
			line_pointer++;
			return;
		}
		line_vector.insert(line_vector.begin() + line_pointer, line);
		line_pointer++;
	}
	break;
	case OPERATOR_I: {
		if (!line.compare(".")) {
			state = NOT_IN_OPERATOR;
			return;
		}
		if (line_pointer == 1) {
			line_vector.insert(line_vector.begin(), line);
			line_pointer++;
			return;
		}
		line_vector.insert(line_vector.begin() + line_pointer - 1, line);
		line_pointer++;
	}
	break;
	case OPERATOR_C: {
		if (line_vector.empty()) {
			line_vector.push_back(line);
			line_pointer++;
			state = NOT_IN_OPERATOR;
			return;
		}
		state = OPERATOR_C_ERASED;
		line_vector.erase(line_vector.begin() + line_pointer - 1);
		line_vector.insert(line_vector.begin() + line_pointer - 1, line);
	}
	break;
	case OPERATOR_C_ERASED: {
		if (!line.compare(".")) {
			state = NOT_IN_OPERATOR;
			return;
		}
		line_vector.insert(line_vector.begin() + line_pointer, line);
		line_pointer++;
	}
	default: {

	}
	break;
	}

}

/*
* Execute operators or change operator states.
* @param operator_s Raw input.
*/
void Document::operator_handler(std::string operator_s)
{
	int operator_temp = get_operator(operator_s);

	switch (operator_temp) {
	case OPERATOR_N: {
		do_operator_n(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_P: {
		do_operator_p(line_vector, &line_pointer);
	}
	break;
	case OPERATOR_A: {
		state = OPERATOR_A;
	}
	break;
	case OPERATOR_I: {
		state = OPERATOR_I;
	}
	break;
	case OPERATOR_C: {
		state = OPERATOR_C;
	}
	break;
	case OPERATOR_D: {
		do_operator_d(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_NEXT_TEXT: {
		do_operator_next_text(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_BACK_TEXT: {
		do_operator_back_text(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_SWAP: {
		do_operator_swap(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_J: {
		do_operator_j(operator_s, line_vector, &line_pointer);
	}
	break;
	case OPERATOR_Q: {
		do_operator_Q();
	}
	break;
	default: {

	}
	break;
	}
}

/*
* Pass input to be processed as a new operator or a new line.
* @param operator_s Raw input.
*/
void Document::input(std::string input) {
	if (state == NOT_IN_OPERATOR) {

		operator_handler(input);
		return;
	}
	add_line(input);
}