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
// Created by Meiyi 
//

#include <mutex>
#include "sql/parser/parse.h"
#include "rc.h"
#include "common/log/log.h"

RC parse(char *st, Query *sqln);

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name)
{
  if (relation_name != nullptr) {
    relation_attr->relation_name = strdup(relation_name);
  } else {
    relation_attr->relation_name = nullptr;
  }
  relation_attr->attribute_name = strdup(attribute_name);
}

void relation_attr_destroy(RelAttr *relation_attr)
{
  free(relation_attr->relation_name);
  free(relation_attr->attribute_name);
  relation_attr->relation_name = nullptr;
  relation_attr->attribute_name = nullptr;
}

void value_init_integer(Value *value, int v)
{
  value->type = INTS;
  value->data = malloc(sizeof(v));
  memcpy(value->data, &v, sizeof(v));
}
void value_init_float(Value *value, float v)
{
  value->type = FLOATS;
  value->data = malloc(sizeof(v));
  memcpy(value->data, &v, sizeof(v));
}
void value_init_string(Value *value, const char *v)
{
  value->type = CHARS;
  value->data = strdup(v);
}

void value_init_null(Value *value) {
  value->type = NULLS;
  value->data = nullptr;
}

void value_init_date(Value *value, const char *v){
  value->type = DATES;
  int date = atoi(date2string(v));
  value->data = malloc(sizeof(date));
  memcpy(value->data, &date, sizeof(date));

}
void value_destroy(Value *value)
{
  value->type = UNDEFINED;
  if (value->data) {
    free(value->data);
    value->data = nullptr;
  }
}

void condition_init(Condition *condition, CompOp comp, Expr* left, Expr* right, int leftChild, int rightChild)
{
  condition->comp = comp;
  condition->leftExpr = left;
  condition->rightExpr = right;
  condition->leftChild = leftChild;
  condition->rightChild = rightChild;
}

void condition_destroy(Condition *condition)
{
  exprDestroy(condition->leftExpr);
  exprDestroy(condition->rightExpr);
}

void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length, size_t nullable)
{
  attr_info->name = strdup(name);
  attr_info->type = type;
  attr_info->length = length;
  attr_info->nullable = nullable;
}
void attr_info_destroy(AttrInfo *attr_info)
{
  free(attr_info->name);
  attr_info->name = nullptr;
}

void order_init(OrderExpression *order, Expr *id, bool is_asc)
{
  order->id = id;
  order->is_asc = is_asc;
}
void order_destroy(OrderExpression *order)
{
  free(order->id);
  order->is_asc = true;
}

void selects_init(Selects *selects, ...);
//void selects_append_attribute(Selects *selects, RelAttr *rel_attr)
//{
//  selects->attributes[selects->attr_num++] = *rel_attr;
//}

void selects_append_expression(Selects *selects, Expr* expression, size_t *expr_num)
{
  selects->expressions[*expr_num] = expression;
  *expr_num += 1;
  selects->expr_num = *expr_num;
}

void selects_append_join_node(Selects *selects, InnerJoinNode* joinNode, size_t *join_num)
{
  selects->joinNodes[*join_num] = joinNode;
  *join_num += 1;
  selects->join_num = *join_num;
}

