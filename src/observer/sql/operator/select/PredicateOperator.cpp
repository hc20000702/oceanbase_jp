//
// Created by zx on 10/19/22.
//

#include "PredicateOperator.h"
#include "sql/expr/expresions/RelationExpression.h"

RC filter_row(FilterUnit *filter_unit, Row &preRow, std::vector<Schema> &withFatherSchema, bool &flag)
{
  RC rc = RC::SUCCESS;
  if(filter_unit->left() && filter_unit->left()->isRelation()){
    RelationExpression* leftRel = (RelationExpression*) filter_unit->left();
    leftRel->setSchemaOfFather(withFatherSchema);
    leftRel->setRowFromFather(preRow);
  }
  if (filter_unit->right()->isRelation())
  {
    RelationExpression* rightRel = (RelationExpression*) filter_unit->right();
    rightRel->setSchemaOfFather(withFatherSchema);
    rightRel->setRowFromFather(preRow);
  }
  rc = filter_unit->compare(preRow, flag);
  return rc;
}
RC filter_row_tree(const std::vector<FilterUnit *> &filter_units, int cur, Row &preRow, std::vector<Schema> &withFatherSchema, bool &flag)
{
  RC rc = RC::SUCCESS;
  const CompOp comp = filter_units[cur]->comp();
  // 非叶子condition
  if (comp == AND_OP || comp == OR_OP) {
    rc = filter_row_tree(filter_units, filter_units[cur]->leftChild(), preRow, withFatherSchema, flag);
    if ((rc != RC::SUCCESS) || (comp == AND_OP && !flag) || (comp == OR_OP && flag)) return rc;
    rc = filter_row_tree(filter_units, filter_units[cur]->rightChild(), preRow, withFatherSchema, flag);
  } else {
    rc = filter_row(filter_units[cur], preRow, withFatherSchema, flag);
  }
  return rc;
}

RC PredicateOperator1::evaluate(Relation *&relation)
{
  RC rc;
  Relation *r;
  rc = subOperator->evaluate(r);
  if(rc != RC::SUCCESS){
    return rc;
  }

//  if(filterStmt == nullptr || filterStmt->filter_units().empty()){
//    relation = r;
//    return SUCCESS;
//  }

  std::vector<Schema> withFatherSchema = r->getSchemas();
  withFatherSchema.insert(withFatherSchema.end(), schemaOfFather_.begin(), schemaOfFather_.end());
  Relation* tempRel = new Relation(withFatherSchema);
  // 应该把mapToRelation改成mapToSchema但我懒得改了
  rc = filterStmt->mapToRelation(tempRel);
//  delete tempRel;
  if(rc != SUCCESS) return rc;

  Relation* retRel = new Relation(r->getSchemas());
  auto it = r->begin();
  while (it != r->end()) {
    bool flag = true;
    Row preRow = *it;
    preRow.insertRow(rowFromFather_);
    if (filterStmt->filter_units().size() == 0) {
      retRel->insertRow(*it);
      ++it;
      continue;
    } else if (!filterStmt->or_mode()) {
      for(FilterUnit *filter_unit : filterStmt->filter_units()){
        rc = filter_row(filter_unit, preRow, withFatherSchema, flag);
        if (rc != RC::SUCCESS) return rc;
        if (!flag) break;
      }
    } else {
      rc = filter_row_tree(filterStmt->filter_units(), filterStmt->filter_units().size()-1, preRow, withFatherSchema, flag);
      if (rc != RC::SUCCESS) return rc;
    }
    if(flag) {
      retRel->insertRow(*it);
    }
    ++it;
  }

  relation = retRel;
  relation->setMultiTable(isMultiTable());
  return RC::SUCCESS;
}

RC PredicateOperator1::close(){
  return subOperator->close();
}