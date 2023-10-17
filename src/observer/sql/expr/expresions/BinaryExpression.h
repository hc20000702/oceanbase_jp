//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_BINARYEXPRESSION_H
#define MINIDB_BINARYEXPRESSION_H

#include "BaseExpression.h"
#include "sql/expr/relation/Relation.h"

class BinaryExpression : public BaseExpression{
public:
  BinaryExpression(BaseExpression* left, BaseExpression* right, ExprOp op)
      :leftExpression(left), rightExpression(right), op(op){
    type_ = inferType();
  }
  ~BinaryExpression() = default;

  RC mapToRelation(Relation*) override;
  RC calculate(Row& row) override;

  std::string toString() const override;
  std::string displayName(bool) const override;
  std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const;
  AttrType type() const override;

  RC get_value(const Tuple &tuple, TupleCell &cell) const override;
  const char* fromSingleTable() const override;
  bool isConst() const override{
    return leftExpression->isConst() && rightExpression->isConst();
  }

  bool containAggregate() const override {
    return leftExpression->containAggregate() || rightExpression->containAggregate();
  }
  virtual std::vector<BaseExpression*> collectAllAggregate() override{
    std::vector<BaseExpression*> left = leftExpression->collectAllAggregate();
    std::vector<BaseExpression*> right = rightExpression->collectAllAggregate();
    for(auto it: right){
      left.emplace_back(it);
    }
    return left;
  }

private:
  bool isInput() const{
    return isInput_;
  }

protected:
  BaseExpression * leftExpression;
  BaseExpression * rightExpression;
  Relation* relation_;
  ExprOp op;
  AttrType type_;
  bool isInput_;

  AttrType inferType();
};

#endif  // MINIDB_BINARYEXPRESSION_H
