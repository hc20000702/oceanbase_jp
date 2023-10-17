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

#ifndef __OBSERVER_SQL_PARSER_PARSE_DEFS_H__
#define __OBSERVER_SQL_PARSER_PARSE_DEFS_H__

#include <stddef.h>
#include <stdbool.h>

#define MAX_NUM 20
#define MAX_REL_NAME 20
#define MAX_ATTR_NAME 20
#define MAX_ERROR_MESSAGE 20
#define MAX_DATA 50

// 先在这声明
typedef struct Expr Expr;
typedef struct InnerJoinNode InnerJoinNode;

//属性结构体
typedef struct {
  char *relation_name;   // relation name (may be NULL) 表名
  char *attribute_name;  // attribute name              属性名
} RelAttr;

typedef enum {
  EQUAL_TO,     //"="     0
  LESS_EQUAL,   //"<="    1
  NOT_EQUAL,    // <> !=  2
  LESS_THAN,    //"<"     3
  GREAT_EQUAL,  //">="    4
  GREAT_THAN,   //">"     5
  LIKE,         //"like"  6
  NOT_LIKE,     //not like7
  IS,           //"is"    8
  IS_NOT,       //is not  9
  IN,           //"in"    10
  NOT_IN,       //not in  11
  EXISTS,       //"exists"12
  NOT_EXISTS,   //        13
  AND_OP,
  OR_OP,
  NO_OP
} CompOp;

typedef enum {
  OP_ADD = 20,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,

} ExprOp;

//属性值类型
typedef enum
{
  UNDEFINED,
  CHARS,
  // TEXTS,
  INTS,
  FLOATS,
  DATES,
  RELATIONS, // nx1 or 1xn or 0xn list
  NULLS
} AttrType;

typedef enum ExpressionType {
  BINARY,
  UNARY,
  CONST,
  FUNC,
  FIE,
  LIST,
  SUBQUERY
} ExpressionType;

typedef struct BinaryExpr{
  Expr* left;
  Expr* right;
  ExprOp op;
} BinaryExpr;

typedef struct ConstExpr{
  AttrType type;
  char* data;
} ConstExpr;

typedef struct UnaryExpr{
  ExprOp op; // can be +,-,!
  Expr* subExpr;
} UnaryExpr;

typedef struct FieldExpression {
  char* table_name;
  char* attr_name;
} FieldExpression;

typedef struct FuncExpr {
  // TODO: think what are needed in function expression
  char* funcName;
  Expr** exprList;
  size_t paraNum;
} FuncExpr;

typedef struct _OrderExpression {
  bool is_asc;
  Expr *id;
} OrderExpression;

// use for * and ID.*
typedef struct FakeExpr{
  char* table_name;
} FakeExpr;

//属性值
typedef struct _Value {
  AttrType type;  // type of value
  void *data;     // value
} Value;

typedef struct ListExpr {
  size_t value_num;
  Value values[MAX_NUM];
} ListExpr;

typedef struct Selects Selects;
struct Expr {
  ExpressionType type;
  char* alias;
  bool isBracketed;
  union {
    BinaryExpr binaryExpr;
    UnaryExpr unaryExpr;
    ConstExpr constExpr;
    FieldExpression fieldExpression;
    FuncExpr funcExpr;
    ListExpr listExpr;
    Selects *selectExpr;
  };
};

typedef struct _Condition{
  Expr *leftExpr;   // left-hand side attribute
  CompOp comp;      // comparison operator
  Expr *rightExpr;  // right-hand side attribute if right_is_attr = TRUE 右边的属性
  int leftChild;
  int rightChild;
} Condition;

// struct of select
typedef struct Selects {
//  size_t attr_num;                // Length of attrs in Select clause
//  RelAttr attributes[MAX_NUM];    // attrs in Select clause
  // replace RelAttr with expr
  size_t expr_num;
  size_t join_num;                  // Length of relations in Fro clause
  size_t condition_num;             // Length of conditions in Where clause
  size_t group_by_num;
  size_t having_num;
  size_t order_by_num;
  Expr* expressions[MAX_NUM];
  InnerJoinNode* joinNodes[MAX_NUM];// relations in From clause
  Condition conditions[MAX_NUM];    // conditions in Where clause
  Expr* group_by[MAX_NUM];
  Condition having[MAX_NUM];
  OrderExpression order[MAX_NUM];
} Selects;

