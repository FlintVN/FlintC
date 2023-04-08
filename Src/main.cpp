
#include <iostream>
#include "flint_parser.h"
#include "flint_error_msg.h"

using namespace std;

const char *sourceCode = "import package1\nimport package2;\r\nimport package3;\r\n\r\npublic class HelloWord \r\n\r\n}";

void test() throw(FlintError *) {
    FlintParser *flintCode = new FlintParser(sourceCode);

    ScanCodeInfo **imports = flintCode->getImportList();
    while(*imports) {
        printf("%s\r\n", (*imports)->getCode());
        imports++;
    }

    printf("\r\n%s\r\n", flintCode->getClassName()->getCode());

    delete flintCode;
}

int main() {
    try {
        test();
    }
    catch(FlintError *err) {
        err->print();
        delete err;
    }

    return 0;
}
