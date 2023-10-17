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
#include "common/log/log.h"
#include "sql/stmt/update_stmt.h"
#include "storage/common/db.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/operator/table_scan_operator.h"
#include "util/util.h"

RC UpdateStmt::create(Db *db, Updates &update, Stmt *&stmt)
{
  const char *table_name = update.relation_name;
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument. db=%p, table_name=%p",
        db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db, table, &table_map,
      update.conditions, update.condition_num, filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  std::vector<Value*> values;
  std::vector<const char*> attributes;
  std::vector<QueryOperator> query_values;
  for (size_t i = 0; i < update.attribute_num; i++) {
    UpdateValues *value = update.updateValues + i;
    const char *attribute = update.attributes[i];
    const int is_value = update.is_value[i];
    if (is_value) {
      values.emplace_back(&value->value);
    } else {  // do select
      SelectStmt *stmt = nullptr;
      rc = SelectStmt::create(db, value->selects, reinterpret_cast<Stmt *&>(stmt));
      if (rc != RC::SUCCESS) {
        return rc;
      }
      for(Table* t : stmt->tables()){
//        Operator *scan_oper = t->try_to_create_index_scan_operator(t, stmt->filter_stmt(), stmt->query_fields());
          Operator *scan_oper = new TableScanOperator(t);
//        }
        stmt->addScanOperator(scan_oper);
      }
      QueryOperator* queryOperator;
      rc = stmt->generateOperatorFlow(queryOperator);
      if (rc != RC::SUCCESS) {
        return rc;
      }

      Relation *relation;
      rc = queryOperator->evaluate(relation);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      Value *result = new Value;
      if (relation->getLength() == 0) {
        result->type = NULLS;
        result->data = nullptr;
      } else if (relation->getLength() != 1 || relation->getWidth() != 1) {
        delete result;
        return RC::RECORD_INVALIDRECSIZE;
      } else {
        Item *item = relation->getItem(0, 0);
        AttrType item_type = relation->getSchema(0).attrType();
        if (item->isNull()) {
          result->type = NULLS;
          result->data = nullptr;
          item_type = NULLS;
        }
        switch (item_type) {
          case INTS:
            value_init_integer(result, item->getInt());
            break;
          case DATES: {
            value_init_integer(result, item->getInt());
            result->type = DATES;
          } break;
          case FLOATS:
            value_init_float(result, item->getFloat());
            break;
          case CHARS:
            value_init_string(result, item->getText());
            break;
          case NULLS:
          default:
            break;
        }
      }
      values.emplace_back(result);
      rc = queryOperator->close();
      if(rc!=RC::SUCCESS) return rc;
    }
    attributes.emplace_back(attribute);
  }
  UpdateStmt *updateStmt = new UpdateStmt();
  updateStmt->value_amount_ = values.size();
  updateStmt->values_.swap(values);
  updateStmt->attribute_names_.swap(attributes);
  updateStmt->table_ = table;
  updateStmt->filter_stmt_ = filter_stmt;
  stmt = updateStmt;
//  update.values
//  const Value *values = &(update.value);
//  const char * attribute_name = update.attribute_name;
//  stmt = new UpdateStmt(table, values, 1, attribute_name, filter_stmt);
  return rc;

}


