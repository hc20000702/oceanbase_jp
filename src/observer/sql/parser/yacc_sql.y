
%{

#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContextI {
  Query * ssql;
  size_t value_length;
  Value values[MAX_NUM];
  size_t condition_length;
  Condition conditions[MAX_NUM];
  size_t comp_length;
  CompOp comps[MAX_NUM];
  size_t exprop_length;
  ExprOp exprops[MAX_NUM];
  char id[MAX_NUM];
  // 解析函数参数列表
  Expr* para_list[MAX_NUM];
  size_t para_num;
  size_t expr_num;
  size_t join_num;
  size_t group_num;
  size_t having_num;
  Condition havingConditions[MAX_NUM];
  size_t order_num;
  OrderExpression orders[MAX_NUM];

  Condition onClauseStack[MAX_NUM][MAX_NUM];
  int onClauseConditionNum[MAX_NUM];
  int stackDeepth;

} ParserContextI;

typedef struct ParserContext {
    ParserContextI pc[MAX_NUM];
    int cur;
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址，最长4096（含\0）*/
{
    if (n2 - n1 + 2 > 4096) {
        n2 = 4096 + n1 - 1;
    }
    char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
    int i, j = 0;
    for (i = n1; i <= n2; i++) {
        sp[j++] = s[i];
    }
    sp[j] = 0;
    return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
    ParserContext *context = (ParserContext *)(yyget_extra(scanner));
    query_reset(context->pc[0].ssql);
    context->cur = 0;
    context->pc[0].ssql->flag = SCF_ERROR;
    context->pc[0].value_length = 0;
    context->pc[0].condition_length = 0;
    context->pc[0].comp_length = 0;
    context->pc[0].exprop_length = 0;
    context->pc[0].para_num = 0;
    context->pc[0].expr_num = 0;
    context->pc[0].join_num = 0;
    context->pc[0].group_num = 0;
    context->pc[0].having_num = 0;
    context->pc[0].order_num = 0;
    context->pc[0].ssql->sstr.insertion.inserts_num = 0;
    printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
    return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)

%}

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        UNIQUE
        INDEX
        SELECT
        GROUP
        ORDER
        BY
        ASC
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        TEXT_T
        FLOAT_T
        NULL_T
        NULL_V
        DATE_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        SET
        ON
        LOAD
        DATA
        INFILE
        EQ
        LT
        GT
        LE
        GE
        NE
        AND
        OR
        IN_OP
        EXISTS_OP
        IS_OP
        NOT
        LIKE_OP
        AS
        INNER
        JOIN
        HAVING

%union {
  struct _Attr *attr;
//   struct _Condition *condition1;
  struct _Value *value1;
  struct Expr *expr;
  char *string;
  int number;
  float floats;
  char *position;
  struct InnerJoinNode* joinNode;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
/* %token <string> PATH */
%token <string> SSS
%token <string> STAR
/* %token <string> STRING_V */
%token <exprOp> ADD     SUB     DIV
%token <string> TIME
//非终结符
%type <joinNode> inner_join_list
%type <expr> expression function    sub_select
%type <expr> summand    factor      unary       term;
%type <number> type     is_nullable is_asc
/* %type <condition1> condition; */
%type <value1> value
%type <number> number   condition   or_condition  and_condition
%type <exprOp> comOp    summandOp  factorOp   unaryOp   in_not_in

%%

commands:        //commands or sqls. parser starts here.
    /* empty */
    | commands command
    ;

command:
      select
    | insert
    | update
    | delete
    | create_table
    | drop_table
    | show_tables
    | desc_table
    | create_index
    | drop_index
    | show_index
    | sync
    | begin
    | commit
    | rollback
    | load_data
    | help
    | exit
    ;

exit:
    EXIT SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_EXIT;//"exit";
    };

help:
    HELP SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_HELP;//"help";
    };

sync:
    SYNC SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SYNC;
    }
    ;

begin:
    TRX_BEGIN SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_BEGIN;
    }
    ;

commit:
    TRX_COMMIT SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_COMMIT;
    }
    ;

rollback:
    TRX_ROLLBACK SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_ROLLBACK;
    }
    ;

