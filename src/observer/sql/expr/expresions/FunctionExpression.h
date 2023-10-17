//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_FUNCTIONEXPRESSION_H
#define MINIDB_FUNCTIONEXPRESSION_H

#include "BaseExpression.h"

class FunctionExpression : public BaseExpression{
public:
    virtual RC mapToRelation(Relation* relation) = 0;
    virtual RC calculate(Row& row) = 0;

    virtual std::string toString() const = 0;
    virtual std::string displayName(bool) const = 0;
    virtual AttrType type() const = 0;

    virtual bool isId() const{ return false;}
    virtual bool isConst() const{ return false; }

    virtual RC get_value(const Tuple &tuple, TupleCell &cell) const = 0;
    virtual const char * fromSingleTable() const = 0;
    virtual bool containAggregate() const = 0;
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const = 0;
};

#endif  // MINIDB_FUNCTIONEXPRESSION_H
