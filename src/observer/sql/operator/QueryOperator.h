//
// Created by zx on 10/18/22.
//

#ifndef MINIDB_QUERYOPERATOR_H
#define MINIDB_QUERYOPERATOR_H

#include "operator.h"
#include "sql/expr/relation/Relation.h"

class QueryOperator {
public:
  virtual ~QueryOperator() {}
  virtual RC evaluate(Relation * &) = 0;
  virtual RC close() = 0;
  void setMultiTable(bool multiTable){
    multiTable_ = multiTable;
  }
  bool isMultiTable(){
    return multiTable_;
  }

  virtual void setSchemaOfFather(std::vector<Schema> schemaOfFather) = 0;

  virtual void setRowFromFather(Row rowFromFather) = 0;

protected:
  bool multiTable_ = false;
  Row rowFromFather_;
  std::vector<Schema> schemaOfFather_;
};

#endif  // MINIDB_QUERYOPERATOR_H
