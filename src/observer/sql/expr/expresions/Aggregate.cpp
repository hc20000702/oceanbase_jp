//
// Created by zx on 10/24/22.
//

#include "Aggregate.h"
#include "sql/expr/relation/Relation.h"

RC Aggregate::evaluate(Relation* &relation){
  RC rc;
  setSource(relation);
  for(auto it: groupBy_){
    rc = it->mapToRelation(relation);
    if(rc!=SUCCESS) return rc;
  }
  rc = beforeStart();
  if(rc != SUCCESS) return rc;
  auto it = source->begin();
  if(source->begin() == source->end()){
    dealWithEmptyRelation();
  }else{
    while (it!=source->end()){
      currentRow = *it;
      rc = treatRow();
      if(rc != SUCCESS) return rc;
      it++;
    }
  }
  rc = afterDealAllRow();
  return rc;
}


std::vector<Item*> Aggregate::getColumn(){
  std::vector<Item*> column;
  for(const auto& it: order){
    column.emplace_back(context[it]);
  }
  cacheClear();
  order.clear();
  context.clear();
  return column;
}