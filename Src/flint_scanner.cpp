
#include "flint_scanner.h"
#include <string.h>

#define KEYWORD_VALUE(k, t)    {.keyword = k, .length = sizeof(k) - 1, .token = t}

typedef struct {
    const char *keyword;
    unsigned char length;
    KeywordToken token;
} KeywordValue;

static const KeywordValue keywordList[] = {
    KEYWORD_VALUE("package", PACKAGE_KEYWORD),
    KEYWORD_VALUE("import", IMPORT_KEYWORD),
    KEYWORD_VALUE("enum", ENUM_KEYWORD),
    KEYWORD_VALUE("class", CLASS_KEYWORD),
    KEYWORD_VALUE("private", PRIVATE_KEYWORD),
    KEYWORD_VALUE("public", PUBLIC_KEYWORD),
    KEYWORD_VALUE("static", STATIC_KEYWORD),
    KEYWORD_VALUE("const", CONST_KEYWORD),
    KEYWORD_VALUE("void", VOID_KEYWORD),
    KEYWORD_VALUE("byte", BYTE_KEYWORD),
    KEYWORD_VALUE("short", SHORT_KEYWORD),
    KEYWORD_VALUE("int", INT_KEYWORD),
    KEYWORD_VALUE("long", LONG_KEYWORD),
    KEYWORD_VALUE("float", FLOAT_KEYWORD),
    KEYWORD_VALUE("double", DOUBLE_KEYWORD),
    KEYWORD_VALUE("if", IF_KEYWORD),
    KEYWORD_VALUE("else", ELSE_KEYWORD),
    KEYWORD_VALUE("for", FOR_KEYWORD),
    KEYWORD_VALUE("do", DO_KEYWORD),
    KEYWORD_VALUE("while", WHILE_KEYWORD),
    KEYWORD_VALUE("switch", SWITCH_KEYWORD),
    KEYWORD_VALUE("case", CASE_KEYWORD),
    KEYWORD_VALUE("default", DEFAULT_KEYWORD),
    KEYWORD_VALUE("try", TRY_KEYWORD),
    KEYWORD_VALUE("catch", CATCH_KEYWORD),
    KEYWORD_VALUE("continue", CONTINUE_KEYWORD),
    KEYWORD_VALUE("break", BREAK_KEYWORD),
    KEYWORD_VALUE("return", RETURN_KEYWORD),
};

FlintScanner::FlintScanner(const char *code) {
    codeStart = code;
    reset();
}

char FlintScanner::currentChar(void) {
    return *codeCursor;
}

char FlintScanner::nextChar(void) {
    if(*codeCursor == '\n') {
        ln++;
        col = 0;
    }
    else if(*codeCursor != '\r')
        col++;
    codeCursor++;
    return *codeCursor;
}

bool FlintScanner::isSpace(char c) {
    return (c == ' ' || c == '\r' || c == '\n' || c == '\t') ? true : false;
}

void FlintScanner::skipSpace(void) {
    char c = currentChar();
    while(isSpace(c))
        c = nextChar();
}

