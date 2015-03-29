#include "CTestAdapter.h"

mfunc_item<CTestAdapter> CTestAdapter::mfuncs[4] = {
    {"Add", &CTestAdapter::Add},
    {"getx", &CTestAdapter::getx},
    {"setx", &CTestAdapter::setx},
    {NULL, NULL}
};

