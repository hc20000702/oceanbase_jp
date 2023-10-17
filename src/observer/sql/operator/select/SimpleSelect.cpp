//
// Created by zx on 10/27/22.
//

#include "SimpleSelect.h"
RC SimpleSelect::evaluate(Relation * &relation) {
  std::vector<Schema> schemas;
  RC rc;
  Row row;
  for(auto expr: expressions_){
    schemas.emplace_back(expr);
    rc = expr->calculate(row);
    if(rc!=SUCCESS) return rc;
    row.add(expr->toItem());
  }

  relation = new Relation(schemas);
  relation->insertRow(row);
  return rc;
}