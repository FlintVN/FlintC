
#ifndef __FLINT_PARSER_H
#define __FLINT_PARSER_H

#include "flint_scanner.h"
#include "flint_error_msg.h"

typedef enum {
    PRIVATE_ACCESS_MODIFIER,
    PUBLIC_ACCESS_MODIFIER,
} AccessModifier;

class FlintParser {
private:
    AccessModifier accessModifier;
    FlintScanner *scanner;
    ScanCodeInfo **importList;
    ScanCodeInfo *className;

    void startParser(void) throw(FlintError *);
    void parserImportList(void) throw(FlintError *);
    void parserClass(void) throw(FlintError *);
public:
    FlintParser(const char *code) throw(FlintError *);

    ScanCodeInfo **getImportList(void);
    ScanCodeInfo *getClassName(void);

    ~FlintParser();
};

#endif // __FLINT_PARSER_H
