//
// Created by zx on 10/19/22.
//

#ifndef MINIDB_IDENTIFIEREXPRESSION_H
#define MINIDB_IDENTIFIEREXPRESSION_H

#include "LeafExpression.h"
#include "sql/expr/expression.h"
#include "unordered_map"
#include "storage/common/table.h"
#include "storage/common/db.h"
#include "sql/expr/relation/Relation.h"
#include "sql/expr/expresions/AggregateFunction.h"

class IdentifierExpression: public LeafExpression {
public:
  IdentifierExpression(const char* tableName, const char* fieldName, AttrType type)
  : tableName(tableName), fieldName(fieldName), attrType(type){  }
  const char* fromSingleTable() const override;
  RC mapToRelation(Relation* relation) override;
  RC calculate(Row& row) override;

  std::string toString() const override;
  std::string displayName(bool) const override;
  std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const;
  AttrType type() const override;

  const char* currentText() override{
    return textValue;
  }

  const char* getTableName(){
    return tableName;
  }

  const char* getFieldName(){
    return fieldName;
  }

  bool isId() const override {return true;}
    bool isConst() const override{return false;}

  RC get_value(const Tuple &tuple, TupleCell &cell) const override;

  static FieldExpr* IdExpr2FieldExpr(IdentifierExpression* idExpr);
  bool containAggregate() const override{
    return false;
  }
  virtual std::vector<BaseExpression*> collectAllAggregate() override{
    std::vector<BaseExpression*> vec;
    vec.emplace_back(new Identity(this));
    return vec;
  }

private:
  char* textValue;
  const char* tableName;
  const char* fieldName;
  AttrType attrType;

  Relation* relation;
  int column;
};

#endif  // MINIDB_IDENTIFIEREXPRESSION_H
