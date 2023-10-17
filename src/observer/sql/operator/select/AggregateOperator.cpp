//
// Created by zx on 10/24/22.
//

#include "AggregateOperator.h"

RC AggregateOperator::close(){
  return  subOperator_->close();
}

RC AggregateOperator::evaluate(Relation * &relation) {
  RC rc;

  Relation* r;
  rc = subOperator_->evaluate(r);
  if(rc != SUCCESS) return rc;

  std::vector<Schema> schemas;
  std::vector<std::vector<Item*>> columns;
  for(auto it: aggregates_){
    schemas.emplace_back(it);
    rc = it->mapToRelation(r);
    it->setGroupBy(groupBy_);
    if(rc!=SUCCESS) return rc;
    rc = it->evaluate(r);
    if(rc!=SUCCESS) return rc;
    columns.emplace_back(it->getColumn());
  }

  int len = columns[0].size();
  relation = new Relation(schemas);
  for(int i = 0 ; i < len; ++i) {
    Row row;
    for(size_t j = 0 ; j < aggregates_.size(); ++j){
      row.add(columns[j][i]);
    }
    rc = relation->insertRow(row);
    if(rc!=SUCCESS) return rc;
  }
  relation->setMultiTable(isMultiTable());
  return RC::SUCCESS;
}