drop_table:         /*drop table 语句的语法解析树*/
    DROP TABLE ID SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.drop_table, $3);
    };

show_tables:
    SHOW TABLES SEMICOLON {
      CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SHOW_TABLES;
    }
    ;

desc_table:
    DESC ID SEMICOLON {
      CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.desc_table, $2);
    }
    ;

create_index:       /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID id_list RBRACE SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_CREATE_INDEX;//"create_index";
        create_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, $3, $5, 0);
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, $7);
    }
    | CREATE UNIQUE INDEX ID ON ID LBRACE ID id_list RBRACE SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_CREATE_INDEX;//"create_index";
        create_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, $4, $6, 1);
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, $8);
    }
    ;
id_list:
    /* empty */
    | COMMA ID id_list {
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, $2);
    }
    ;

drop_index:         /*drop index 语句的语法解析树*/
    DROP INDEX ID  SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_DROP_INDEX;//"drop_index";
        drop_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.drop_index, $3);
    }
    ;

show_index:         /*show index 语句的语法解析树*/
    SHOW INDEX FROM ID SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SHOW_INDEX;
        show_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.show_index, $4);
    }
    ;

create_table:       /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_CREATE_TABLE;//"create_table";
        create_table_init_name(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, $3);
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length = 0;
    }
    ;
attr_def_list:
    /* empty */
    | COMMA attr_def attr_def_list {    }
    ;

attr_def:
    ID_get type LBRACE number RBRACE is_nullable {
        AttrInfo attribute;
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, $2, $4, $6);
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
    | ID_get type is_nullable {
        AttrInfo attribute;
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, $2, 4, $3);
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
    | ID_get TEXT_T is_nullable {
        AttrInfo attribute;
        // TODO: try 4096?
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, CHARS, 4097, $3);
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T { $$=INTS; }
    | STRING_T { $$=CHARS; }
    /* | TEXT_T { $$ = TEXT; } */
    | FLOAT_T { $$=FLOATS; }
    | DATE_T { $$ = DATES; }
    ;
is_nullable:
    /* empty */  { $$ = 0; }
    | NOT NULL_V { $$ = 0; }
    | NULL_T     { $$ = 1; }
    ;
ID_get:
    ID {
        char *temp=$1;
        snprintf(CONTEXT->pc[CONTEXT->cur].id, sizeof(CONTEXT->pc[CONTEXT->cur].id), "%s", temp);
    }
    ;

insert:         /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE insert_list SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_INSERT;//"insert";
        inserts_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, $3);
        // 从前往后插，不是逆序
        inserts_add(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
    }
insert_list:
    /* empty */
    | insert_comma LBRACE value value_list RBRACE insert_list { }
    ;
insert_comma:
    COMMA {
        inserts_add(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
    }
value_list:
    /* empty */
    | COMMA value value_list  {
        // CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++] = *$2;
    }
    ;
value:
    SUB NUMBER{
        value_init_integer(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], -$2);
    }
    | SUB FLOAT{
        value_init_float(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], -$2);
    }
    | NUMBER {
        value_init_integer(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], $1);
    }
    |FLOAT {
        value_init_float(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], $1);
    }
    |SSS {
        $1 = substr($1,1,strlen($1)-2);
        value_init_string(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], $1);
    }
    | TIME {
        value_init_date(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], $1);
    }
    | NULL_V {
        value_init_null(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++]);
    }
    ;

delete:        /*  delete 语句的语法解析树*/
    DELETE FROM ID where SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DELETE;//"delete";
        deletes_init_relation(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.deletion, $3);
        deletes_set_conditions(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.deletion,
            CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
    ;
update:         /*  update 语句的语法解析树*/
    UPDATE ID SET id_value_pair id_value_pair_list where SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_UPDATE;//"update";
        updates_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.update, $2, CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
        // 虽然大多数都没用，但一股脑清零总没错
        CONTEXT->pc[CONTEXT->cur].value_length = 0;
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
        CONTEXT->pc[CONTEXT->cur].comp_length = 0;
        CONTEXT->pc[CONTEXT->cur].exprop_length = 0;
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        CONTEXT->pc[CONTEXT->cur].expr_num = 0;
        CONTEXT->pc[CONTEXT->cur].join_num = 0;
        CONTEXT->pc[CONTEXT->cur].group_num = 0;
        CONTEXT->pc[CONTEXT->cur].having_num = 0;
        CONTEXT->pc[CONTEXT->cur].order_num = 0;
    }
    ;
