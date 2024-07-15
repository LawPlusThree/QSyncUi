#include "globalvalue.h"
QAtomicInt globalFileTaskId=QAtomicInt(0);

int getNextFileTaskId() {
    return globalFileTaskId.fetchAndAddRelaxed(1);
}
