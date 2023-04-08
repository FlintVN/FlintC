
#include "flint_code_info.h"
#include <string.h>

ScanCodeInfo::ScanCodeInfo(void) {
    cursor = 0;
    line = 0;
    startCol = 0;
    endCol = 0;
    length = 0;
    code = 0;
}

void ScanCodeInfo::setInfo(int cursor, int line, int startCol, int endCol) {
    this->cursor = cursor;
    this->line = line;
    this->startCol = startCol;
    this->endCol = endCol;
}

int ScanCodeInfo::getCursor(void) {
    return cursor;
}

int ScanCodeInfo::getLine(void) {
    return line;
}

int ScanCodeInfo::getStartCol(void) {
    return startCol;
}

int ScanCodeInfo::getEndCol(void) {
    return endCol;
}

int ScanCodeInfo::getLength(void) {
    return length;
}

const char *ScanCodeInfo::getCode(void) {
    return code;
}

void ScanCodeInfo::setCode(const char *code) {
    int length = strlen(code);
    setCode(code, length);
}

void ScanCodeInfo::setCode(const char *code, int length) {
    this->length = length;
    if(this->code)
        delete this->code;
    char *buff = new char[length + 1];
    for(int i = 0; i < length; i++)
        buff[i] = code[i];
    buff[length] = 0;
    this->code = (const char *)buff;
}

ScanCodeInfo::~ScanCodeInfo() {
    if(code)
        delete code;
}
