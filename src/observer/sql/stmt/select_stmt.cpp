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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/common/db.h"
#include "util/util.h"
#include "sql/expr/expresions/ExpressionHead.h"
#include "sql/operator/select/ScanOperator.h"
#include "sql/operator/select/PredicateOperator.h"
#include "sql/operator/select/TransformerAndProjectOperator.h"
#include "sql/operator/select/JoinOperator.h"
#include "sql/operator/select/AggregateOperator.h"
#include "sql/expr/expresions/AggregateFunction.h"
#include "sql/operator/select/SimpleSelect.h"
#include "sql/operator/select/OrderOperator.h"
#include <algorithm>
#include "stack"

std::unordered_map<std::string, std::string> SelectStmt::staticTableAliasMap_({});

RC creatInnerJoin(Db* db, Table* defaultTable, std::unordered_map<std::string,Table *>& table_map,
                             InnerJoinNode* innerJoinNode, InnerJoin* &inner_join){
  RC rc;
  FilterStmt *filter_stmt = nullptr;

  if(innerJoinNode->leftNode == nullptr){
    // 递归出口
    inner_join = new InnerJoin(nullptr, table_map[innerJoinNode->rightTable], filter_stmt);
  }else{
    // 生成条件 fliter
    rc = FilterStmt::create(db, defaultTable, &table_map,
                            innerJoinNode->conditions, innerJoinNode->condition_num, filter_stmt);
    if (rc != RC::SUCCESS) return rc;

    // 递归产生左join
    rc = creatInnerJoin(db, defaultTable, table_map, innerJoinNode->leftNode, inner_join);
    if(rc != RC::SUCCESS) return rc;

    InnerJoin* higherJoin = new InnerJoin(inner_join, table_map[innerJoinNode->rightTable], filter_stmt);
    inner_join->setParent(higherJoin);
    inner_join = higherJoin;
  }

  return RC::SUCCESS;
}

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

void SelectStmt::wildcard_fields(Table *table, std::vector<Field> &field_metas)
{
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
//    expressions.emplace_back(new IdentifierExpression(table->name(), table_meta.field(i)->name(), table_meta.field(i)->type()));
    field_metas.emplace_back(table, table_meta.field(i));
  }
}
void SelectStmt::expandStar(Table* table, std::vector<BaseExpression*>& expressions){
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    expressions.emplace_back(new IdentifierExpression(table->name(), table_meta.field(i)->name(), table_meta.field(i)->type()));
  }
}


// 获取与表元素有关的叶节点
RC getExpressionLeafs(Expr* root, std::vector<Expr*>& container, const char* defaultTableName){
  RC rc = RC::SUCCESS;
  if (!root) {
    return rc;
  }
  switch(root->type){
    case BINARY:
      rc = getExpressionLeafs(root->binaryExpr.left, container, defaultTableName);
      if(rc != SUCCESS) return rc;
      rc = getExpressionLeafs(root->binaryExpr.right, container, defaultTableName);
      if(rc != SUCCESS) return rc;
      break;
    case UNARY:
      rc = getExpressionLeafs(root->unaryExpr.subExpr, container, defaultTableName);
      break;
    case FIE:
      if(0 == strcmp(root->fieldExpression.attr_name, "*")){
        return SUCCESS;
      }
      for (auto expr: container) {
        const char* exprTableName = expr->fieldExpression.table_name ? expr->fieldExpression.table_name : defaultTableName;
        const char* rootTableName = root->fieldExpression.table_name ? root->fieldExpression.table_name : defaultTableName;

        if(exprTableName == nullptr){
          continue;
        }
        if(strcmp(exprTableName, rootTableName) == 0 && strcmp(expr->fieldExpression.attr_name, root->fieldExpression.attr_name) == 0){
          return SUCCESS;
        }
      }
      container.emplace_back(root);
      break;
    case FUNC:
      for(size_t i = 0 ; i < root->funcExpr.paraNum; ++i){
        rc = getExpressionLeafs(root->funcExpr.exprList[i], container, defaultTableName);
        if(rc!=SUCCESS) return rc;
      }
      break;
    case CONST:
    case LIST:
    case SUBQUERY:
      break;
    default:
      LOG_ERROR("get expression leafs failed. ");
      rc = RC::GENERIC_ERROR;
  }
  return rc;
}

