
#ifndef __FLINT_CODE_INFO_H
#define __FLINT_CODE_INFO_H

class ScanCodeInfo {
private:
    int cursor;
    int line;
    int startCol;
    int endCol;
    int length;
    const char *code;
public:
    ScanCodeInfo(void);
    void setInfo(int cursor, int line, int startCol, int endCol);
    int getCursor(void);
    int getLine(void);
    int getStartCol(void);
    int getEndCol(void);
    int getLength(void);
    const char *getCode(void);
    void setCode(const char *code);
    void setCode(const char *code, int length);
    ~ScanCodeInfo();
};

#endif // __FLINT_CODE_INFO_H