id_value_pair_list :
    | COMMA id_value_pair id_value_pair_list {

    }
    ;
id_value_pair:
    ID EQ value {
        Value value = CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length-1];
        updates_append_value_pair(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.update, $1, value);
    }
    | ID EQ sub_select {
        // NOTE: pc[0], or pc[CONTEXT->cur - 1]
        updates_append_select_pair(&CONTEXT->pc[0].ssql->sstr.update, $1, &CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
    }
    ;
group_by:
    /* empty */ { CONTEXT->pc[CONTEXT->cur].group_num = 0; }
    | GROUP BY ID group_by_list{
        Expr* expr = generate_id_expr_ptr(NULL, $3);
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
    | GROUP BY ID DOT ID group_by_list{
        Expr* expr = generate_id_expr_ptr($3, $5);
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    };

group_by_list:
    | COMMA ID group_by_list{
        Expr* expr = generate_id_expr_ptr(NULL, $2);
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
    | COMMA ID DOT ID group_by_list{
        Expr* expr = generate_id_expr_ptr($2, $4);
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
    ;
select:         /*  select 语句的语法解析树*/
    SELECT select_attr fromClause SEMICOLON{
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_SELECT;//"select";
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length = 0;
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
        CONTEXT->pc[CONTEXT->cur].comp_length = 0;
        CONTEXT->pc[CONTEXT->cur].exprop_length = 0;
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        CONTEXT->pc[CONTEXT->cur].expr_num = 0;
        CONTEXT->pc[CONTEXT->cur].join_num = 0;
        CONTEXT->pc[CONTEXT->cur].group_num = 0;
        CONTEXT->pc[CONTEXT->cur].having_num = 0;
        CONTEXT->pc[CONTEXT->cur].order_num = 0;
    }
    ;
sub_select:         /*  select 子查询*/
    LBRACE select_flag select_attr fromClause RBRACE  {
        // 每次调用sub_select都需要cur--, selects_destroy
    }
    ;
select_flag:
    SELECT {
        CONTEXT->cur++;
        memset(&CONTEXT->pc[CONTEXT->cur], 0, sizeof(ParserContextI));
        CONTEXT->pc[CONTEXT->cur].ssql = query_create();
    }
    ;
fromClause:
    /* empty */ {
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.expr_num = CONTEXT->pc[CONTEXT->cur].expr_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.join_num = CONTEXT->pc[CONTEXT->cur].join_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.condition_num = CONTEXT->pc[CONTEXT->cur].condition_length;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.group_by_num = CONTEXT->pc[CONTEXT->cur].group_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.having_num = CONTEXT->pc[CONTEXT->cur].having_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.order_by_num = CONTEXT->pc[CONTEXT->cur].order_num;
    }
    | FROM join join_list where_tree group_by havingClause order_by {
        selects_append_conditions(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
        selects_append_having(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].havingConditions, CONTEXT->pc[CONTEXT->cur].having_num);
        selects_append_order(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].orders, CONTEXT->pc[CONTEXT->cur].order_num);
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.expr_num = CONTEXT->pc[CONTEXT->cur].expr_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.join_num = CONTEXT->pc[CONTEXT->cur].join_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.group_by_num = CONTEXT->pc[CONTEXT->cur].group_num;
    }
    ;

havingClause:
    /*empty*/ { CONTEXT->pc[CONTEXT->cur].having_num = 0; }
    | HAVING having_condition having_condition_list{

    };

having_condition:
    function comOp expression{
        // TODO: need or operator?
        Condition condition;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], $1, $3, -1, -1);
        CONTEXT->pc[CONTEXT->cur].havingConditions[CONTEXT->pc[CONTEXT->cur].having_num++] = condition;
    };

having_condition_list:
    | AND having_condition having_condition_list{

    };

order_by:
    /* empty */ { CONTEXT->pc[CONTEXT->cur].order_num = 0; }
    | ORDER BY order_condition order_condition_list {
        
    }
    ;
order_condition:
    term is_asc {
        OrderExpression order;
        order_init(&order, $1, $2);
        CONTEXT->pc[CONTEXT->cur].orders[CONTEXT->pc[CONTEXT->cur].order_num++] = order;
    }
    ;
order_condition_list:
    /* empty */
    | COMMA order_condition order_condition_list {

    }
    ;
is_asc:
    /* empty */ { $$ = 1; }
    | ASC       { $$ = 1; }
    | DESC      { $$ = 0; }
    ;
as_or_empty:
    /* empty */
    | AS
    ;
select_attr:
    STAR attr_list{
        Expr* starExpr = generate_id_expr_ptr(NULL, "*");
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, starExpr, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
    |expression as_or_empty ID attr_list {
        set_alias($1, $3);
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $1, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
    | expression attr_list {
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $1, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }

    ;
attr_list:
    /* empty */
    | COMMA expression as_or_empty ID attr_list {
        set_alias($2, $4);
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $2, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
    | COMMA expression attr_list {
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $2, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
    ;

join:
    ID inner_join_list{
        InnerJoinNode* startNode = make_join_node($1, CONTEXT->pc[CONTEXT->cur].conditions, 0);
        if($2 != NULL){
            set_join_node_left($2, startNode);
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $2, &CONTEXT->pc[CONTEXT->cur].join_num);
        }else{
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, startNode, &CONTEXT->pc[CONTEXT->cur].join_num);
        }
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
    | ID as_or_empty ID inner_join_list{
        InnerJoinNode* startNode = make_join_node($1, CONTEXT->pc[CONTEXT->cur].conditions, 0);
        JoinNodeSetAlias(startNode, $3);
        if($4 != NULL){
            set_join_node_left($4, startNode);
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, $4, &CONTEXT->pc[CONTEXT->cur].join_num);
        }else{
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, startNode, &CONTEXT->pc[CONTEXT->cur].join_num);
        }
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
    ;

join_list:
    /* empty */
    | COMMA join join_list{
        // nothing to do
    };

inner_join_list:
    /*empty*/{
        $$ = NULL;
    }
    | INNER JOIN ID on_clause inner_join_list{
        CONTEXT->pc[CONTEXT->cur].stackDeepth --;
        InnerJoinNode* node = make_join_node($3, CONTEXT->pc[CONTEXT->cur].onClauseStack[CONTEXT->pc[CONTEXT->cur].stackDeepth], CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth]);
        CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth] = 0;
        if($5 != NULL){
            set_join_node_left($5, node);
            $$ = $5;
        }else{
            $$ = node;
        }
    };

on_clause:
    // empty
    | ON condition condition_list{
        // do something!!!!
        for(size_t i = 0 ; i < CONTEXT->pc[CONTEXT->cur].condition_length; ++i){
            CONTEXT->pc[CONTEXT->cur].onClauseStack[CONTEXT->pc[CONTEXT->cur].stackDeepth][i] = CONTEXT->pc[CONTEXT->cur].conditions[i];
        }
        CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth++] = CONTEXT->pc[CONTEXT->cur].condition_length;
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    };

