
#include "flint_error_msg.h"
#include <stdarg.h>
#include <string.h>
#include <iostream>

using namespace std;

FlintError::FlintError(ScanCodeInfo *codeInfo, const char *msg) {
    initFlintError(codeInfo->getLine(), codeInfo->getStartCol(), codeInfo->getEndCol(), 1, msg);
}

FlintError::FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2) {
    initFlintError(codeInfo->getLine(), codeInfo->getStartCol(), codeInfo->getEndCol(), 2, msg1, msg2);
}

FlintError::FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2, const char *msg3) {
    initFlintError(codeInfo->getLine(), codeInfo->getStartCol(), codeInfo->getEndCol(), 3, msg1, msg2, msg3);
}

FlintError::FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2, const char *msg3, const char *msg4) {
    initFlintError(codeInfo->getLine(), codeInfo->getStartCol(), codeInfo->getEndCol(), 4, msg1, msg2, msg3, msg4);
}

FlintError::FlintError(int line, int startCol, int endCol, const char *msg) {
    initFlintError(line, startCol, endCol, 1, msg);
}

void FlintError::initFlintError(int line, int startCol, int endCol, int length, ...) {
    va_list ap;
    int sum = 0;
    int count = 0;
    int *len = new int[length];

    this->line = line;
    this->startCol = startCol;
    this->endCol = endCol;

    va_start(ap, length);
    for(int i = 0; i < length; i++ ) {
        const char *msg = va_arg(ap, const char *);
        if(msg) {
            len[i] = strlen(msg);
            sum += len[i];
        }
    }
    va_end(ap);

    if(sum == 0) {
        this->msg = 0;
        delete len;
        return;
    }

    this->msg = new char[sum + 1];
    va_start(ap, length);
    for(int i = 0; i < length; i++ ) {
        const char *msg = va_arg(ap, const char *);
        if(msg) {
            strcpy(&this->msg[count], msg);
            count += len[i];
        }
    }
    va_end(ap);

    delete len;
}

int FlintError::getLine(void) {
    return line;
}

int FlintError::getStartCol(void) {
    return startCol;
}

int FlintError::getEndCol(void) {
    return endCol;
}

void FlintError::print(void) {
    printf("\033[0;31mError: \033[0;32m%d, %d: \033[0;36m%s\033[0;37m\r\n", line + 1, startCol + 1, msg);
}

FlintError::~FlintError() {
    if(msg)
        delete msg;
}