bool FlintScanner::verifyNumberToken(const char *code) {
    int index = 0;
    bool isHex = false;
    if(code[0] == '0' && (code[1] == 'x' || code[1] == 'X')) {
        isHex = true;
        index = 2;
    }
    char c = code[index];
    while(isNumber(c) || (isHex && (('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'))))
        c = code[index++];
    if(c == '_' || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        return false;
    else if(index == 2 && (code[1] == 'x' || code[1] == 'X'))
        return false;
    return true;
}

CharsToken FlintScanner::getToken(const char *code) {
    char c = *code;
    if(c == '_' || isNumber(c) || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        return CHARS_TOKEN;
    // else if(isNumber(c))
    //     return verifyNumberToken(code) ? NUMBER_TOKEN : CHARS_TOKEN;
    else if(c == '"')
        return STRING_TOKEN;
    else if(c == '=')
        return OPERATOR_TOKEN;
    else if(c == '~' || c == '!')
        return OPERATOR_TOKEN;
    else if(c == '+' || c == '-')
        return OPERATOR_TOKEN;
    else if(c == '*' || c == '/')
        return OPERATOR_TOKEN;
    else if(c == '<' || c == '>')
        return OPERATOR_TOKEN;
    else if(c == '&' || c == '|' || c == '^')
        return OPERATOR_TOKEN;
    else if(c == 0)
        return ENDFILE_TOKEN;
    else
        return SYMBOL_TOKEN;
}

int FlintScanner::readChars(char *buff) throw(const char *) {
    int count = 0;
    char c = currentChar();
    while(c == '_' || isNumber(c) || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
        if(count < KEYWORD_BUFF_SIZE)
            buff[count++] = c;
        c = nextChar();
    }
    if(count >= KEYWORD_BUFF_SIZE)
        throw "keyword is too long";
    buff[count] = 0;
    return count;
}

int FlintScanner::readNumber(char *buff) throw(const char *) {
    int count = 0;
    if(count < KEYWORD_BUFF_SIZE)
        buff[count++] = currentChar();
    char c = nextChar();
    bool isHex = false;
    if(buff[0] == '0' && (c == 'x' || c == 'X')) {
        isHex = true;
        if(count < KEYWORD_BUFF_SIZE)
            buff[count++] = c;
        c = nextChar();
    }
    while(isNumber(c) || (isHex && (('A' <= c && c <= 'F') || ('a' <= c && c <= 'f')))) {
        if(count < KEYWORD_BUFF_SIZE)
            buff[count++] = c;
        c = nextChar();
    }
    if(count >= KEYWORD_BUFF_SIZE)
        throw "keyword is too long";
    buff[count] = 0;
    return count;
}

int FlintScanner::readString(char *buff) throw(const char *) {
    int count = 0;
    if(count < KEYWORD_BUFF_SIZE)
        buff[count++] = currentChar();
    char c = nextChar();
    while(c) {
        if(c == '\\') {
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            c = nextChar();
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            c = nextChar();
        }
        else {
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            if(c != '"')
                c = nextChar();
            else
                break;
        }
    }
    if(count >= KEYWORD_BUFF_SIZE)
        throw "keyword is too long";
    buff[count] = 0;
    return count;
}

int FlintScanner::readOperator(char *buff) throw(const char *) {
    int count = 0;
    char c = currentChar();
    if(count < KEYWORD_BUFF_SIZE)
        buff[count++] = c;
    nextChar();
    switch(c) {
    case '~':
        return 1;
    case '!':
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '=':
    case '<':
    case '>':
        c = currentChar();
        if(c == '=') {
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            nextChar();
        }
        break;
    case '&':
        c = currentChar();
        if(c == '=' || c == '&') {
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            nextChar();
        }
        break;
    case '|':
        c = currentChar();
        if(c == '=' || c == '|') {
            if(count < KEYWORD_BUFF_SIZE)
                buff[count++] = c;
            nextChar();
        }
        break;
    default:
        return 0;
    }
    if(count >= KEYWORD_BUFF_SIZE)
        throw "keyword is too long";
    buff[count] = 0;
    return count;
}

int FlintScanner::readSymbol(char *buff) throw(const char *) {
    buff[0] = currentChar();
    buff[1] = 0;
    nextChar();
    if(2 >= KEYWORD_BUFF_SIZE)
        throw "keyword is too long";
    return 1;
}

CharsToken FlintScanner::read(ScanCodeInfo *codeInfo) throw(FlintError *) {
    int length;
    char *buff = new char[KEYWORD_BUFF_SIZE];
    unsigned char count = 0;
    CharsToken token;
    skipSpace();
    int startCol = col;
    int cursor = (int)(codeCursor - codeStart);
    try {
        switch(getToken(codeCursor)) {
        case CHARS_TOKEN:
            length = readChars(buff);
            token = CHARS_TOKEN;
            break;
        case NUMBER_TOKEN:
            length = readNumber(buff);
            token = NUMBER_TOKEN;
            break;
        case STRING_TOKEN:
            length = readString(buff);
            token = STRING_TOKEN;
            break;
        case OPERATOR_TOKEN:
            length = readOperator(buff);
            token = OPERATOR_TOKEN;
            break;
        case SYMBOL_TOKEN:
            length = readSymbol(buff);
            token = SYMBOL_TOKEN;
            break;
        case ENDFILE_TOKEN:
        default:
            length = 0;
            token = ENDFILE_TOKEN;
            break;
        }
    }
    catch(const char *msg) {
        delete buff;
        throw new FlintError(ln, startCol, col, msg);
    }

    if(length) {
        codeInfo->setInfo(cursor, ln, startCol, col);
        codeInfo->setCode(buff, length);
    }
    delete buff;
    return token;
}

void FlintScanner::setCursor(int cursor) {
    codeCursor = codeStart;
    ln = 0;
    col = 0;
    while(cursor--)
        nextChar();
}

void FlintScanner::reset(void) {
    codeCursor = codeStart;
    ln = 0;
    col = 0;
}

bool FlintScanner::isNumber(char c) {
    return ('0' <= c && c <= '9') ? true : false;
}

KeywordToken FlintScanner::isKeyword(ScanCodeInfo *codeInfo) {
    int keyCount = sizeof(keywordList) / sizeof(keywordList[0]);
    int length = codeInfo->getLength();
    const char *code = codeInfo->getCode();
    for(int k = 0; k < keyCount; k++) {
        if((length == keywordList[k].length) && (strcmp(code, keywordList[k].keyword) == 0))
            return keywordList[k].token;
    }
    return NONE_KEYWORD;
}

FlintScanner::~FlintScanner() {
    
}