where_tree:
    /* empty */ { CONTEXT->pc[CONTEXT->cur].condition_length = 0; }
    | WHERE or_condition {
        // NOTE: 根节点是最后一个，只要从根节点开始遍历
    };
or_condition:
    and_condition {
        $$ = $1;
    } 
    | or_condition OR and_condition {
        Condition condition;
        condition_init(&condition, OR_OP, NULL, NULL, $1, $3);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    ;
and_condition:
    condition {
        $$ = $1;
    }
    | and_condition AND condition {
        Condition condition;
        condition_init(&condition, AND_OP, NULL, NULL, $1, $3);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    ;
where:
    /* empty */ { CONTEXT->pc[CONTEXT->cur].condition_length = 0; }
    | WHERE condition condition_list {
        // nothing to do
    };
condition:
    expression comOp expression{
        Condition condition;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], $1, $3, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    | expression in_not_in sub_select {
        Condition condition;
        Expr* select_expr = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], $1, select_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    | expression in_not_in LBRACE value value_list RBRACE {
        // in和comOp不放在一起防冲突
        Condition condition;
        Expr* list_expr = generate_list_expr_ptr(CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], $1, list_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    | exists_not_exists sub_select {
        Condition condition;
        Expr* select_expr = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], NULL, select_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        $$ = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
    ;

condition_list :
    | AND condition condition_list {
        // nothing to do
    }
    ;

expression:
    summand {
        $$ = $1;
    }
    | sub_select {
        $$ = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
    }
    ;

summand:
    factor {
        $$ = $1;
    }
    | summand summandOp factor {
        $$ = generate_binary_expr_ptr($1, $3, CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
    ;
factor:
    unary {
        $$ = $1;
    }
    | factor factorOp unary {
        $$ = generate_binary_expr_ptr($1, $3, CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
    ;
unary:
    term {
        $$ = $1;
    }
    | unaryOp unary {
        $$ = generate_unary_expr_ptr($2, CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
    ;
term:
    LBRACE expression RBRACE {
        setBracketed($2, true);
        $$ = $2;
    }
    | function {
        $$ = $1;
    }
    | NUMBER {
        char * literal = int2string($1);
        $$ = generate_const_expr_ptr(literal, INTS);
    }
    | FLOAT {
        char* literal = float2string($1);
        $$ = generate_const_expr_ptr(literal, FLOATS);
    }
    | TIME {
        char* literal = date2string($1);
        $$ = generate_const_expr_ptr(literal, DATES);
    }
    | SSS {
        $1 = substr($1,1,strlen($1)-2);
        $$ = generate_const_expr_ptr($1, CHARS);
    }
    | NULL_V {
        $$ = generate_const_expr_ptr(NULL, NULLS);
    }
    | ID {
        $$ = generate_id_expr_ptr(NULL, $1);
    }
    | ID DOT ID {
        $$ = generate_id_expr_ptr($1, $3);
    }
    | ID DOT STAR{
        $$ = generate_id_expr_ptr($1, "*");
    }
    ;

function:
    ID LBRACE expression func_para_list RBRACE{
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = $3;
        Expr* para = generate_func_expr_ptr($1, CONTEXT->pc[CONTEXT->cur].para_list, CONTEXT->pc[CONTEXT->cur].para_num);
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        $$ = para;
    }
    | ID LBRACE STAR RBRACE{
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = $$ = generate_id_expr_ptr(NULL, "*");
        Expr* para = generate_func_expr_ptr($1, CONTEXT->pc[CONTEXT->cur].para_list, CONTEXT->pc[CONTEXT->cur].para_num);
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        $$ = para;
    };

func_para_list:
    /* empty */
    | COMMA expression func_para_list{
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = $2;
    };

summandOp:
      ADD { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_ADD; }
    | SUB { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_SUB; }
    ;
factorOp:
     STAR { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_MUL; }
    | DIV { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_DIV; }
    ;
unaryOp:
      ADD { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_ADD; }
    | SUB { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_SUB; }
    ;

comOp:
      EQ { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = EQUAL_TO; }
    | LT { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LESS_THAN; }
    | GT { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = GREAT_THAN; }
    | LE { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LESS_EQUAL; }
    | GE { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = GREAT_EQUAL; }
    | NE { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_EQUAL; }
    | LIKE_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LIKE; }
    | NOT LIKE_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_LIKE; }
    | IS_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IS; }
    | IS_OP NOT { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IS_NOT; }
in_not_in:
      IN_OP     { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IN; }
    | NOT IN_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_IN; }
    ;
exists_not_exists:
      EXISTS_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = EXISTS; }
    | NOT EXISTS_OP { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_EXISTS; }
    ;

load_data:
    LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_LOAD_DATA;
        load_data_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.load_data, $7, $4);
    }
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

// 后续对sqls内容解析
int sql_parse(const char *s, Query *sqls) {
    ParserContext context;
    memset(&context, 0, sizeof(context));

    yyscan_t scanner;
    yylex_init_extra(&context, &scanner);
    context.pc[0].ssql = sqls;
    scan_string(s, scanner);
    int result = yyparse(scanner);
    yylex_destroy(scanner);
    return result;
}
