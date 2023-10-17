#ifndef MINIDB_RELATIONEXPRESSION_H
#define MINIDB_RELATIONEXPRESSION_H

#include "BaseExpression.h"
#include "storage/common/db.h"
#include "sql/operator/QueryOperator.h"
class Schema;
class RelationExpression : public BaseExpression {
public:
  RelationExpression(const Value *values, const size_t value_num);
  RelationExpression(Db *db, const Selects *select, RC& rc);

  RC mapToRelation(Relation *relation) override;
  // 不可能从row读取数据
  RC calculate(Row &row) override
  {
    return RC::SUCCESS;
  };

  std::string toString() const override
  {
    return "";
  };
  std::string displayName(bool, std::unordered_map<std::string, std::string> &) const override
  {
    return "";
  };
  std::string displayName(bool) const override
  {
    return "";
  };

  AttrType type() const override
  {
    return RELATIONS;
  };
  bool isConst() const override
  {
    return true;
  }
  RC get_value(const Tuple &tuple, TupleCell &cell) const override
  {
    return RC::SHOULD_NOT_BE_CALLED;
  };

  const char *fromSingleTable() const override
  {
    return "*";
  };

  bool containAggregate() const override
  {
    return false;
  };

  Relation *relation() const
  {
    return relation_;
  }

  bool needRecalculate(){
    return needRecalculate_;
  }

  RC update();

  void setRowFromFather(Row& rowFromFather){
    if(needRecalculate_){
      rowFromFather_.clean();
      rowFromFather_ = rowFromFather;
    }
  }

  RC setSchemaOfFather(std::vector<Schema> schemas);

  bool isRelation() const {
    return true;
  }

private:
  // 列表转换为relation，子查询结果为relation，空或一列或一行
  Relation *relation_;
  Row rowFromFather_;
  std::vector<Schema> schemaOfFather_;
  bool schemasSet = false;
  QueryOperator *queryOperator;
  bool needRecalculate_ = true;
};

#endif  // MINIDB_RELATIONEXPRESSION_H
