//
// Created by zx on 10/20/22.
//

#include "TransformerAndProjectOperator.h"

RC TransformerAndProjectOperator::evaluate(Relation * & relation) {
  RC rc;

  Relation* r;
  rc = subOperator->evaluate(r);
  if(rc != SUCCESS) return rc;

  std::vector<Schema> rSchemas = r->getSchemas();
  rSchemas.insert(rSchemas.end(), schemaOfFather_.begin(), schemaOfFather_.end());
  Relation* tempr = new Relation(rSchemas);

  std::vector<Schema> schemas;
  for(auto it: expressions){
    rc = it->mapToRelation(tempr);
    if(rc!=SUCCESS) return rc;
    schemas.emplace_back(it);
  }

  auto* retRelation = new Relation(schemas);

  for(auto it = r->begin(); it != r->end(); ++it){
    Row row;
    rc = constructRow(it, row);

    if(rc !=SUCCESS) return rc;

    retRelation->insertRow(row);
  }
  r->releaseAll();
  delete r;
  relation = retRelation;
  relation->setMultiTable(isMultiTable());
  return SUCCESS;
}

RC TransformerAndProjectOperator::constructRow(std::vector<Row>::iterator & it, Row& row){
  RC rc;
  for(auto expr: expressions){
    Row calRow = *it;
    calRow.insertRow(rowFromFather_);
    rc = expr->calculate(calRow);
    if(rc != SUCCESS) return rc;

    Item * ptr = expr->toItem();
    if(!ptr){
      return GENERIC_ERROR;
    }
    row.add(ptr);
  }
  return SUCCESS;
}

RC TransformerAndProjectOperator::close()
{
  return subOperator->close();
}