RC constructTableMap(Db* db, const Selects &select_sql,
                     std::unordered_map<std::string, Table *>& table_map,
                     std::vector<Table *>& tables,
                     std::unordered_map<std::string, std::string>& tableAliasMap){
  std::stack<const char *> tableNameStack;
  std::vector<const char *> tableNames;
  for(size_t i = 0 ; i < select_sql.join_num; ++i){
    InnerJoinNode* joinNode = select_sql.joinNodes[i];
    while(joinNode != nullptr){
      tableNameStack.push(joinNode->rightTable);
      if(joinNode->alias!=nullptr){
        tableAliasMap[joinNode->rightTable] = joinNode->alias;
      }
      joinNode = joinNode->leftNode;
    }
    while(!tableNameStack.empty()){
      tableNames.emplace_back(tableNameStack.top());
      tableNameStack.pop();
    }
  }

  for (const char *table_name : tableNames) {
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null");
      return RC::INVALID_ARGUMENT;
    }

    if(table_map.count(table_name) == 0){
      Table *table = db->find_table(table_name);
      if (nullptr == table) {
        LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      tables.push_back(table);
      table_map.insert(std::pair<std::string, Table*>(table_name, table));
    }
  }

  return RC::SUCCESS;
}

RC revertTableName(Expr* expr, std::multimap<std::string, std::string>& aliasTableMap){
  if (!expr) {
    return RC::SUCCESS;
  }
  RC rc;
  switch (expr->type) {
    case BINARY:
      rc = revertTableName(expr->binaryExpr.left, aliasTableMap);
      if(rc!=RC::SUCCESS) return rc;
      rc = revertTableName(expr->binaryExpr.right, aliasTableMap);
      if(rc!=RC::SUCCESS) return rc;
      break;
    case UNARY:
      rc = revertTableName(expr->unaryExpr.subExpr, aliasTableMap);
      if(rc!=RC::SUCCESS) return rc;
      break;
    case FUNC:
      for(size_t i = 0 ; i < expr->funcExpr.paraNum; ++i){
        rc = revertTableName(expr->funcExpr.exprList[i], aliasTableMap);
        if(rc!=RC::SUCCESS) return rc;
      }
      break;
    case FIE:
      if(expr->fieldExpression.table_name != nullptr){
        if(aliasTableMap.count(expr->fieldExpression.table_name) > 1){
          return RC::GENERIC_ERROR;
        }
        if(aliasTableMap.count(expr->fieldExpression.table_name) == 1){
          auto it = aliasTableMap.find(expr->fieldExpression.table_name);
          if(!it->second.empty()){
            std::string tableName = expr->fieldExpression.table_name;
            free(expr->fieldExpression.table_name);
            expr->fieldExpression.table_name = strdup(it->second.c_str());
          }
        }
      }
      break;
    default:
      return RC::SUCCESS;
  }
  return RC::SUCCESS;
}

bool checkAliasRepeat(std::multimap<std::string, std::string> aliasTableMap){
  for(auto it: aliasTableMap){
    if(it.first == ""){
      continue;
    }
    if(aliasTableMap.count(it.first) > 1){
      return false;
    }
  }
  return true;
}

