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

#pragma once

#include <vector>
#include <unordered_map>
#include "rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "sql/expr/expression.h"
#include "sql/expr/expresions/BaseExpression.h"

class Db;
class Table;
class FieldMeta;

class FilterUnit
{
public:
  FilterUnit() = default;
  ~FilterUnit()
  {
    if (left_) {
      delete left_;
      left_ = nullptr;
    }
    if (right_) {
      delete right_;
      right_ = nullptr;
    }
  }
  
  void set_comp(CompOp comp) {
    comp_ = comp;
  }

  RC compare(Row& row, bool &result) const{
    return BaseExpression::compare(left_, right_, row, comp_, result);
  }

  CompOp comp() const {
    return comp_;
  }

  void set_left(BaseExpression *expr)
  {
    left_ = expr;
  }
  void set_right(BaseExpression *expr)
  {
    right_ = expr;
  }
  BaseExpression *left() const
  {
    return left_;
  }
  BaseExpression *right() const
  {
    return right_;
  }
  void set_leftChild(int leftChild)
  {
    leftChild_ = leftChild;
  }
  void set_rightChild(int rightChild)
  {
    rightChild_ = rightChild;
  }
  int leftChild() const
  {
    return leftChild_;
  }
  int rightChild() const
  {
    return rightChild_;
  }

  RC mapToRelation(Relation*);

  const char* isScanFilter() const;

  bool isJoinFilter() const;

private:
  CompOp comp_ = NO_OP;
  BaseExpression *left_ = nullptr;
  BaseExpression *right_ = nullptr;
  int leftChild_ = -1;
  int rightChild_ = -1;
};

class FilterStmt 
{
public:

  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  const std::vector<FilterUnit *> &filter_units() const
  {
    return filter_units_;
  }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
			const Condition *conditions, int condition_num,
			FilterStmt *&stmt);
  static RC create_tree(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
		      const Condition *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
			       const Condition &condition, FilterUnit *&filter_unit);
  static RC create_filter_unit_tree(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
             const Condition *&conditions, int cur, FilterStmt *&stmt, FilterUnit *&filter_unit, int &idx);

  RC mapToRelation(Relation* relation);

  FilterStmt* getScanFilterStmtByTableName(const char* table);
  FilterStmt* getJoinFilterStmt();
  void push_back(FilterUnit *&filter_unit) { filter_units_.push_back(filter_unit); }
  bool or_mode() { return or_mode_; }

private:
  std::vector<FilterUnit *>  filter_units_; // 默认当前都是AND关系
  bool or_mode_ = false;
};
