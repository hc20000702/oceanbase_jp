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
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "rc.h"
#include "sql/stmt/stmt.h"
#include "storage/common/field.h"
#include "sql/expr/expresions/BaseExpression.h"
#include "sql/operator/QueryOperator.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;

class InnerJoin {
public:
    InnerJoin(InnerJoin* left, Table* right, FilterStmt* filterStmt): left(left), right(right), filterStmt(filterStmt){}
    void setParent(InnerJoin* par){parent = par;}
public:
    InnerJoin* parent = nullptr;
    InnerJoin* left;
    Table* right;
    FilterStmt *filterStmt  = nullptr;
};

class SelectStmt : public Stmt
{
public:

  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override { return StmtType::SELECT; }
public:
  static RC create(Db *db, const Selects &select_sql, Stmt *&stmt);

public:
  const std::vector<Table *> &tables() const { return tables_; }
  const std::vector<Field> &query_fields() const { return query_fields_; }
  FilterStmt *filter_stmt() const { return filter_stmt_; }
  static void wildcard_fields(Table *table, std::vector<Field> &field_metas);
  static void expandStar(Table* table, std::vector<BaseExpression*>& expressions);
  std::vector<BaseExpression*>& getExpressions(){ return expressions_; }

  RC generateOperatorFlow(QueryOperator* &queryOperator);
  void addScanOperator(Operator*);
  bool containAggregateFunction();
  RC collectAggregate(std::vector<BaseExpression*>& collect);
  std::unordered_map<std::string, std::string>& getTableAliasMap(){
    return tableAliasMap_;
  }
  static std::unordered_map<std::string, std::string> staticTableAliasMap_;

private:
  std::vector<BaseExpression*> expressions_;
  std::vector<BaseExpression*> groupBy_;
  std::vector<std::pair<BaseExpression*, bool> > orderBy_;
  std::vector<Field> query_fields_;
  std::vector<Table *> tables_;
  FilterStmt *filter_stmt_ = nullptr;
  FilterStmt *having_filter_stmt_ = nullptr;
  std::vector<Operator*> scanOperators_;
  std::vector<InnerJoin*> inner_joins_;
  std::unordered_map<const Table*, Relation*> relations_;
  std::unordered_map<std::string, std::string> tableAliasMap_;
};

