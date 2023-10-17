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

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "storage/common/db.h"
#include "storage/common/table.h"
#include "util/util.h"

InsertStmt::InsertStmt(Table *table, const Value *values, const size_t *offset, const size_t inserts_num)
  : table_ (table), values_(values), offset_(offset), inserts_num_(inserts_num)
{}

RC InsertStmt::create(Db *db, Inserts &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name;
  if (nullptr == db || nullptr == table_name || inserts.inserts_num <= 0) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, inserts_size=%d", 
             db, table_name, inserts.inserts_num);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  const TableMeta &table_meta = table->table_meta();
  Value *values = inserts.values;
  const size_t *offset = inserts.offset;
  const size_t inserts_num = inserts.inserts_num;
  // check the fields number
  const size_t field_num = table_meta.field_num() - table_meta.sys_field_num();
  for (size_t k = 0; k < inserts_num; k++) {
    int last = 0;
    if (k > 0) {
      last = offset[k - 1];
    }
    if (field_num != offset[k] - last) {
      LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", offset[k] - last, field_num);
      return RC::SCHEMA_FIELD_MISSING;
    }
  }
  for (size_t k = 0; k < inserts_num; k++) {
    // check fields type
    const int sys_field_num = table_meta.sys_field_num();
    int left = 0;
    if (k > 0) {
      left = offset[k - 1];
    }
    for (size_t i = left; i < offset[k]; i++) {
      const FieldMeta *field_meta = table_meta.field(i + sys_field_num - left);
      const AttrType field_type = field_meta->type();
      const AttrType value_type = values[i].type;
      if (field_type != value_type) {
        if (field_type == CHARS && (value_type == INTS || value_type == FLOATS)) {
          void *p = values[i].data;
          if (value_type == INTS) {
            snprintf((char*)values[i].data, field_meta->len(), "%d", *(int*)p);
          } else {
            snprintf((char*)values[i].data, field_meta->len(), "%f", *(float*)p);
          }
          values[i].type = CHARS;
        } else if (field_type == INTS && (value_type == FLOATS || value_type == CHARS)) {
          void *p = values[i].data;
          int new_value;
          if (value_type == FLOATS) {
            new_value = int(*(float *)p + 0.5);
          } else {
            new_value = (int)char2double((char *)p);
          }
          values[i].type = INTS;
          memcpy(values[i].data, &new_value, sizeof(int));
        } else if (field_type == FLOATS && (value_type == INTS || value_type == CHARS)) {
          void *p = values[i].data;
          float new_value;
          if (value_type == INTS) {
            new_value = *(int *)p;
          } else {
            new_value = char2double((char *)p);
          }
          values[i].type = FLOATS;
          memcpy(values[i].data, &new_value, sizeof(float));
        } else if (value_type == NULLS && !(field_meta->nullable())) {
          LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d", 
                table_name, field_meta->name(), field_type, value_type);
          return RC::SCHEMA_FIELD_TYPE_MISMATCH;
        }
      }
    }
  }

  // everything alright
  stmt = new InsertStmt(table, values, offset, inserts_num);
  return RC::SUCCESS;
}
