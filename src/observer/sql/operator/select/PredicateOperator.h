//
// Created by zx on 10/19/22.
//

#ifndef MINIDB_PREDICATEOPERATOR_H
#define MINIDB_PREDICATEOPERATOR_H

#include "sql/operator/QueryOperator.h"
#include "sql/stmt/filter_stmt.h"

class PredicateOperator1 : public QueryOperator{
public:
  PredicateOperator1(QueryOperator* sub_operator, FilterStmt *filter_stmt):
        filterStmt(filter_stmt), subOperator(sub_operator){}

  ~PredicateOperator1(){
    if (subOperator != nullptr) {
      delete subOperator;
      subOperator = nullptr;
    }
  }

  RC evaluate(Relation * &) override;

  RC close() override;

  void setSchemaOfFather(std::vector<Schema> schemaOfFather){
    subOperator->setSchemaOfFather(schemaOfFather);
    schemaOfFather_ = schemaOfFather;
  }

  void setRowFromFather(Row rowFromFather) override{
    subOperator->setRowFromFather(rowFromFather);
    rowFromFather_ = rowFromFather;
  }
private:
  FilterStmt *filterStmt = nullptr;
  QueryOperator* subOperator;
};

#endif  // MINIDB_PREDICATEOPERATOR_H
