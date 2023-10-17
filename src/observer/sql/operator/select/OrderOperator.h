#ifndef MINIDB_ORDEROPERATOR_H
#define MINIDB_ORDEROPERATOR_H

#include "sql/operator/QueryOperator.h"
#include "sql/stmt/filter_stmt.h"

class OrderOperator : public QueryOperator{
public:
  OrderOperator(QueryOperator* sub_operator, std::vector<std::pair<BaseExpression*, bool> > orderBy):
        orderBy_(orderBy), subOperator(sub_operator){}

  ~OrderOperator(){
    if (subOperator != nullptr) {
      delete subOperator;
      subOperator = nullptr;
    }
  }

  RC evaluate(Relation * &) override;

  RC close() override;

  void setSchemaOfFather(std::vector<Schema> schemaOfFather) override{
    subOperator->setSchemaOfFather(schemaOfFather);
    schemaOfFather_ = schemaOfFather;
  }

  virtual void setRowFromFather(Row rowFromFather) override{
    subOperator->setRowFromFather(rowFromFather);
    rowFromFather_ = rowFromFather;
  }
private:
  std::vector<std::pair<BaseExpression*, bool> > orderBy_;
  QueryOperator* subOperator;
};

#endif  // MINIDB_ORDEROPERATOR_H
