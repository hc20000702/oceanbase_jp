//
// Created by zx on 10/17/22.
//

#include "Relation.h"
#include "sql/expr/expresions/IdentifierExpression.h"

RC Relation::insertRow(Row& row){
  if(row.size() != width){
    return RC::GENERIC_ERROR;
  }
  rows_.emplace_back(row);
  length_ ++;
  return RC::SUCCESS;
}

RC Relation::rewrite(int column, int row, Item* item){
  if(column >= width || row >= rows_.size()){
    return RC::GENERIC_ERROR;
  }
  rows_[row].rewrite(column, item);
  return RC::SUCCESS;
}

int Relation::findExpressionColumn(BaseExpression *expr)
{
  for(size_t i = 0 ; i < schemas_.size(); ++i){
    auto schema = schemas_[i];
    if (expr->toString() == schema.getExpression()->toString()){
      return i;
    }
  }
  return -1;
}


void row_to_string(std::ostream &os, const Row &row)
{
  bool first_field = true;
  for (int i = 0; i < row.size(); i++) {
    if (!first_field) {
      os << " | ";
    } else {
      first_field = false;
    }
    row.at(i)->to_string(os);
  }
  os << std::endl;
}

void Relation::printToStream(std::ostream &os){
  const int column = getWidth();

  for (int i = 0; i < column; i++) {
    Schema schema = getSchema(i);
    if (i != 0) {
      os << " | ";
    }

    os << schema.display(multiTable);
  }

  if (column > 0) {
    os << '\n';
  }

  for(auto it : rows_){
    row_to_string(os, it);
  }
}

void Relation::printToStream(std::ostream &os, std::unordered_map<std::string, std::string>& tableAliasMap){
  const int column = getWidth();

  for (int i = 0; i < column; i++) {
    Schema schema = getSchema(i);
    if (i != 0) {
      os << " | ";
    }

    os << schema.display(multiTable, tableAliasMap);
  }

  if (column > 0) {
    os << '\n';
  }

  for(auto it : rows_){
    row_to_string(os, it);
  }
}

void Relation::releaseAll(){
  for(auto  it: rows_){
    it.releaseAll();
  }
}