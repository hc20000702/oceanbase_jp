//
// Created by 吴小宁 on 10/21/22.
//

#include "update_operator.h"
#include "QueryOperator.h"
#include "sql/stmt/select_stmt.h"
#include "util/util.h"

RC UpdateOperator::open()
{
  if (children_.size() != 1) {
    LOG_WARN("update operator must has 1 child");
    return RC::INTERNAL;
  }

  Operator *child = children_[0];
  RC rc = child->open();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  Table *table = update_stmt_->table();
  const TableMeta &table_meta = table->table_meta();
  Record new_record;
  char *data = (char *)malloc(table_meta.record_size());
  new_record.set_data(data);
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      free(data);
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &record = row_tuple->record();
    new_record.set_rid(record.rid());
    memcpy(data, record.data(), table_meta.record_size());
    // memset(data, 0, table_meta.record_size());
    const FieldMeta *field_meta_null = table_meta.null_field();
    int32_t null_map = *(int32_t *)(record.data() + field_meta_null->offset());

    for (int i = 0; i < update_stmt_->value_amount(); i++) {
      // 获得 field_i 和 field
      int field_i;
      const char *field_name = update_stmt_->attribute_names()[i];
      if (nullptr == field_name) {
        LOG_WARN("There is no such field attribute=%s in the table table=%s",
            update_stmt_->attribute_names()[i],
            table_meta.name());
        free(data);
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      const FieldMeta *field = table_meta.field_and_index(field_name, field_i);
      if (field == nullptr) {
        LOG_WARN("There is no such field attribute=%s in the table table=%s",
            update_stmt_->attribute_names()[i],
            table_meta.name());
        free(data);
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }

      Value *value = update_stmt_->values()[i];
      if (value->type == NULLS) {
        if (field->nullable()) {
          null_map |= (1 << (31 - field_i));
        } else {
          LOG_ERROR("Invalid null value. ");
          free(data);
          return RC::SCHEMA_FIELD_NOT_NULL;
        }
      } else {
        // 类型转换
        if (field->type() != value->type) {
          rc = type_cast(update_stmt_->values()[i], field);
          if (rc != RC::SUCCESS) {
            free(data);
            return rc;
          }
        }
        null_map &= ~(1 << (31 - field_i));
        if (value->type == CHARS) {
          memcpy(new_record.data() + field->offset(), value->data, strlen((char*)value->data)+1);
        } else {
          memcpy(new_record.data() + field->offset(), value->data, field->len());
        }
      }
    }
    memcpy(new_record.data() + field_meta_null->offset(), &null_map, field_meta_null->len());
    if (!table->is_unique_key(&new_record)) {
      free(data);
      return RC::RECORD_DUPLICATE_KEY;
    }
    memcpy(record.data() + field_meta_null->offset(), &null_map, field_meta_null->len());
    for (int i = 0; i < update_stmt_->value_amount(); i++) {
      const FieldMeta *field = table_meta.field(update_stmt_->attribute_names()[i]);
      if (update_stmt_->values()[i]->type == NULLS) {
        continue;
      } else if (update_stmt_->values()[i]->type == CHARS) {
        memcpy(record.data() + field->offset(), update_stmt_->values()[i]->data, strlen((char*)update_stmt_->values()[i]->data)+1);
      } else {
        memcpy(record.data() + field->offset(), update_stmt_->values()[i]->data, field->len());
      }
    }
    rc = table->update_record(trx_, &record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to update record: %s", strrc(rc));
      free(data);
      return rc;
    }
  }
  free(data);
  return RC::SUCCESS;
}

RC UpdateOperator::next()
{
  return RC::RECORD_EOF;
}

RC UpdateOperator::close()
{
  children_[0]->close();
  return RC::SUCCESS;
}
