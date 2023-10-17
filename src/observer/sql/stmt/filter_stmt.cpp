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
// Created by Wangyunlai on 2022/5/22.
//

#include "rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/common/db.h"
#include "storage/common/table.h"
#include "util/util.h"

FilterStmt::~FilterStmt()
{
  for (FilterUnit *unit : filter_units_) {
    delete unit;
  }
  filter_units_.clear();
}

RC FilterStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
		      const Condition *conditions, int condition_num,
		      FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt = nullptr;

  auto *tmp_stmt = new FilterStmt();
  for (int i = 0; i < condition_num; i++) {
    FilterUnit *filter_unit = nullptr;
    rc = create_filter_unit(db, default_table, tables, conditions[i], filter_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->filter_units_.push_back(filter_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC FilterStmt::create_tree(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
		      const Condition *conditions, int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt = nullptr;
  auto *tmp_stmt = new FilterStmt();


  if (condition_num <= 0) {
    stmt = tmp_stmt;
    return rc; 
  }
  int idx = 0;
  FilterUnit *filter_unit = nullptr;
  rc = create_filter_unit_tree(db, default_table, tables, conditions, condition_num-1, tmp_stmt, filter_unit, idx);
  if (rc != RC::SUCCESS) {
    delete tmp_stmt;
    LOG_WARN("failed to create filter units. ");
    return rc;
  }
  tmp_stmt->push_back(filter_unit);
  tmp_stmt->or_mode_ = true;

  stmt = tmp_stmt;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
		       const RelAttr &attr, Table *&table, const FieldMeta *&field)
{
  if (common::is_blank(attr.relation_name)) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(std::string(attr.relation_name));
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr.relation_name);
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr.relation_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr.attribute_name);
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr.attribute_name);
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

RC FilterStmt::create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
				  const Condition &condition, FilterUnit *&filter_unit)
{
  RC rc = RC::SUCCESS;
  
  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp >= NO_OP) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }

  BaseExpression *left = nullptr;
  BaseExpression *right = nullptr;

  if (comp != EXISTS && comp != NOT_EXISTS) {
    left = constructExpression(condition.leftExpr, default_table, tables, db);
    if(!left){
      return RC::EXPR_PARSE_ERROR;
    }
  }

  right = constructExpression(condition.rightExpr, default_table, tables, db);
  if(!right){
    return RC::EXPR_PARSE_ERROR;
  }

  filter_unit = new FilterUnit;
  filter_unit->set_comp(comp);
  filter_unit->set_left(left);
  filter_unit->set_right(right);

  // TODO 要支持字符串比较, 首先需要支持expression里字符串的解析
  // 检查两个类型是否能够比较
  return rc;
}

RC FilterStmt::create_filter_unit_tree(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
			       const Condition *&conditions, int cur, FilterStmt *&stmt, FilterUnit *&filter_unit, int &idx)
{
  RC rc = RC::SUCCESS;
  const Condition &condition = conditions[cur];
  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp >= NO_OP) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }
  // 非叶子condition
  if (comp == AND_OP || comp == OR_OP) {
    filter_unit = new FilterUnit;
    filter_unit->set_comp(comp);
    FilterUnit *left_filter_unit = nullptr;
    rc = create_filter_unit_tree(db, default_table, tables, conditions, condition.leftChild, stmt, left_filter_unit, idx);
    if (rc != RC::SUCCESS) {
      delete filter_unit;
      return rc;
    }
    filter_unit->set_leftChild(idx++);
    stmt->push_back(left_filter_unit);
    FilterUnit *right_filter_unit = nullptr;
    rc = create_filter_unit_tree(db, default_table, tables, conditions, condition.rightChild, stmt, right_filter_unit, idx);
    if (rc != RC::SUCCESS) {
      delete filter_unit;
      return rc;
    }
    filter_unit->set_rightChild(idx++);
    stmt->push_back(right_filter_unit);
    return rc;
  }
  BaseExpression *left = nullptr;
  BaseExpression *right = nullptr;

  if (comp != EXISTS && comp != NOT_EXISTS) {
    left = constructExpression(condition.leftExpr, default_table, tables, db);
    if(!left){
      return RC::EXPR_PARSE_ERROR;
    }
  }

  right = constructExpression(condition.rightExpr, default_table, tables, db);
  if(!right){
    return RC::EXPR_PARSE_ERROR;
  }

  filter_unit = new FilterUnit;
  filter_unit->set_comp(comp);
  filter_unit->set_left(left);
  filter_unit->set_right(right);

  return rc;
}

RC FilterStmt::mapToRelation(Relation *relation)
{
  RC rc;
  for(auto it: filter_units_){
    rc = it->mapToRelation(relation);
    if (rc != SUCCESS){
      return rc;
    }
  }
  return SUCCESS;
}
RC FilterUnit::mapToRelation(Relation * relation)
{
  RC rc;

  if (left_) {
    rc = left_->mapToRelation(relation);
    if(rc != SUCCESS) return rc;
  }
  if (right_) {
    rc = right_->mapToRelation(relation);
    if(rc != SUCCESS) return rc;
  }

  return SUCCESS;
}

const char* FilterUnit::isScanFilter() const{
  if(left_ == nullptr || right_ == nullptr){
    return nullptr;
  }
  const char* leftTable = left_->fromSingleTable();
  const char* rightTable = right_->fromSingleTable();

  if(leftTable == nullptr || rightTable == nullptr){
    return nullptr;
  }
  if( 0 == strcmp(leftTable, "*") ){
    return rightTable;
  }
  if( 0 == strcmp("*", rightTable) ){
    return leftTable;
  }
  if(0 == strcmp(leftTable, rightTable)){
    return rightTable;
  }else{
    return nullptr;
  }
}

bool FilterUnit::isJoinFilter() const{
  if(comp_ != EQUAL_TO){
    return false;
  }
  const char* leftTable = left_->fromSingleTable();
  const char* rightTable = right_->fromSingleTable();
  if(leftTable != nullptr && rightTable != nullptr){
    return 0 != strcmp(leftTable, rightTable);
  }
  return false;
}

FilterStmt* FilterStmt::getScanFilterStmtByTableName(const char* table){
  auto ret = new FilterStmt();
  for(auto it: filter_units_){
    if(it->isScanFilter() != nullptr && (0 == strcmp(table, it->isScanFilter()) || 0 == strcmp(it->isScanFilter(), "*"))){
      ret->filter_units_.emplace_back(it);
    }
  }
  return ret;
}

// TODO inner join请修改此处
FilterStmt* FilterStmt::getJoinFilterStmt(){
  auto ret = new FilterStmt();
  // TODO: 如果全是AND，没有OR，需要加进来
  // for(auto it: filter_units_){
  //   if(!it->isScanFilter()){
  //     ret->filter_units_.emplace_back(it);
  //   }
  // }
  return ret;
}