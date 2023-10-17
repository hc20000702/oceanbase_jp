//
// Created by zx on 10/19/22.
//

#include "IdentifierExpression.h"

RC IdentifierExpression::mapToRelation(Relation* relation_){
  relation = relation_;
  column = relation_->findExpressionColumn(this);
  if(column == -1){
    return GENERIC_ERROR;
  }
  return SUCCESS;
}

RC IdentifierExpression::calculate(Row& row){
  if(row.at(column)->isNull()){
    isNull_ = true;
    return RC::SUCCESS;
  } else {
    isNull_ = false;
  }
  if(column>=row.size()){
    return RC::GENERIC_ERROR;
  }
  switch (type()) {
    case INTS:
    case DATES:
      intValue = row.at(column)->getInt();
      break ;
    case FLOATS:
      floatValue = row.at(column)->getFloat();
      break ;
    case CHARS:
      textValue = row.at(column)->getText();
      break ;
    case NULLS:
      isNull_ = true;
      // no value
      break;
    default:
      return GENERIC_ERROR;
  }
  return SUCCESS;
}

std::string IdentifierExpression::toString() const{
  std::string s;
  s.append(tableName).append(".");
  s.append(fieldName);
  return s;
}

std::string IdentifierExpression::displayName(bool needTableName) const
{
  std::string s;
  if(needTableName){
    s.append(tableName).append(".");
  }
  s.append(fieldName);
  return s;
}

std::string IdentifierExpression::displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
  std::string s;
  if(flag){
    if(map.count(tableName)!=0 && !map[tableName].empty())
      s.append(map[tableName]).append(".");
    else{
      s.append(tableName).append(".");
    };
  }
  s.append(fieldName);
  return s;
}

AttrType IdentifierExpression::type() const{
  return attrType;
}
// TODO: warning not used
static FieldExpr* IdExpr2FieldExpr(IdentifierExpression* idExpr,
    Table* defaultTable,
    std::unordered_map<std::string, Table *>* tables,
    Db* db){

  const char * tableName = idExpr->getTableName();
  const char * fieldName = idExpr->getFieldName();

  Table* table = nullptr;
  if(tableName == nullptr){
    table = defaultTable;
  }else{
    if(tables != nullptr && tables->count(tableName) != 0){
      table = (*tables)[tableName];
    }else if(db != nullptr){
      table = db->find_table(tableName);
    }
  }

  if(!table){
    return nullptr;
  }

  const FieldMeta* fieldMeta = table->findField(fieldName);

  FieldExpr* fieldExpr = new FieldExpr(table, fieldMeta);
  return fieldExpr;
}

RC IdentifierExpression::get_value(const Tuple &tuple, TupleCell &cell) const{
  RC rc;
  if(tableName == nullptr || 0!=strcmp(tuple.getTableName(), tableName)){
    return GENERIC_ERROR;
  }

  const TupleCellSpec * cellSpec;
  for(int i = 0 ; i < tuple.cell_num(); ++i){
    rc = tuple.cell_spec_at(i, cellSpec);
    if(rc != SUCCESS){
      return rc;
    }
    FieldExpr * fieldExpr = dynamic_cast<FieldExpr *>(cellSpec->expression());
    const Field &field = fieldExpr->field();
    if (0 == strcmp(fieldName, field.field_name())) {
      return tuple.cell_at(i, cell);
    }
  }
  return GENERIC_ERROR;
}

const char* IdentifierExpression::fromSingleTable() const{
  return tableName;
}