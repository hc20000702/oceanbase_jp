//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_NEGATIVEEXPRESSION_H
#define MINIDB_NEGATIVEEXPRESSION_H

#include "BaseExpression.h"

class NegativeExpression : public BaseExpression{
public:
  NegativeExpression(BaseExpression * subExpression): expression(subExpression){
    type_ = inferType();
  }
  ~NegativeExpression() = default;

  RC mapToRelation(Relation* ) override;
  RC calculate(Row& row) override;

  std::string toString() const override;
  std::string displayName(bool) const override;

  AttrType type() const override;
  RC get_value(const Tuple &tuple, TupleCell &cell) const override;
  const char* fromSingleTable() const override;
  bool isConst() const override{return expression->isConst();}
  bool containAggregate() const override{ return false; }
  virtual std::vector<BaseExpression*> collectAllAggregate() override{
    return expression->collectAllAggregate();
  }
  std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const;

private :
  Relation* relation_;
  BaseExpression * expression;
  bool isInput_;
  AttrType type_;

  AttrType inferType();
  bool isInput(){
    return isInput_;
  }
};

#endif  // MINIDB_NEGATIVEEXPRESSION_H
