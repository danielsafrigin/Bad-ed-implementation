#ifndef EX_1_EDITOR
#define EX_1_EDITOR

#include "Document.h"

class Editor {
	Document* document;
public:
	void loop(void);
	Editor(void) {
		document = new Document();
	};
	~Editor(void) {
		//delete document;
	};

};



#endif