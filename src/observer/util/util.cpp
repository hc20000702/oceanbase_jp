/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai on 2022/9/28
//

#include <string.h>
#include "util/util.h"
#include "sql/expr/expresions/ExpressionHead.h"
#include "sql/expr/expresions/FunctionHelper.h"

std::string double2string(double v)
{
  char buf[256];
  snprintf(buf, sizeof(buf), "%.2f", v);
  size_t len = strlen(buf);
  while (buf[len - 1] == '0') {
    len--;
      
  }
  if (buf[len - 1] == '.') {
    len--;
  }

  return std::string(buf, len);
}

double char2double(const char *str)
{
  char buf[256];
  size_t len = strlen(str);
  if (len > 255) {
    len = 255;
  }
  size_t i = 0;
  for (; i < len; i++) {
    if (str[i] < '0' || str[i] > '9') {
      break;
    }
    buf[i] = str[i];
  }
  if (i < len && str[i] == '.') {
    buf[i] = str[i];
    i++;
    for (; i < len; i++) {
      if (str[i] < '0' || str[i] > '9') {
        break;
      }
      buf[i] = str[i];
    }
  }
  buf[i] = '\0';
  return atof(buf);
}

char* parse2int(const char* literal){
  int* value = new int;
  *value = atoi(literal);
  return (char*) value;
}

char* parse2float(const char* literal){
  float* value = new float;
  *value = atof(literal);
  return (char*) value;
}

RC constructExpression__(const Expr* expr, std::map<const Expr*,BaseExpression*>& exprMap,Table* defaultTable,
    std::unordered_map<std::string, Table *> *tables,
    Db *db);

BaseExpression* constructExpression(const Expr* expr,Table* defaultTable, std::unordered_map<std::string, Table *> *tables, Db *db){
  std::map<const Expr*, BaseExpression*> exprMap;
  RC rc = constructExpression__(expr, exprMap, defaultTable, tables, db);
  if(rc != SUCCESS){
    return nullptr;
  }
  if(expr->alias){
    exprMap[expr]->setAlias(expr->alias);
  }
  return exprMap[expr];
}

std::string toUpperCase(std::string origin){
  std::transform(origin.begin(), origin.end(), origin.begin(), ::toupper);
  return origin;
}

RC constructExpression__(const Expr* expr, std::map<const Expr*, BaseExpression*>& exprMap,Table* defaultTable, std::unordered_map<std::string, Table *> *tables, Db *db){
  if(exprMap.count(expr)>0) return SUCCESS;
  switch (expr->type) {
    case BINARY:{
      Expr *left = expr->binaryExpr.left;
      Expr *right = expr->binaryExpr.right;
      RC rc;
      if(exprMap.count(left) == 0){
        rc = constructExpression__(left, exprMap, defaultTable, tables, db);
        if(rc!=SUCCESS){
          return rc;
        }
      }
      if(exprMap.count(right) == 0){
        rc = constructExpression__(right, exprMap, defaultTable, tables, db);
        if(rc!=SUCCESS){
          return rc;
        }
      }
      BaseExpression* binary = new BinaryExpression(exprMap[left], exprMap[right], expr->binaryExpr.op);
      binary->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, binary));
      return SUCCESS;
    }
    case UNARY:{
      Expr *subExpr = expr->unaryExpr.subExpr;
      if(exprMap.count(subExpr) == 0){
        RC rc = constructExpression__(expr->unaryExpr.subExpr, exprMap, defaultTable, tables, db);
        if(rc!=SUCCESS){
          return rc;
        }
      }
      if(expr->unaryExpr.op == OP_ADD){
        exprMap.insert(std::make_pair(expr, exprMap[subExpr]));
        return SUCCESS;
      } else {
        BaseExpression* unary = new NegativeExpression(exprMap[subExpr]);
        unary->setBracketed(expr->isBracketed);
        exprMap.insert(std::make_pair(expr, unary));
        return SUCCESS;
      }
    }
    case CONST:{
      BaseExpression* constant = new ConstExpression(expr->constExpr.data, expr->constExpr.type);
      constant->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, constant));
      return SUCCESS;
    }
    case FIE:{
      const char* tableName = expr->fieldExpression.table_name;
      const char* attrName = expr->fieldExpression.attr_name;

      Table* table = nullptr;
      if(tableName == nullptr && defaultTable != nullptr){
        table = defaultTable;
        tableName = table->name();
      }else{
        if(tables !=nullptr && tables->count(tableName) != 0){
          table = (*tables)[tableName];
        }else if(db != nullptr){
          table = db->find_table(tableName);
        }
      }

      if(!table){
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }

      const FieldMeta* fieldMeta = table->findField(attrName);
      if(fieldMeta == nullptr){
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }

      BaseExpression* fieldExpr = new IdentifierExpression(tableName, attrName, fieldMeta->type());
      fieldExpr->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, fieldExpr));
      return SUCCESS;
    }
    case FUNC:{
      RC rc;
      std::vector<BaseExpression*> paras;

      // 特殊处理count(*)
      if(expr->funcExpr.paraNum == 1
          && expr->funcExpr.exprList[0]->type == FIE
          && strcmp(expr->funcExpr.exprList[0]->fieldExpression.attr_name, "*") == 0){
        BaseExpression* funcExpr;
        if(toUpperCase(expr->funcExpr.funcName) != "COUNT"){
          return GENERIC_ERROR;
        }
        rc = functionFactory("COUNT_STAR" , paras, funcExpr);
        if(rc!=SUCCESS) return rc;
        funcExpr->setBracketed(expr->isBracketed);
        exprMap.insert(std::make_pair(expr, funcExpr));
        return SUCCESS;
      }

      // parse paras
      for(size_t i = 0; i < expr->funcExpr.paraNum ; ++i){
        Expr* subExpr = expr->funcExpr.exprList[expr->funcExpr.paraNum - 1 - i];
        if(exprMap.count(subExpr) == 0) {
          rc = constructExpression__(subExpr, exprMap, defaultTable, tables, db);
          if (rc != SUCCESS) {
            return rc;
          }
        }
        if(exprMap.count(subExpr) == 0){
          return GENERIC_ERROR;
        }
        paras.emplace_back(exprMap[subExpr]);
      }

      BaseExpression* funcExpr;
      rc = functionFactory(expr->funcExpr.funcName, paras, funcExpr);
      if(rc!=SUCCESS) return rc;
      funcExpr->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, funcExpr));
      return SUCCESS;
    }
    case LIST: {
      BaseExpression* list = new RelationExpression(expr->listExpr.values, expr->listExpr.value_num);
      list->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, list));
      return RC::SUCCESS;
    }
    case SUBQUERY: {
      RC rc;
      RelationExpression* select = new RelationExpression(db, expr->selectExpr, rc);
      if(rc!=RC::SUCCESS) return rc;
//      select->setBracketed(expr->isBracketed);
      exprMap.insert(std::make_pair(expr, select));
      return RC::SUCCESS;
    }
    default:
      return GENERIC_ERROR;
  }
}