RC SelectStmt::create(Db *db, const Selects &select_sql, Stmt *&stmt)
{
  RC rc;

  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;
  std::unordered_map<std::string, std::string> tableAliasMap;
  rc = constructTableMap(db, select_sql, table_map, tables, tableAliasMap);

  if(rc != SUCCESS) return rc;
  for(auto it : tableAliasMap){
    staticTableAliasMap_[it.first] = it.second;
  }


  std::multimap<std::string, std::string> aliasTableMap;
  for(auto it: staticTableAliasMap_){
    aliasTableMap.insert(std::make_pair(it.second, it.first));
  }
  // 检测 alias 同名
  // =========================================================================
//  if(!checkAliasRepeat(staticTableAliasMap_)){
//    return RC::ALIAS_REPEAT;
//  }

  std::set<std::string> fieldAlias;
  int fieldAliasCount = 0;
  for(size_t i = 0 ; i < select_sql.expr_num; ++i){
    if(select_sql.expressions[i]->alias != nullptr){
      ++ fieldAliasCount;
      fieldAlias.insert(select_sql.expressions[i]->alias);
    }
  }
  if(fieldAliasCount != fieldAlias.size()){
    return RC::ALIAS_REPEAT;
  }

  //======================================================================

  for(size_t i = 0; i < select_sql.expr_num; ++i){
    auto it = select_sql.expressions[i];
    rc = revertTableName(it, aliasTableMap);
    if(rc!=RC::SUCCESS) return rc;
  }

  for(size_t i = 0 ; i < select_sql.condition_num; ++i){
    auto it = select_sql.conditions;
    rc = revertTableName(it[i].leftExpr, aliasTableMap);
    if(rc!=RC::SUCCESS) return rc;
    rc = revertTableName(it[i].rightExpr, aliasTableMap);
    if(rc!=RC::SUCCESS) return rc;
  }

  for(size_t i = 0 ; i < select_sql.group_by_num; ++i){
    auto it = select_sql.group_by;
    rc = revertTableName(it[i], aliasTableMap);
    if(rc!=RC::SUCCESS) return rc;
  }

  for (size_t i = 0; i < select_sql.order_by_num; ++i) {
    auto it = select_sql.order[i].id;
    rc = revertTableName(it, aliasTableMap);
    if(rc!=RC::SUCCESS) return rc;
  }

  Table* defaultTable = nullptr;
  const char* defaultTableName = nullptr;
  if(tables.size() == 1){
    defaultTable = tables[0];
    defaultTableName = defaultTable->name();
  }

  std::vector<BaseExpression*> queryExpressions;
  std::vector<Expr*> leaf_expressions;

  for (int i = select_sql.expr_num - 1; i >= 0; i--){
    if(select_sql.expressions[i]->type == FIE ||strcmp(select_sql.expressions[i]->fieldExpression.attr_name,"*") == 0 ){
      leaf_expressions.emplace_back(select_sql.expressions[i]);
    }else{
      getExpressionLeafs(select_sql.expressions[i], leaf_expressions, defaultTableName);
    }
  }

  for (size_t i = 0; i < select_sql.condition_num; ++i) {
    getExpressionLeafs(select_sql.conditions[i].leftExpr, leaf_expressions, defaultTableName);
    getExpressionLeafs(select_sql.conditions[i].rightExpr, leaf_expressions, defaultTableName);
  }

  // collect query fields in `select` statement
  std::vector<Field> query_fields;
  for (size_t i = 0 ; i < leaf_expressions.size(); ++i) {
    const Expr* leaf = leaf_expressions[i];

    if(leaf->type == FIE){
      // 代表表达式是 * 或者 id.*
      const char* table_name = leaf->fieldExpression.table_name;
      const char* attr_name = leaf->fieldExpression.attr_name;

      if(common::is_blank(table_name) && 0 == strcmp(attr_name, "*"))
      {
        if(!checkAliasRepeat(aliasTableMap)){
          return RC::ALIAS_REPEAT;
        }
        for (Table *table : tables) {
          wildcard_fields(table, query_fields);
        }
      }else if(!common::is_blank(table_name))
      {
        if (0 == strcmp(table_name, "*")) {
          if (0 != strcmp(attr_name, "*")) {
            LOG_WARN("invalid field name while table is *. attr=%s", attr_name);
            return RC::SCHEMA_FIELD_MISSING;
          }
          if(!checkAliasRepeat(aliasTableMap)){
            return RC::ALIAS_REPEAT;
          }
          for (Table *table : tables) {
            wildcard_fields(table, query_fields);
          }
        } else {
          auto iter = table_map.find(table_name);
          Table *table;
          if (iter == table_map.end()) {
            table = db->find_table(table_name);
          }else{
            table = iter->second;
          }

          if (0 == strcmp(attr_name, "*")) {
            wildcard_fields(table, query_fields);
          } else {
            const FieldMeta *field_meta = table->table_meta().field(attr_name);
            if (nullptr == field_meta) {
              LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), attr_name);
              return RC::SCHEMA_FIELD_MISSING;
            }

            query_fields.emplace_back(table, field_meta);
          }
        }
      } else {
        if (tables.size() != 1) {
          LOG_WARN("invalid. I do not know the attr's table. attr=%s", attr_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = tables[0];
        const FieldMeta *field_meta = table->table_meta().field(attr_name);
        if (nullptr == field_meta) {
          LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), attr_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        query_fields.emplace_back(table, field_meta);
      }
    }
  }

  // update expression list
  for (int i = select_sql.expr_num - 1; i >= 0; i--){
    Expr* currentExpr = select_sql.expressions[i];
    if(currentExpr->type == FIE && 0 == strcmp(currentExpr->fieldExpression.attr_name, "*")){
      if(currentExpr->alias!=nullptr) {
        return RC::ERROR_ALIAS;
      }
      if(currentExpr->fieldExpression.table_name == nullptr
          || 0 == strcmp(currentExpr->fieldExpression.table_name,"*")){
        for(auto table: tables){
          expandStar(table, queryExpressions);
        }
      } else {
        std::string alias = currentExpr->fieldExpression.table_name;
        if(aliasTableMap.count(alias) != 0){
          if(aliasTableMap.count(alias) != 1){
            auto it = aliasTableMap.find(alias);
            alias = (*it).second;
          }else{
            return RC::GENERIC_ERROR;
          }
        }
        expandStar(table_map[alias], queryExpressions);
      }
    }else{
      BaseExpression* expression = constructExpression(currentExpr, defaultTable, &table_map, db);
      if(expression == nullptr){
        if(currentExpr->type == FIE){
          continue ;
        }else{
          return RC::MISMATCH;
        }
      }else{
        queryExpressions.emplace_back(expression);
      }
    }
  }

  // parse Inner Join
  std::vector<InnerJoin*> innerJoins;
  for(size_t i = select_sql.join_num; i > 0 ; --i ){
    InnerJoin* innerJoin;
    rc = creatInnerJoin(db, defaultTable, table_map, select_sql.joinNodes[i - 1], innerJoin);
    if(rc != SUCCESS){
      LOG_INFO("fail to init inner join");
      return rc;
    }
    innerJoins.emplace_back(innerJoin);
  }

  // parse group by clause
  std::vector<BaseExpression*> groupBy;
  for(int i = select_sql.group_by_num; i > 0 ; --i){
    Expr* currentExpr = select_sql.group_by[i - 1];
    BaseExpression* expression = constructExpression(currentExpr, defaultTable, &table_map, db);
    if(expression == nullptr){
      return RC::SCHEMA_FIELD_MISSING;
    }
    groupBy.emplace_back(expression);
  }

  // parse order by clause
  std::vector<std::pair<BaseExpression*, bool> > orderBy;
  for (size_t i = 0; i < select_sql.order_by_num; ++i) {
    Expr* currentExpr = select_sql.order[i].id;
    BaseExpression* expression = constructExpression(currentExpr, defaultTable, &table_map, db);
    if (expression == nullptr) {
      return RC::SCHEMA_FIELD_MISSING;
    }
    orderBy.emplace_back(std::pair<BaseExpression*, bool>(expression, select_sql.order[i].is_asc));
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), query_fields.size());

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `where` statement, including 'or' operator
  FilterStmt *filter_stmt = nullptr;
  rc = FilterStmt::create_tree(db, default_table, &table_map,
      select_sql.conditions, select_sql.condition_num, filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  FilterStmt *having_filter_stmt = nullptr;
  rc = FilterStmt::create(db, default_table, &table_map,
      select_sql.having, select_sql.having_num, having_filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct having filter stmt");
    return rc;
  }

  staticTableAliasMap_.clear();

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  select_stmt->expressions_ = queryExpressions;
  select_stmt->tables_.swap(tables);
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->groupBy_ = groupBy;
  select_stmt->inner_joins_ = innerJoins;
  select_stmt->having_filter_stmt_ = having_filter_stmt;
  select_stmt->orderBy_ = orderBy;
  select_stmt->tableAliasMap_ = tableAliasMap;
  stmt = select_stmt;
  return RC::SUCCESS;
}