// struct of insert
typedef struct {
  char *relation_name;    // Relation to insert into
  size_t inserts_num;           // size of insert data
  size_t offset[MAX_NUM];       // e.g. [4, 8, 12, ...]
  Value values[MAX_NUM * MAX_NUM];  // values to insert
} Inserts;

// struct of delete
typedef struct {
  char *relation_name;            // Relation to delete from
  size_t condition_num;           // Length of conditions in Where clause
  Condition conditions[MAX_NUM];  // conditions in Where clause
} Deletes;

union UpdateValues {
  Value value;
  Selects selects;
};
// struct of update
typedef struct {
  char *relation_name;            // Relation to update
//  char *attribute_name;           // Attribute to update
//  Value value;                    // update value
  size_t condition_num;           // Length of conditions in Where clause
  Condition conditions[MAX_NUM];  // conditions in Where clause
//  Value values[MAX_NUM];
  union UpdateValues updateValues[MAX_NUM];
  int is_value[MAX_NUM];
  char *attributes[MAX_NUM];
  size_t attribute_num;
} Updates;

typedef struct {
  char *name;     // Attribute name
  AttrType type;  // Type of attribute
  size_t length;  // Length of attribute
  size_t nullable;// 1 for nullable
} AttrInfo;

// struct of craete_table
typedef struct {
  char *relation_name;           // Relation name
  size_t attribute_count;        // Length of attribute
  AttrInfo attributes[MAX_NUM];  // attributes
} CreateTable;

// struct of drop_table
typedef struct {
  char *relation_name;  // Relation name
} DropTable;

// struct of create_index
typedef struct {
  char *index_name;      // Index name
  char *relation_name;   // Relation name
  size_t attribute_num;
  char *attributes[MAX_NUM];  // Attribute name
  size_t is_unique;        // 1 for unique
} CreateIndex;

// struct of  drop_index
typedef struct {
  const char *index_name;  // Index name
} DropIndex;

typedef struct {
  const char *relation_name;
} DescTable;

typedef struct {
  const char *relation_name;
  const char *file_name;
} LoadData;

typedef struct {
  const char *relation_name;
} ShowIndex;

struct InnerJoinNode{
    InnerJoinNode* leftNode;
    char* alias;
    // 第一张表会记录在 rightTable内
    char* rightTable;
    size_t condition_num;
    Condition conditions[MAX_NUM];
};

union Queries {
  Selects selection;
  Inserts insertion;
  Deletes deletion;
  Updates update;
  CreateTable create_table;
  DropTable drop_table;
  CreateIndex create_index;
  DropIndex drop_index;
  DescTable desc_table;
  LoadData load_data;
  ShowIndex show_index;
  char *errors;
};



// 修改yacc中相关数字编码为宏定义
enum SqlCommandFlag {
  SCF_ERROR = 0,
  SCF_SELECT,
  SCF_INSERT,
  SCF_UPDATE,
  SCF_DELETE,
  SCF_CREATE_TABLE,
  SCF_DROP_TABLE,
  SCF_CREATE_INDEX,
  SCF_DROP_INDEX,
  SCF_SHOW_INDEX,
  SCF_SYNC,
  SCF_SHOW_TABLES,
  SCF_DESC_TABLE,
  SCF_BEGIN,
  SCF_COMMIT,
  SCF_CLOG_SYNC,
  SCF_ROLLBACK,
  SCF_LOAD_DATA,
  SCF_HELP,
  SCF_EXIT
};
// struct of flag and sql_struct
typedef struct Query {
  enum SqlCommandFlag flag;
  union Queries sstr;
} Query;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name);
void relation_attr_destroy(RelAttr *relation_attr);

void value_init_integer(Value *value, int v);
void value_init_float(Value *value, float v);
// CHAR和TEXT都在这，type赋值为CHARS，只是长度不同
void value_init_string(Value *value, const char *v);
void value_init_date(Value *value, const char *v);
void value_init_null(Value *value);
void value_destroy(Value *value);

