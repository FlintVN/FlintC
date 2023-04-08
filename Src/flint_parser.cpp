
#include "flint_parser.h"
#include "flint.h"
#include <iostream>

FlintParser::FlintParser(const char *code) throw(FlintError *) {
    accessModifier = PRIVATE_ACCESS_MODIFIER;
    scanner = new FlintScanner(code);
    importList = 0;
    className = 0;
    startParser();
}

void FlintParser::startParser(void) throw(FlintError *) {
    parserImportList();
    parserClass();
}

void FlintParser::parserImportList(void) throw(FlintError *) {
    int count = 0;
    int currentListCount = 16;
    ScanCodeInfo **list = (ScanCodeInfo **)malloc(currentListCount * sizeof(ScanCodeInfo **));
    ScanCodeInfo *codeInfo = new ScanCodeInfo();
    while(true) {
        scanner->read(codeInfo);
        if(FlintScanner::isKeyword(codeInfo) != IMPORT_KEYWORD) {
            scanner->setCursor(codeInfo->getCursor());
            break;
        }

        if(scanner->read(codeInfo) == CHARS_TOKEN) {
            if(count >= currentListCount) {
                currentListCount += 16;
                list = (ScanCodeInfo **)realloc(list, currentListCount * sizeof(ScanCodeInfo **));
            }
            list[count] = codeInfo;
            codeInfo = new ScanCodeInfo();
        }
        else {
            FlintError *err = new FlintError(codeInfo, "Expected a package name");
            delete codeInfo;
            throw err;
        }

        scanner->read(codeInfo);
        if(codeInfo->getLength() != 1 || codeInfo->getCode()[0] != ';') {
            FlintError *err = new FlintError(codeInfo, "Missing \";\" before ", codeInfo->getCode());
            delete codeInfo;
            for(int i = 0; i < count; i++)
                delete list[i];
            delete list;
            throw err;
        }

        count++;
    }
    delete codeInfo;
    if(count == 0)
        delete list;
    else {
        importList = (ScanCodeInfo **)realloc(list, (count + 1) * sizeof(ScanCodeInfo **));
        importList[count] = 0;
    }
}

void FlintParser::parserClass(void) throw(FlintError *) {
    ScanCodeInfo *codeInfo = new ScanCodeInfo();
    scanner->read(codeInfo);
    KeywordToken keyword = FlintScanner::isKeyword(codeInfo);
    if((keyword == PRIVATE_KEYWORD) || (keyword == PUBLIC_KEYWORD)) {
        if(keyword == PUBLIC_KEYWORD)
            accessModifier = PUBLIC_ACCESS_MODIFIER;

        scanner->read(codeInfo);
        keyword = FlintScanner::isKeyword(codeInfo);
        if(keyword != CLASS_KEYWORD) {
            FlintError *err = new FlintError(codeInfo, "Missing \"class\" or \"enum\" keyword");
            delete codeInfo;
            throw err;
        }

        goto get_name;
    }
    else if(keyword != CLASS_KEYWORD) {
        FlintError *err = new FlintError(codeInfo, "Missing \"class\" or \"enum\" keyword");
        delete codeInfo;
        throw err;
    }
    else {
    get_name:
        if(scanner->read(codeInfo) != CHARS_TOKEN) {
            FlintError *err = new FlintError(codeInfo, "Expected a class name");
            delete codeInfo;
            throw err;
        }
        else if(FlintScanner::isNumber(codeInfo->getCode()[0])) {
            FlintError *err = new FlintError(codeInfo, "Class name can't start with a number");
            delete codeInfo;
            throw err;
        }
        
        className = codeInfo;
        codeInfo = new ScanCodeInfo();

        scanner->read(codeInfo);
        if(codeInfo->getLength() != 1 || codeInfo->getCode()[0] != '{') {
            FlintError *err = new FlintError(codeInfo, "Missing \"{\" after ", className->getCode());
            delete codeInfo;
            delete className;
            className = 0;
            throw err;
        }

        // TODO many many things here
    }
}

ScanCodeInfo **FlintParser::getImportList(void) {
    return importList;
}

ScanCodeInfo *FlintParser::getClassName(void) {
    return className;
}

FlintParser::~FlintParser() {
    if(scanner)
        delete scanner;
    if(importList) {
        ScanCodeInfo **temp = importList;
        while(*temp) {
            delete *temp;
            temp++;
        }
        delete importList;
    }
    if(className)
        delete className;
}
