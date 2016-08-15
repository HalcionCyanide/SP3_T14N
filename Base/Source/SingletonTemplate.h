#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

#include "DetectMemoryLeak.h"

template<class Type>
class SingletonTemplate/*<Type>*/
{
public:
    static Type &accessing() {
        static Type Cant_touch_this;
        return Cant_touch_this;
    }

protected:
    SingletonTemplate() {};
    virtual ~SingletonTemplate() {};
};

#endif