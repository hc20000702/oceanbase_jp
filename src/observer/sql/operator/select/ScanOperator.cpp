//
// Created by zx on 10/19/22.
//

#include "ScanOperator.h"
#include "sql/operator/ScanOperatorI.h"

RC ScanOperator::evaluate(Relation* &relation)
{
  RC rc;
//  const Table* table = ((ScanOperatorI*) scanOperator_)->getTable();
//  if(relations_->count(table) != 0){
//    relation = (*relations_)[table];
//    return RC::SUCCESS;
//  }
  if(relation_){
    relation = relation_;
    return RC::SUCCESS;
  }

  rc = scanOperator_->open();
  if(rc!=SUCCESS){
    return rc;
  }

  Relation* r = new Relation(schema_);

  while(scanOperator_->next() == RC::SUCCESS) {
    Tuple *tuple = scanOperator_->current_tuple();
    // 直接认为是RowTuple
    Row *row = Row::tupleToRow((RowTuple*) tuple);
    r->insertRow(*row);
    // 为了内存控制， insertRow是复制新的row
    delete row;
  }

  relation = r;
  relation->setMultiTable(isMultiTable());
  relation_ = r;
  return SUCCESS;
}

RC ScanOperator::close(){
  RC rc = scanOperator_->close();
  return rc;
}