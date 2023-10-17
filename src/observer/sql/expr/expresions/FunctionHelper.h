//
// Created by zx on 10/24/22.
//

#ifndef MINIDB_FUNCTIONHELPER_H
#define MINIDB_FUNCTIONHELPER_H

#include "FunctionExpression.h"
#include "SimpleFunction.h"
#include "AggregateFunction.h"
#include "SimpleFunction.h"
#include "algorithm"
#include "map"

typedef enum Func{
   MAX = 1,
   MIN,
   SUM,
   AVG,
   COUNT,
   COUNT_STAR,

   LENGTH,
   ROUND,
   DATE_FORMAT,
} Func;

extern std::map<std::string, Func> funcMap;

RC functionFactory(const char* funcName, std::vector<BaseExpression*> paras, BaseExpression*& target);


#endif //MINIDB_FUNCTIONHELPER_H