bool containExpression(std::vector<BaseExpression*> list, BaseExpression* target){
  for(auto it: list){
    if(it->toString() == target->toString()){
      return true;
    }
  }
  return false;
}

bool SelectStmt::containAggregateFunction(){
  if(groupBy_.size() != 0){
    return true;
  }
  for(auto it: expressions_){
    if(it->containAggregate()){
      return true;
    }
  }
  return false;
}

RC SelectStmt::collectAggregate(std::vector<BaseExpression*>& collect){
  for(auto it: expressions_){
    if(it->isId() && !containExpression(groupBy_, it)){
      return RC::GENERIC_ERROR;
    }
    std::vector<BaseExpression*> exprCollect;
    exprCollect = it->collectAllAggregate();
    for(auto sit: exprCollect){
      if(!containExpression(collect, sit)){
        collect.emplace_back(sit);
      }
    }
  }

  // group by
  for(auto it: groupBy_){
    if(!containExpression(collect, it)){
      collect.emplace_back(new Identity(it));
    }
  }

  //having
  for(auto it: having_filter_stmt_->filter_units()){
    BaseExpression* left = it->left();
    BaseExpression* right = it->right();
    if(!containExpression(collect, left) && !left->isConst()){
      if(!left->isAggregateFunc()){
        if(!containExpression(groupBy_, left)){
          return RC::ERROR_LOGIC_PLAN;
        }
        collect.emplace_back(new Identity(left));
      }else{
        collect.emplace_back((AggregateFunction*) left);
      }
    }
    if(!containExpression(collect, right) && !right->isConst()){
      if(!right->isAggregateFunc()){
        if(!containExpression(groupBy_, right)){
          return RC::ERROR_LOGIC_PLAN;
        }
        collect.emplace_back(new Identity(right));
      }else{
        collect.emplace_back((AggregateFunction*) right);
      }
    }
  }

  for (auto it: orderBy_) {
    std::vector<BaseExpression*> exprCollect;
    exprCollect = it.first->collectAllAggregate();
    for (auto sit : exprCollect) {
      if (!containExpression(collect, sit)) {
        if (sit->isAggregateFunc()) {
          collect.emplace_back((AggregateFunction*) sit);
        } else {
          collect.emplace_back(new Identity(sit));
        }
      }
    }
  }
  return RC::SUCCESS;
}

