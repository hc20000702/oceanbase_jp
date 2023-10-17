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

#include "rc.h"
#include "sql/stmt/stmt.h"
#include "sql/operator/QueryOperator.h"
#include <vector>


class Table;
class FilterStmt;

class UpdateStmt : public Stmt
{
public:

  UpdateStmt() = default;
//  UpdateStmt(Table *table, Value *values, int value_amount);
//  UpdateStmt(Table *table, const Value *values, int value_amount, const char *attri_name, FilterStmt *filter_stmt);
  FilterStmt *filter_stmt() const { return filter_stmt_; }

public:
  static RC create(Db *db, Updates &update_sql, Stmt *&stmt);

public:
  Table *table() const {return table_;}
//  const Value *values() const { return values_; }
//  const char * attribute_name() const {return attribute_name_;}
  int value_amount() const { return value_amount_; }
  std::vector<Value*> &values()  { return values_; }
  const std::vector<const char*> &attribute_names() const { return attribute_names_; }
//  const std::vector<int> &is_value() const { return is_value_; }

  StmtType type() const override { return StmtType::UPDATE; }

private:
  Table *table_;
  std::vector<Value*> values_;

  std::vector<const char*> attribute_names_;
//  const Value *values_ = nullptr;
//  const char *attribute_name_ = nullptr;
  int value_amount_;
  FilterStmt *filter_stmt_;
};