void selects_append_conditions(Selects *selects, Condition conditions[], size_t condition_num)
{
  assert(condition_num <= sizeof(selects->conditions) / sizeof(selects->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    selects->conditions[i] = conditions[i];
  }
  selects->condition_num = condition_num;
}

void selects_append_having(Selects *selects, Condition conditions[], size_t having_num){
  assert(having_num <= sizeof(selects->having) / sizeof(selects->having[0]));
  for (size_t i = 0; i < having_num; i++) {
    selects->having[i] = conditions[i];
  }
  selects->having_num = having_num;
}

void selects_append_order(Selects *selects, OrderExpression orders[], size_t order_num){
  assert(order_num <= sizeof(selects->order) / sizeof(selects->order[0]));
  for (size_t i = 0; i < order_num; i++) {
    selects->order[i] = orders[i];
  }
  selects->order_by_num = order_num;
}

void selects_destroy(Selects *selects)
{
  for (size_t i = 0; i < selects->expr_num; i++) {
    exprDestroy(selects->expressions[i]);
  }
  selects->expr_num = 0;

  for (size_t i = 0; i < selects->join_num; i++) {
    destroy_join_node(selects->joinNodes[i]);
    selects->joinNodes[i] = nullptr;
  }
  selects->join_num = 0;

  for (size_t i = 0; i < selects->condition_num; i++) {
    condition_destroy(&selects->conditions[i]);
  }
  selects->condition_num = 0;

  // TODO: free error, group_by_num so big
  for(size_t i = 0 ; i < selects->group_by_num; ++i){
    exprDestroy(selects->group_by[i]);
  }
  selects->group_by_num = 0;
  for (size_t i = 0; i < selects->having_num; ++i) {
    condition_destroy(&selects->having[i]);
  }
  selects->having_num = 0;
  for (size_t i = 0; i < selects->order_by_num; ++i) {
    order_destroy(&selects->order[i]);
  }
  selects->order_by_num = 0;
}

void destroy_join_node(InnerJoinNode* node){
  if(node->leftNode != nullptr){
    destroy_join_node(node->leftNode);
  }
  if(node->rightTable){
    free(node->rightTable);
  }
  if(node->alias){
//    free(node->alias);
    node->alias = nullptr;
  }
  free(node);
}

void inserts_init(Inserts *inserts, const char *relation_name)
{
  inserts->relation_name = strdup(relation_name);
}
void inserts_add(Inserts *inserts, Value values[], size_t value_num)
{
  size_t line = inserts->inserts_num;
  size_t offset = 0;
  if (line > 0 ) {
    offset = inserts->offset[line - 1];
  }
  assert(offset + value_num <= sizeof(inserts->values) / sizeof(inserts->values[0]));

  for (size_t i = 0; i < value_num; i++) {
    inserts->values[i + offset] = values[i];
  }
  inserts->offset[line] = offset + value_num;
  inserts->inserts_num++;
}
void inserts_destroy(Inserts *inserts)
{
  free(inserts->relation_name);
  inserts->relation_name = nullptr;

  size_t offset = inserts->offset[inserts->inserts_num - 1];
  for (size_t i = 0; i < offset; i++) {
    value_destroy(&inserts->values[i]);
  }
  for (size_t i = 0; i < inserts->inserts_num; i++) {
    inserts->offset[i] = 0;
  }
  inserts->inserts_num = 0;
}

void deletes_init_relation(Deletes *deletes, const char *relation_name)
{
  deletes->relation_name = strdup(relation_name);
}

void deletes_set_conditions(Deletes *deletes, Condition conditions[], size_t condition_num)
{
  assert(condition_num <= sizeof(deletes->conditions) / sizeof(deletes->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    deletes->conditions[i] = conditions[i];
  }
  deletes->condition_num = condition_num;
}
void deletes_destroy(Deletes *deletes)
{
  for (size_t i = 0; i < deletes->condition_num; i++) {
    condition_destroy(&deletes->conditions[i]);
  }
  deletes->condition_num = 0;
  free(deletes->relation_name);
  deletes->relation_name = nullptr;
}

void updates_init(Updates *updates, const char *relation_name, Condition conditions[], size_t condition_num)
{
  updates->relation_name = strdup(relation_name);

  assert(condition_num <= sizeof(updates->conditions) / sizeof(updates->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    updates->conditions[i] = conditions[i];
  }
  updates->condition_num = condition_num;
}

void updates_append_value_pair(Updates *updates, const char *attribute_name, Value value) {
  updates->attributes[updates->attribute_num] = strdup(attribute_name);
//  updates->values[updates->attribute_num] = value;
  updates->updateValues[updates->attribute_num].value = value;
  updates->is_value[updates->attribute_num] = 1;
  updates->attribute_num ++;
}

void updates_append_select_pair(Updates *updates, const char *attribute_name, const Selects *select) {
  updates->attributes[updates->attribute_num] = strdup(attribute_name);
  updates->updateValues[updates->attribute_num].selects = *selects_copy(select);
  updates->is_value[updates->attribute_num] = 0;
  updates->attribute_num ++;
}

void updates_destroy(Updates *updates)
{
  free(updates->relation_name);
  for (size_t i = 0; i < updates->attribute_num; i++) {
    if (updates->is_value[i]) {
      value_destroy(&updates->updateValues[i].value);
    } else {
      selects_destroy(&updates->updateValues[i].selects);
    }
//    value_destroy(&updates->values[i]);
    free(updates->attributes[i]);
    updates->attributes[i] = nullptr;
    updates->is_value[i] = 0;
  }
  updates->attribute_num = 0;

  updates->relation_name = nullptr;

  for (size_t i = 0; i < updates->condition_num; i++) {
    condition_destroy(&updates->conditions[i]);
  }
  updates->condition_num = 0;
}

void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info)
{
  create_table->attributes[create_table->attribute_count++] = *attr_info;
}

void create_table_init_name(CreateTable *create_table, const char *relation_name)
{
  create_table->relation_name = strdup(relation_name);
}

void create_table_destroy(CreateTable *create_table)
{
  for (size_t i = 0; i < create_table->attribute_count; i++) {
    attr_info_destroy(&create_table->attributes[i]);
  }
  create_table->attribute_count = 0;
  free(create_table->relation_name);
  create_table->relation_name = nullptr;
}

void drop_table_init(DropTable *drop_table, const char *relation_name)
{
  drop_table->relation_name = strdup(relation_name);
}

void drop_table_destroy(DropTable *drop_table)
{
  free(drop_table->relation_name);
  drop_table->relation_name = nullptr;
}

void create_index_init(
    CreateIndex *create_index, const char *index_name, const char *relation_name, const size_t is_unique)
{
  create_index->index_name = strdup(index_name);
  create_index->relation_name = strdup(relation_name);
  create_index->is_unique = is_unique;
}

void create_index_append_attribute(CreateIndex *create_index, const char *attr_name)
{
  create_index->attributes[create_index->attribute_num++] = strdup(attr_name);

}

void create_index_destroy(CreateIndex *create_index)
{
  free(create_index->index_name);
  free(create_index->relation_name);

  create_index->index_name = nullptr;
  create_index->relation_name = nullptr;


  for (size_t i = 0; i < create_index->attribute_num; i++) {
    free(create_index->attributes[i]);
    create_index->attributes[i] = nullptr;
  }
  create_index->attribute_num = 0;
  create_index->is_unique = 0;

}

void show_index_init(ShowIndex *showIndex, const char *relation_name) {
  showIndex->relation_name = strdup(relation_name);
}

void drop_index_init(DropIndex *drop_index, const char *index_name)
{
  drop_index->index_name = strdup(index_name);
}

void drop_index_destroy(DropIndex *drop_index)
{
  free((char *)drop_index->index_name);
  drop_index->index_name = nullptr;
}

void desc_table_init(DescTable *desc_table, const char *relation_name)
{
  desc_table->relation_name = strdup(relation_name);
}

void desc_table_destroy(DescTable *desc_table)
{
  free((char *)desc_table->relation_name);
  desc_table->relation_name = nullptr;
}

void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name)
{
  load_data->relation_name = strdup(relation_name);

  if (file_name[0] == '\'' || file_name[0] == '\"') {
    file_name++;
  }
  char *dup_file_name = strdup(file_name);
  int len = strlen(dup_file_name);
  if (dup_file_name[len - 1] == '\'' || dup_file_name[len - 1] == '\"') {
    dup_file_name[len - 1] = 0;
  }
  load_data->file_name = dup_file_name;
}

void load_data_destroy(LoadData *load_data)
{
  free((char *)load_data->relation_name);
  free((char *)load_data->file_name);
  load_data->relation_name = nullptr;
  load_data->file_name = nullptr;
}

void query_init(Query *query)
{
  query->flag = SCF_ERROR;
  memset(&query->sstr, 0, sizeof(query->sstr));
}

Query *query_create()
{
  Query *query = (Query *)malloc(sizeof(Query));
  if (nullptr == query) {
    LOG_ERROR("Failed to alloc memroy for query. size=%ld", sizeof(Query));
    return nullptr;
  }

  query_init(query);
  return query;
}

void query_reset(Query *query)
{
  switch (query->flag) {
    case SCF_SELECT: {
      selects_destroy(&query->sstr.selection);
    } break;
    case SCF_INSERT: {
      inserts_destroy(&query->sstr.insertion);
    } break;
    case SCF_DELETE: {
      deletes_destroy(&query->sstr.deletion);
    } break;
    case SCF_UPDATE: {
      updates_destroy(&query->sstr.update);
    } break;
    case SCF_CREATE_TABLE: {
      create_table_destroy(&query->sstr.create_table);
    } break;
    case SCF_DROP_TABLE: {
      drop_table_destroy(&query->sstr.drop_table);
    } break;
    case SCF_CREATE_INDEX: {
      create_index_destroy(&query->sstr.create_index);
    } break;
    case SCF_DROP_INDEX: {
      drop_index_destroy(&query->sstr.drop_index);
    } break;
    case SCF_SYNC: {

    } break;
    case SCF_SHOW_INDEX: // no reset
    case SCF_SHOW_TABLES:
      break;

    case SCF_DESC_TABLE: {
      desc_table_destroy(&query->sstr.desc_table);
    } break;

    case SCF_LOAD_DATA: {
      load_data_destroy(&query->sstr.load_data);
    } break;
    case SCF_CLOG_SYNC:
    case SCF_BEGIN:
    case SCF_COMMIT:
    case SCF_ROLLBACK:
    case SCF_HELP:
    case SCF_EXIT:
    case SCF_ERROR:
      break;
  }
}

void query_destroy(Query *query)
{
  query_reset(query);
  free(query);
}

Expr* generate_binary_expr_ptr(Expr* left, Expr* right, ExprOp op){
  Expr* binary = (Expr*) malloc(sizeof(Expr));
  binary->isBracketed = false;
  binary->type = BINARY;
  binary->binaryExpr.left = left;
  binary->binaryExpr.right = right;
  binary->binaryExpr.op = op;
  binary->alias = nullptr;
  return binary;
}

Expr* generate_unary_expr_ptr(Expr* subExpr, ExprOp op){
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->isBracketed = false;
    expr->type = UNARY;
    expr->unaryExpr.subExpr = subExpr;
    expr->unaryExpr.op = op;
    expr->alias = nullptr;
    return expr;
}

Expr* generate_const_expr_ptr(char* data, AttrType attrType){
  Expr* expr = (Expr*)malloc(sizeof(Expr));
  expr->isBracketed = false;
  expr->type = CONST;
  expr->constExpr.data = data;
  expr->constExpr.type = attrType;
  expr->alias = nullptr;
  return expr;
}

Expr* generate_id_expr_ptr(char* table_name, char* attr_name){
  Expr* expr = (Expr*)malloc(sizeof(Expr));
  expr->isBracketed = false;
  expr->type = FIE;
  expr->fieldExpression.table_name = table_name;
  expr->fieldExpression.attr_name = attr_name;
  expr->alias = nullptr;
  return expr;
}

Expr* generate_func_expr_ptr(char* funcName, Expr** para_list, size_t para_num){
  Expr* expr = (Expr*)malloc(sizeof(Expr));
  expr->isBracketed = false;
  expr->type = FUNC;
  expr->funcExpr.funcName = funcName;
  expr->funcExpr.paraNum = para_num;
  expr->funcExpr.exprList = new Expr*[para_num];
  expr->alias = nullptr;
  for(int i = para_num - 1 ; i >= 0; --i ){
    expr->funcExpr.exprList[para_num - 1 - i] = para_list[i];
  }
  return expr;
}

Expr* generate_list_expr_ptr(const Value values[], const size_t value_num) {
  Expr* expr = (Expr*)malloc(sizeof(Expr));
  expr->isBracketed = true;
  expr->type = LIST;
  expr->alias = nullptr;
  for (size_t i = 0; i < value_num; i++) {
    expr->listExpr.values[i] = values[i];
  }
  expr->listExpr.value_num = value_num;
  return expr;
}

Expr* generate_subquery_expr_ptr(const Selects *select) {
  Expr* expr = (Expr*)malloc(sizeof(Expr));
  expr->isBracketed = true;
  expr->type = SUBQUERY;
  expr->alias = nullptr;
  expr->selectExpr = selects_copy(select);
  return expr;
}

void setBracketed(Expr* expr, bool flag){
  expr->isBracketed = flag;
}

void set_alias(Expr* expr, const char* alias){
  expr->alias = strdup(alias);
}

void exprDestroy(Expr* expr){
  if (!expr) {
    return;
  }
  if(expr->alias){
    free(expr->alias);
  }
  switch(expr->type){
    case BINARY:
      exprDestroy(expr->binaryExpr.left);
      exprDestroy(expr->binaryExpr.right);
      break;
    case UNARY:
      exprDestroy(expr->unaryExpr.subExpr);
      break;
    case FUNC:
      break;
    case CONST:
    case FIE:
      break;
    case LIST:
      for (size_t i = 0; i < expr->listExpr.value_num; i++) {
        value_destroy(&expr->listExpr.values[i]);
      }
      break;
    case SUBQUERY:
      selects_destroy(expr->selectExpr);
      break;
    default:
      break ;
  }
  free(expr);
}

InnerJoinNode* make_join_node(const char* rightTable,
                              Condition* conditions,
                              size_t condition_num){
  InnerJoinNode * ret = (InnerJoinNode*) malloc(sizeof(InnerJoinNode));
  ret->rightTable = strdup(rightTable);
  ret->leftNode = nullptr;
  ret->alias = nullptr;
  for(size_t i = 0 ; i < condition_num; ++i){
    ret->conditions[i] = conditions[i];
  }
  ret->condition_num = condition_num;
  return ret;
}

void set_join_node_left(InnerJoinNode *node, InnerJoinNode* leftNode){

  while(node->leftNode != nullptr){
    node = node->leftNode;
  }
  node->leftNode = leftNode;
}

char* int2string(int value){
  char* literal = (char*)malloc(20*sizeof(int));
  snprintf(literal, 19, "%d", value);
  return literal;
}

char* float2string(float value){
  char* literal = (char*)malloc(20*sizeof(int));
  snprintf(literal, 19, "%f", value);
  return literal;
}

char* date2string(const char *v){
  char *str1 = new char[strlen(v) + 1];
  strncpy(str1, v, strlen(v) + 1);
  const char *delim = "-\'";
  char *split_str;
  split_str = strtok(str1, delim);
  std::string year = split_str;
  std::string month = strtok(NULL, delim);
  if (month.length() < 2) {
    month = "0" + month;
  }
  std::string day = strtok(NULL, delim);
  if (day.length() < 2) {
    day = "0" + day;
  }
  char *date = new char[9];
  strcpy(date, (year + month + day).c_str());
  return date;
}

void select_append_group_by_element(Selects *selects, Expr* element, size_t *group_num){
  selects->group_by[*group_num] = element;
  *group_num += 1;
  selects->group_by_num = *group_num;
}

Expr * expr_copy(const Expr* temp) {
  if (!temp) {
    return nullptr;
  }
  Expr *expr = nullptr;;
  switch (temp->type) {
    case BINARY:
      expr = generate_binary_expr_ptr(temp->binaryExpr.left, temp->binaryExpr.right, temp->binaryExpr.op);
      break ;
    case UNARY:
      expr = generate_unary_expr_ptr(temp->unaryExpr.subExpr, temp->unaryExpr.op);
      break ;
    case CONST:
      expr = generate_const_expr_ptr(temp->constExpr.data, temp->constExpr.type);
      break ;
    case FUNC:
      expr = generate_func_expr_ptr(temp->funcExpr.funcName, temp->funcExpr.exprList, temp->funcExpr.paraNum);
      break ;
    case FIE:
      expr = generate_id_expr_ptr(temp->fieldExpression.table_name, temp->fieldExpression.attr_name);
      break ;
    case LIST:
      expr = generate_list_expr_ptr(temp->listExpr.values, temp->listExpr.value_num);
      break;
    case SUBQUERY:
      expr = generate_subquery_expr_ptr(temp->selectExpr);
      break;
    default:
      return nullptr;
      break ;
  }
  if (temp->alias != nullptr) {
    expr->alias = strdup(temp->alias);
  }
  expr->isBracketed = temp->isBracketed;
  return expr;
}

Selects *selects_copy(const Selects* temp) {
  if (!temp) {
    return nullptr;
  }
  Selects *selects = new Selects;
  selects->expr_num = temp->expr_num;
  for (size_t i = 0; i < temp->expr_num; i++) {
    selects->expressions[i] = expr_copy(temp->expressions[i]);
  }
  selects->join_num = temp->join_num;
  for (size_t i = 0; i < temp->join_num; i++) {
    selects->joinNodes[i] = joinnode_copy(temp->joinNodes[i]);
  }
  selects->condition_num = temp->condition_num;
  for (size_t i = 0; i < temp->condition_num; i++) {
    selects->conditions[i] = *condition_copy(&temp->conditions[i]);
  }
  selects->group_by_num = temp->group_by_num;
  for (size_t i = 0; i < temp->group_by_num; i++) {
    selects->group_by[i] = expr_copy(temp->group_by[i]);
  }
  selects->having_num = temp->having_num;
  for (size_t i = 0; i < temp->having_num; i++) {
    selects->having[i] = *condition_copy(&temp->having[i]);
  }
  selects->order_by_num = temp->order_by_num;
  for (size_t i = 0; i < temp->order_by_num; i++) {
    selects->order[i] = *order_copy(&temp->order[i]);
  }
  return selects;
}

Condition *condition_copy(const Condition* temp) {
  if (!temp) {
    return nullptr;
  }
  Condition *condition = new Condition;
  condition->leftExpr = expr_copy(temp->leftExpr);
  condition->rightExpr = expr_copy(temp->rightExpr);
  condition->comp = temp->comp;
  condition->leftChild = temp->leftChild;
  condition->rightChild = temp->rightChild;
  return condition;
}
OrderExpression *order_copy(const OrderExpression *temp) {
  if (!temp) {
    return nullptr;
  }
  OrderExpression *order = new OrderExpression;
  order->is_asc = temp->is_asc;
  order->id = expr_copy(temp->id);
  return order;
}
InnerJoinNode *joinnode_copy(const InnerJoinNode *node) {
  if (!node) {
    return nullptr;
  }
  InnerJoinNode *ret = (InnerJoinNode *)malloc(sizeof(InnerJoinNode));
  if (node->leftNode) {
    ret->leftNode = joinnode_copy(node->leftNode);
  } else {
    ret->leftNode = nullptr;
  }
  ret->rightTable = strdup(node->rightTable);
  ret->condition_num = node->condition_num;
  ret->alias = node->alias;
  for (size_t i = 0; i < ret->condition_num; i++) {
    ret->conditions[i] = node->conditions[i];
  }
  return ret;
}

void JoinNodeSetAlias(InnerJoinNode* joinNode, const char* alias){
  joinNode->alias = strdup(alias);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

////////////////////////////////////////////////////////////////////////////////

extern "C" int sql_parse(const char *st, Query *sqls);

RC parse(const char *st, Query *sqln)
{
  sql_parse(st, sqln);

  if (sqln->flag == SCF_ERROR)
    return SQL_SYNTAX;
  else
    return SUCCESS;
}