void condition_init(Condition *condition, CompOp comp, Expr* left, Expr* right, int leftChild, int rightRchild);
void condition_destroy(Condition *condition);

void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length, size_t nullable);
void attr_info_destroy(AttrInfo *attr_info);

void order_init(OrderExpression *order, Expr *id, bool is_asc);
void order_destroy(OrderExpression *order);

void selects_init(Selects *selects, ...);
//void selects_append_attribute(Selects *selects, RelAttr *rel_attr);
void selects_append_expression(Selects *selects, Expr* expression, size_t *expr_num);
void selects_append_join_node(Selects *selects, InnerJoinNode* joinNode, size_t *join_num);
void selects_append_conditions(Selects *selects, Condition conditions[], size_t condition_num);
void selects_append_having(Selects *selects, Condition conditions[], size_t condition_num);
void selects_append_order(Selects *selects, OrderExpression orders[], size_t order_num);
void selects_destroy(Selects *selects);

void destroy_join_node(InnerJoinNode*);

void inserts_init(Inserts *inserts, const char *relation_name);
void inserts_add(Inserts *inserts, Value values[], size_t value_num);
void inserts_destroy(Inserts *inserts);

void deletes_init_relation(Deletes *deletes, const char *relation_name);
void deletes_set_conditions(Deletes *deletes, Condition conditions[], size_t condition_num);
void deletes_destroy(Deletes *deletes);

void updates_init(Updates *updates, const char *relation_name, Condition conditions[], size_t condition_num);
void updates_append_value_pair(Updates *updates, const char *attribute_name, Value value);
void updates_append_select_pair(Updates *updates, const char *attribute_name, const Selects *select);
void updates_destroy(Updates *updates);

void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info);
void create_table_init_name(CreateTable *create_table, const char *relation_name);
void create_table_destroy(CreateTable *create_table);

void drop_table_init(DropTable *drop_table, const char *relation_name);
void drop_table_destroy(DropTable *drop_table);


void create_index_init(
    CreateIndex *create_index, const char *index_name, const char *relation_name, const size_t is_unique);
void create_index_append_attribute(CreateIndex *create_index, const char *attr_name);

void create_index_destroy(CreateIndex *create_index);

void show_index_init(ShowIndex *showIndex, const char *relation_name);

void drop_index_init(DropIndex *drop_index, const char *index_name);
void drop_index_destroy(DropIndex *drop_index);

void desc_table_init(DescTable *desc_table, const char *relation_name);
void desc_table_destroy(DescTable *desc_table);

void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name);
void load_data_destroy(LoadData *load_data);

void query_init(Query *query);
Query *query_create();  // create and init
void query_reset(Query *query);
void query_destroy(Query *query);  // reset and delete

Expr* generate_binary_expr_ptr(Expr* left, Expr* right, ExprOp op);
Expr* generate_unary_expr_ptr(Expr* subExpr, ExprOp op);
Expr* generate_const_expr_ptr(char* data, AttrType attrType);
Expr* generate_id_expr_ptr(char* table_name, char* attr_name);
Expr* generate_func_expr_ptr(char* funcName, Expr** para_list, size_t para_num);
Expr* generate_list_expr_ptr(const Value values[], const size_t value_num);
Expr* generate_subquery_expr_ptr(const Selects *select);
void setBracketed(Expr* expr, bool flag);

void set_alias(Expr* expr, const char*);
void exprDestroy(Expr*);

InnerJoinNode* make_join_node(const char* rightTable,
                              Condition* conditions,
                              size_t condition_num);

void set_join_node_left(InnerJoinNode *node, InnerJoinNode* leftNode);
char* int2string(int value);
char* float2string(float value);
char* date2string(const char *value);

void select_append_group_by_element(Selects *selects, Expr* element, size_t *group_num);

Expr * expr_copy(const Expr* temp);
Selects *selects_copy(const Selects* temp);
Condition *condition_copy(const Condition* temp);
OrderExpression *order_copy(const OrderExpression *temp);
InnerJoinNode *joinnode_copy(const InnerJoinNode *node);
void JoinNodeSetAlias(InnerJoinNode* joinNode, const char* alias);

//Expr* generate_func_expr_ptr()

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __OBSERVER_SQL_PARSER_PARSE_DEFS_H__
