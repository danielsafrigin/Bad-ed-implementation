#ifndef EX_1_DOCUMENT
#define EX_1_DOCUMENT

#include <string>
#include <vector>


class Document {
private:
	int line_pointer;
	std::vector <std::string> line_vector;
	int state;

public:
	// Line counting starts from 1
	Document(void) : line_pointer(0), line_vector(), state(-1) {};
	~Document(void) {};
	void operator_handler(const std::string&);
	void add_line(const std::string&);
	void input(const std::string&);
};

#endif
