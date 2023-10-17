#include "RelationExpression.h"
#include "sql/stmt/select_stmt.h"
#include "sql/operator/table_scan_operator.h"
#include "sql/operator/QueryOperator.h"
#include "sql/expr/relation/Relation.h"

RelationExpression::RelationExpression(const Value *values, const size_t value_num)
{
  // 保存类型的schema
  std::vector<Schema> schemas;
  Row row;
  for (size_t i = 0; i < value_num; i++) {
    Schema schema;
    schema.setType(values[i].type);
    schemas.emplace_back(schema);
    Item *ptr = nullptr;
    switch (values[i].type) {
      case INTS:
        ptr = new IntItem(*(int *)values[i].data, false);
        break;
      case DATES:
        ptr = new DateItem(*(int *)values[i].data, false);
        break;
      case FLOATS:
        ptr = new FloatItem(*(float *)values[i].data, false);
        break;
      case CHARS:
        ptr = new TextItem((char *)values[i].data, false);
        break;
      case NULLS:
        ptr = new NullItem;
        break;
      default:
        delete relation_;
        relation_ = nullptr;
        return;
    }
    row.add(ptr);
  }
  relation_ = new Relation(schemas);
  relation_->insertRow(row);
  needRecalculate_ = false;
}

RelationExpression::RelationExpression(Db *db, const Selects *select, RC& outRc)
{
  RC rc;
  SelectStmt *stmt = nullptr;
  relation_ = nullptr;
  rc = SelectStmt::create(db, *select, reinterpret_cast<Stmt *&>(stmt));
  if (rc != RC::SUCCESS) {
    LOG_ERROR("create sub query failed. ");
    outRc = RC::GENERIC_ERROR;
    return;
  }
  for (Table *t : stmt->tables()) {
    // TODO: index scan
    Operator *scan_oper = new TableScanOperator(t);
    stmt->addScanOperator(scan_oper);
  }

  rc = stmt->generateOperatorFlow(queryOperator);
  if (rc != RC::SUCCESS) {
    outRc = rc;
    return;
  }
  needRecalculate_ = true;
  outRc = RC::SUCCESS;
}

RC RelationExpression::mapToRelation(Relation *relation)
{
//  relation_ = relation;
  return RC::SUCCESS;
}

// tempo do nothing
RC RelationExpression::update(){
  if(needRecalculate_){
    queryOperator->setRowFromFather(rowFromFather_);
    RC rc = queryOperator->evaluate(relation_);
    if(rc!=SUCCESS) return rc;
    return queryOperator->close();
  }
  return RC::SUCCESS;
}

RC RelationExpression::setSchemaOfFather(std::vector<Schema> schemas){
  if(!needRecalculate_) return RC::SUCCESS;
  if(schemasSet){
    return RC::SUCCESS;
  }
  schemaOfFather_ = schemas;
  queryOperator->setSchemaOfFather(schemas);
  schemasSet = true;
  return RC::SUCCESS;
}