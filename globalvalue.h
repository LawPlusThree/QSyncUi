#ifndef GLOBALVALUE_H
#define GLOBALVALUE_H
#include <QAtomicInt>

// 定义一个全局的原子变量
QAtomicInt globalFileTaskId(0);
int getNextFileTaskId() {
    return globalFileTaskId.fetchAndAddRelaxed(1);
}


#endif // GLOBALVALUE_H
