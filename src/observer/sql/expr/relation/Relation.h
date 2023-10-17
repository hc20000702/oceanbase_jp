//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_RELATION_H
#define MINIDB_RELATION_H

#include "Row.h"
#include "vector"
#include "string"
#include "sql/parser/parse_defs.h"
#include "sql/expr/expresions/BaseExpression.h"
#include "string.h"
#include "unordered_map"

class Schema{
private:
  BaseExpression * expr_;
  AttrType type;
  char* alia_;
  bool hasAlia;

public:
  Schema() = default;
  Schema(BaseExpression *expr): expr_(expr){
    type = expr->type();
    alia_ = nullptr;
    if(expr->getAlias() != nullptr){
      alia_ = strdup(expr_->getAlias());
    }
  }

  AttrType attrType(){
    return type;
  }

  void setType(AttrType t) {
    type = t;
  }

  void setAlia(const char* alia){
    alia_ = strdup(alia);
  }

  bool isExpr(BaseExpression *expr){
    return expr == expr_;
  }

  BaseExpression* getExpression(){
    return expr_;
  }

  std::string display(bool needTableName, std::unordered_map<std::string, std::string>& tableAliasMap){
    if (alia_ == nullptr){
      return expr_->displayName(needTableName, tableAliasMap);
    }
    return alia_;
  }

  std::string display(bool needTableName){
    if (alia_ == nullptr){
      return expr_->displayName(needTableName);
    }
    return alia_;
  }

  const char* fromSingleTable(){
    return expr_->fromSingleTable();
  }

  Schema& operator=(const Schema& schema){
    if (this == &schema) {
      return *this;
    }

    this->expr_ = schema.expr_;
    this->type = schema.type;
    this->alia_ = strdup(schema.alia_);
    this->hasAlia = schema.hasAlia;
    return *this;
  }
};

class Relation {
private:
  std::vector<Row> rows_;
  int width;
  std::vector<Schema> schemas_;
  bool multiTable = false;
  int length_ = 0;

public:
  Relation(const std::vector<Schema>& schemas): schemas_(schemas){
    width = schemas_.size();
  }
  ~Relation() = default;

  RC insertRow(Row& row);

  RC rewrite(int column, int row, Item* item);

  std::vector<Schema> getSchemas(){
    return schemas_;
  }

  Schema& getSchema(int column){
    return schemas_[column];
  }

  Item* getItem(int row, int column){
    return rows_[row].getItem(column);
  }

  int getWidth(){
    return width;
  }

  int getLength(){
    return rows_.size();
  }

  int findExpressionColumn(BaseExpression *expr);

  std::vector<Row>::iterator begin(){
    return rows_.begin();
  }

  std::vector<Row>::iterator end(){
    return rows_.end();
  }

  std::vector<Row>::iterator removeRow(std::vector<Row>::iterator row){
    return rows_.erase(row);
  }

  bool fromSingleTable();

  void printToStream(std::ostream &os);
  void printToStream(std::ostream &os, std::unordered_map<std::string, std::string>& tableAliasMap);
  void releaseAll();

  Row& getRowAt(int index){
    return rows_[index];
  }

  bool isEmpty(){
    return rows_.empty();
  }

  void setMultiTable(bool multitable){
    multiTable = multitable;
  }

  bool isMultiTable(){
    return multiTable;
  }

  Row& rowAt(int index){
    return rows_[index];
  }

  std::vector<Row>& getRows() {
    return rows_;
  }
};

#endif  // MINIDB_RELATION_H