std::string date2string(int v) {
  std::string date;
  int day = v % 100;
  std::string str_day = std::to_string(day);
  if (str_day.size() == 1) {
    str_day = "0" + str_day;
  }
  int year = v / 10000;
  std::string str_year = std::to_string(year);
  int month = (v % 10000) / 100;
  std::string str_month = std::to_string(month);
  if (str_month.size() == 1) {
    str_month = "0" + str_month;
  }
  date = str_year + "-" + str_month + "-" + str_day;
  return date;

}

RC type_cast(Value *value, const FieldMeta *fieldMeta) {
  RC rc = RC::SUCCESS;
  AttrType from_type = value->type;
  AttrType to_type = fieldMeta->type();
  if (to_type == CHARS && (from_type == INTS || from_type == FLOATS)) {  // INT-->CHAR
    void *p = value->data;
    if (from_type == INTS) {
      snprintf((char *)value->data, fieldMeta->len(), "%d", *(int *)p);
    } else {
      snprintf((char *)value->data, fieldMeta->len(), "%f", *(float *)p);
    }
    value->type = CHARS;
  } else if (to_type == INTS && (from_type == FLOATS || from_type == CHARS)) { // CHAR --> INT
    void *p = value->data;
    int new_value;
    if (from_type == FLOATS) {
      new_value = int(*(float *)p + 0.5);
    } else {
      new_value = (int)char2double((char *)p);
    }
    value->type = INTS;
    memcpy(value->data, &new_value, sizeof(int));
  } else if (to_type == FLOATS && (from_type == INTS || from_type == CHARS)) {
    void *p = value->data;
    float new_value;
    if (from_type == INTS) {
      new_value = *(int *)p;
    } else {
      new_value = char2double((char *)p);
    }
    value->type = FLOATS;
    memcpy(value->data, &new_value, sizeof(float));
  } else {
    LOG_WARN("field type mismatch. field=%s, field type=%d, value_type=%d",
        fieldMeta->name(), from_type, to_type);
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  return rc;
}

RC relation2item(Relation *relation, Item *&item, AttrType &type)
{
  if (relation->getLength() > 1 || (relation->getLength() == 1 && relation->getWidth() > 1)) {
    return RC::ERROR_INPUT_TYPE;
  }
  item = relation->getLength() == 1 ? relation->getItem(0, 0) : nullptr;
  type = relation->getLength() == 1 ? relation->getSchema(0).attrType() : NULLS;
  if (item && item->isNull()) {
    type = NULLS;
  }
  return RC::SUCCESS;
}