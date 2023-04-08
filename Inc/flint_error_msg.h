
#ifndef __FLINT_ERROR_MSG_H
#define __FLINT_ERROR_MSG_H

#include "flint_code_info.h"

class FlintError {
private:
    int line;
    int startCol;
    int endCol;
    char *msg;
    void initFlintError(int line, int startCol, int endCol, int length, ...);
public:
    FlintError(ScanCodeInfo *codeInfo, const char *msg);
    FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2);
    FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2, const char *msg3);
    FlintError(ScanCodeInfo *codeInfo, const char *msg1, const char *msg2, const char *msg3, const char *msg4);
    FlintError(int line, int startCol, int endCol, const char *msg);
    int getLine(void);
    int getStartCol(void);
    int getEndCol(void);

    void print();
    ~FlintError();
};

void flintError(const char *codeName, int line, int col, const char *msg);

#endif // __FLINT_ERROR_MSG_H
