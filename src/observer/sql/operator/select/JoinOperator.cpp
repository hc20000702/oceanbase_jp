//
// Created by zx on 10/23/22.
//

#include "JoinOperator.h"
#include "sql/stmt/filter_stmt.h"

RC JoinOperator::close() {
  RC rc = SUCCESS;
  for(auto it: subOperators){
    rc = it->close();
    if(rc != SUCCESS){
      return rc;
    }
  }
  return rc;
}

RC filter_row_tree(const std::vector<FilterUnit *> &filter_units, int cur, Row &row, bool &flag)
{
  RC rc = RC::SUCCESS;
  const CompOp comp = filter_units[cur]->comp();
  // 非叶子condition
  if (comp == AND_OP || comp == OR_OP) {
    rc = filter_row_tree(filter_units, filter_units[cur]->leftChild(), row, flag);
    if ((rc != RC::SUCCESS) || (comp == AND_OP && !flag) || (comp == OR_OP && flag)) return rc;
    rc = filter_row_tree(filter_units, filter_units[cur]->rightChild(), row, flag);
  } else {
    rc = filter_units[cur]->compare(row, flag);
  }
  return rc;
}

RC JoinOperator::evaluate(Relation *& relation) {
  RC rc;
  flag = true;
  for(int i = 0 ; i < relNum; ++i){
    relations.emplace_back(nullptr);
    indexes.emplace_back(0);
    relLengths.emplace_back(0);
  }

  std::vector<Schema> schemas;

  for(size_t i = 0 ; i < subOperators.size(); ++i){
    rc = subOperators[i]->evaluate(relations[i]);
    if (rc != RC::SUCCESS) return rc;
    for(auto it: relations[i]->getSchemas()){
      schemas.push_back(it);
    }
    if (rc != RC::SUCCESS) return rc;
    relLengths[i] = relations[i]->getLength();
    if(flag){
      flag = !relations[i]->isEmpty();
    }
  }

  relation = new Relation(schemas);
  rc = filterStmt_->mapToRelation(relation);
  if(rc != SUCCESS) return rc;

  while(hasNext()){
    Row row = nextRow();
    bool satisfy;
    if(filterStmt_->filter_units().size() == 0){
      rc = relation->insertRow(row);
      if(rc!=SUCCESS) return rc;
      continue;
    } else if (!filterStmt_->or_mode()) {
      for(const FilterUnit *filter_unit : filterStmt_->filter_units()){
        rc = filter_unit->compare(row, satisfy);
        if (rc != RC::SUCCESS) return rc;
        if(!satisfy) break;
      }
    } else {
      rc = filter_row_tree(filterStmt_->filter_units(), filterStmt_->filter_units().size()-1, row, satisfy);
    }
    if(satisfy) {
      rc = relation->insertRow(row);
      if(rc!=SUCCESS) return rc;
    }
  }

  for(auto it: relations){
    it->releaseAll();
  }

  return rc;
}

bool JoinOperator::hasNext(){
  for(int i = 0 ; i < relNum; ++i){
    if(indexes[i] != 0){
      return true;
    }
  }
  if(flag){
    flag = false;
    return true;
  }
  return false;
}

Row JoinOperator::nextRow(){
  Row row;
  for(int i = 0 ; i < relNum ; ++i){
    row.insertRow(relations[i]->getRowAt(indexes[i]));
  }

  int push = 0;
  indexes[relNum - 1] ++;
  for(int i = relNum - 1 ; i >= 0; --i){
    indexes[i] += push;
    push = indexes[i] / relLengths[i];
    indexes[i] %= relLengths[i];
  }

  return row;
}