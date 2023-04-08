
#ifndef __FLINT_SCANNER_H
#define __FLINT_SCANNER_H

#include "flint.h"
#include "flint_code_info.h"
#include "flint_error_msg.h"

typedef enum {
    CHARS_TOKEN,
    NUMBER_TOKEN,
    STRING_TOKEN,
    OPERATOR_TOKEN,
    SYMBOL_TOKEN,
    ENDFILE_TOKEN,
} CharsToken;

typedef enum {
    PACKAGE_KEYWORD,
    IMPORT_KEYWORD,
    ENUM_KEYWORD,
    CLASS_KEYWORD,
    PRIVATE_KEYWORD,
    PUBLIC_KEYWORD,
    STATIC_KEYWORD,
    CONST_KEYWORD,
    VOID_KEYWORD,
    BYTE_KEYWORD,
    SHORT_KEYWORD,
    INT_KEYWORD,
    LONG_KEYWORD,
    FLOAT_KEYWORD,
    DOUBLE_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
    FOR_KEYWORD,
    DO_KEYWORD,
    WHILE_KEYWORD,
    SWITCH_KEYWORD,
    CASE_KEYWORD,
    DEFAULT_KEYWORD,
    CONTINUE_KEYWORD,
    BREAK_KEYWORD,
    TRY_KEYWORD,
    CATCH_KEYWORD,
    RETURN_KEYWORD,
    NONE_KEYWORD,
} KeywordToken;

class FlintScanner {
private:
    const char *codeStart;
    const char *codeCursor;
    int ln;
    int col;

    char currentChar(void);
    char nextChar(void);
    bool isSpace(char c);
    void skipSpace(void);
    bool verifyNumberToken(const char *code);
    CharsToken getToken(const char *code);
    int readChars(char *buff) throw(const char *);
    int readNumber(char *buff) throw(const char *);
    int readString(char *buff) throw(const char *);
    int readOperator(char *buff) throw(const char *);
    int readSymbol(char *buff) throw(const char *);
public:
    FlintScanner(const char *code);
    CharsToken read(ScanCodeInfo *codeInfo) throw(FlintError *);
    void setCursor(int cursor);
    void reset(void);
    static bool isNumber(char c);
    static KeywordToken isKeyword(ScanCodeInfo *codeInfo);

    ~FlintScanner();
};

#endif // __FLINT_SCANNER_H
