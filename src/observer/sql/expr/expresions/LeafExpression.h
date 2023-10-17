//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_LEAFEXPRESSION_H
#define MINIDB_LEAFEXPRESSION_H

#include "BaseExpression.h"

// 表达式的叶节点，应当包括常数和属性
class LeafExpression : public BaseExpression{
public:
  virtual std::string toString() const = 0;
  virtual AttrType type() const= 0;

  virtual bool isId() const{ return false;}
  virtual bool isConst() const { return false; }

  virtual RC get_value(const Tuple &tuple, TupleCell &cell) const = 0;
};

#endif  // MINIDB_LEAFEXPRESSION_H
