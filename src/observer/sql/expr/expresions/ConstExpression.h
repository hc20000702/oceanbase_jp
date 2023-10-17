//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_CONSTEXPRESSION_H
#define MINIDB_CONSTEXPRESSION_H

#include "LeafExpression.h"

class ConstExpression : public LeafExpression{
public:
  ConstExpression(const char* literal, AttrType type) : literal_(literal), attrType(type) {
    parse();
  };

  ~ConstExpression() = default;
  RC mapToRelation(Relation*) override;
  RC calculate(Row&) override;

  bool isConst() const override {return true;}

  std::string toString() const override;
  std::string displayName(bool) const override { return toString(); }
  AttrType type() const override;

  const char* currentText() override{
    return literal_;
  }

  RC get_value(const Tuple &tuple, TupleCell &cell) const override;
  const char* fromSingleTable() const override;
  bool containAggregate() const override {return false;}
  std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return toString();
  }
private:
  const char* literal_;
  AttrType attrType;
  RC status;

private:
  void parse();
};

#endif  // MINIDB_CONSTEXPRESSION_H