std::vector<AggregateFunction*> expressionToAggregate(std::vector<BaseExpression*> vec){
  std::vector<AggregateFunction*> ret;
  for(auto it: vec){
    ret.emplace_back((AggregateFunction*) it);
  }
  return ret;
}

RC SelectStmt::generateOperatorFlow(QueryOperator* &queryOperator){

  // select without table
  if(inner_joins_.size() == 0){
    queryOperator = new SimpleSelect(expressions_);
    return RC::SUCCESS;
  }

  // 首先产生多个scan
  std::unordered_map<Table*, QueryOperator*> scanMap;
  for(size_t k = 0 ; k < tables_.size(); ++k){
    Table* table = tables_[k];
    const char* tableName = table->name();
    std::vector<Schema> schemas;

    int fieldNumber = table->table_meta().field_num();
    for(int i = table->table_meta().sys_field_num(); i < fieldNumber; i ++){
      const FieldMeta* fieldMeta = table->table_meta().field(i);
      auto expr = new IdentifierExpression(tableName, fieldMeta->name(), fieldMeta->type());
      schemas.emplace_back(expr);
    }

    ScanOperator* myScanOperator = new ScanOperator(scanOperators_[k], schemas, &relations_);
    myScanOperator->setMultiTable(false);
    scanMap[table] = myScanOperator;
  }

  std::vector<QueryOperator*> innerJoinList;
  for(auto it: inner_joins_){
    InnerJoin* head = it;
    while(head->left != nullptr){
      head = head->left;
    }
    // 只有一层不需要包装
    if(head->parent == nullptr){
      innerJoinList.emplace_back(scanMap[head->right]);
      continue;
    }
    QueryOperator* lastOperator = scanMap[head->right];
    while(head->parent != nullptr){
      head = head->parent;
      JoinOperator* aJoin = new JoinOperator();
      aJoin->addOperator(scanMap[head->right]);
      aJoin->addOperator(lastOperator);
      aJoin->setJoinFilter(head->filterStmt);
      lastOperator = aJoin;
    }

    innerJoinList.emplace_back(lastOperator);
  }

  QueryOperator* beforeTransform;
  if(innerJoinList.size() != 1){
    // 生成 innerJoin tree
    JoinOperator * joinOperator = new JoinOperator;
    for(auto it: innerJoinList){
      joinOperator->addOperator(it);
    }

    joinOperator->setMultiTable(tables_.size() > 1);
    joinOperator->setJoinFilter(filter_stmt_->getJoinFilterStmt());
    beforeTransform = joinOperator;
  }else{
    beforeTransform = innerJoinList[0];
  }

  beforeTransform = new PredicateOperator1(beforeTransform, filter_stmt_);

  TransformerAndProjectOperator* transformerAndProjectOperator;

  if(containAggregateFunction()){
    std::vector<BaseExpression*> transformExpression;
    std::vector<BaseExpression*> allAggregate;
    RC rc = collectAggregate(allAggregate);
    if(rc!=SUCCESS) return rc;

    for(auto it: allAggregate){
      auto expr = ((AggregateFunction*)it)->getSubExpression();
      if(!containExpression(transformExpression, expr)){
        transformExpression.emplace_back(expr);
      }
    }

    for(size_t i = 0 ; i < expressions_.size(); ++i){
      if(expressions_[i]->isId()){
        auto aggreId = new Identity(expressions_[i]);
        aggreId->setAlias(expressions_[i]->getAlias());
        expressions_[i] = aggreId;
      }
    }

    transformerAndProjectOperator = new TransformerAndProjectOperator(beforeTransform, transformExpression);
    AggregateOperator* aggregateOperator = new AggregateOperator(transformerAndProjectOperator, expressionToAggregate(allAggregate));
    aggregateOperator->setMultiTable(tables_.size() > 1);
    aggregateOperator->setGroupBy(groupBy_);
    queryOperator = aggregateOperator;
    if(having_filter_stmt_ != nullptr && ! having_filter_stmt_->filter_units().empty()){
      queryOperator = new PredicateOperator1(queryOperator, having_filter_stmt_);
    }
    if (!orderBy_.empty()) queryOperator = new OrderOperator(queryOperator, orderBy_);
    queryOperator = new TransformerAndProjectOperator(queryOperator, expressions_);
    queryOperator->setMultiTable(tables_.size()>1);
  }else{
    if(having_filter_stmt_ != nullptr && ! having_filter_stmt_->filter_units().empty()) return RC::GENERIC_ERROR;
    if (!orderBy_.empty()) beforeTransform = new OrderOperator(beforeTransform, orderBy_);
    queryOperator = new TransformerAndProjectOperator(beforeTransform, expressions_);
    queryOperator->setMultiTable(tables_.size() > 1);
  }
  return RC::SUCCESS;
}

void SelectStmt::addScanOperator(Operator* scan){
  scanOperators_.emplace_back(scan);
}