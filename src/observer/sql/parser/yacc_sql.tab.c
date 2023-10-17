/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


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


#line 159 "yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_UNIQUE = 8,                     /* UNIQUE  */
  YYSYMBOL_INDEX = 9,                      /* INDEX  */
  YYSYMBOL_SELECT = 10,                    /* SELECT  */
  YYSYMBOL_GROUP = 11,                     /* GROUP  */
  YYSYMBOL_ORDER = 12,                     /* ORDER  */
  YYSYMBOL_BY = 13,                        /* BY  */
  YYSYMBOL_ASC = 14,                       /* ASC  */
  YYSYMBOL_DESC = 15,                      /* DESC  */
  YYSYMBOL_SHOW = 16,                      /* SHOW  */
  YYSYMBOL_SYNC = 17,                      /* SYNC  */
  YYSYMBOL_INSERT = 18,                    /* INSERT  */
  YYSYMBOL_DELETE = 19,                    /* DELETE  */
  YYSYMBOL_UPDATE = 20,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 21,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 22,                    /* RBRACE  */
  YYSYMBOL_COMMA = 23,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 24,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 25,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 26,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 27,                     /* INT_T  */
  YYSYMBOL_STRING_T = 28,                  /* STRING_T  */
  YYSYMBOL_TEXT_T = 29,                    /* TEXT_T  */
  YYSYMBOL_FLOAT_T = 30,                   /* FLOAT_T  */
  YYSYMBOL_NULL_T = 31,                    /* NULL_T  */
  YYSYMBOL_NULL_V = 32,                    /* NULL_V  */
  YYSYMBOL_DATE_T = 33,                    /* DATE_T  */
  YYSYMBOL_HELP = 34,                      /* HELP  */
  YYSYMBOL_EXIT = 35,                      /* EXIT  */
  YYSYMBOL_DOT = 36,                       /* DOT  */
  YYSYMBOL_INTO = 37,                      /* INTO  */
  YYSYMBOL_VALUES = 38,                    /* VALUES  */
  YYSYMBOL_FROM = 39,                      /* FROM  */
  YYSYMBOL_WHERE = 40,                     /* WHERE  */
  YYSYMBOL_SET = 41,                       /* SET  */
  YYSYMBOL_ON = 42,                        /* ON  */
  YYSYMBOL_LOAD = 43,                      /* LOAD  */
  YYSYMBOL_DATA = 44,                      /* DATA  */
  YYSYMBOL_INFILE = 45,                    /* INFILE  */
  YYSYMBOL_EQ = 46,                        /* EQ  */
  YYSYMBOL_LT = 47,                        /* LT  */
  YYSYMBOL_GT = 48,                        /* GT  */
  YYSYMBOL_LE = 49,                        /* LE  */
  YYSYMBOL_GE = 50,                        /* GE  */
  YYSYMBOL_NE = 51,                        /* NE  */
  YYSYMBOL_AND = 52,                       /* AND  */
  YYSYMBOL_OR = 53,                        /* OR  */
  YYSYMBOL_IN_OP = 54,                     /* IN_OP  */
  YYSYMBOL_EXISTS_OP = 55,                 /* EXISTS_OP  */
  YYSYMBOL_IS_OP = 56,                     /* IS_OP  */
  YYSYMBOL_NOT = 57,                       /* NOT  */
  YYSYMBOL_LIKE_OP = 58,                   /* LIKE_OP  */
  YYSYMBOL_AS = 59,                        /* AS  */
  YYSYMBOL_INNER = 60,                     /* INNER  */
  YYSYMBOL_JOIN = 61,                      /* JOIN  */
  YYSYMBOL_HAVING = 62,                    /* HAVING  */
  YYSYMBOL_NUMBER = 63,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 64,                     /* FLOAT  */
  YYSYMBOL_ID = 65,                        /* ID  */
  YYSYMBOL_SSS = 66,                       /* SSS  */
  YYSYMBOL_STAR = 67,                      /* STAR  */
  YYSYMBOL_ADD = 68,                       /* ADD  */
  YYSYMBOL_SUB = 69,                       /* SUB  */
  YYSYMBOL_DIV = 70,                       /* DIV  */
  YYSYMBOL_TIME = 71,                      /* TIME  */
  YYSYMBOL_YYACCEPT = 72,                  /* $accept  */
  YYSYMBOL_commands = 73,                  /* commands  */
  YYSYMBOL_command = 74,                   /* command  */
  YYSYMBOL_exit = 75,                      /* exit  */
  YYSYMBOL_help = 76,                      /* help  */
  YYSYMBOL_sync = 77,                      /* sync  */
  YYSYMBOL_begin = 78,                     /* begin  */
  YYSYMBOL_commit = 79,                    /* commit  */
  YYSYMBOL_rollback = 80,                  /* rollback  */
  YYSYMBOL_drop_table = 81,                /* drop_table  */
  YYSYMBOL_show_tables = 82,               /* show_tables  */
  YYSYMBOL_desc_table = 83,                /* desc_table  */
  YYSYMBOL_create_index = 84,              /* create_index  */
  YYSYMBOL_id_list = 85,                   /* id_list  */
  YYSYMBOL_drop_index = 86,                /* drop_index  */
  YYSYMBOL_show_index = 87,                /* show_index  */
  YYSYMBOL_create_table = 88,              /* create_table  */
  YYSYMBOL_attr_def_list = 89,             /* attr_def_list  */
  YYSYMBOL_attr_def = 90,                  /* attr_def  */
  YYSYMBOL_number = 91,                    /* number  */
  YYSYMBOL_type = 92,                      /* type  */
  YYSYMBOL_is_nullable = 93,               /* is_nullable  */
  YYSYMBOL_ID_get = 94,                    /* ID_get  */
  YYSYMBOL_insert = 95,                    /* insert  */
  YYSYMBOL_insert_list = 96,               /* insert_list  */
  YYSYMBOL_insert_comma = 97,              /* insert_comma  */
  YYSYMBOL_value_list = 98,                /* value_list  */
  YYSYMBOL_value = 99,                     /* value  */
  YYSYMBOL_delete = 100,                   /* delete  */
  YYSYMBOL_update = 101,                   /* update  */
  YYSYMBOL_id_value_pair_list = 102,       /* id_value_pair_list  */
  YYSYMBOL_id_value_pair = 103,            /* id_value_pair  */
  YYSYMBOL_group_by = 104,                 /* group_by  */
  YYSYMBOL_group_by_list = 105,            /* group_by_list  */
  YYSYMBOL_select = 106,                   /* select  */
  YYSYMBOL_sub_select = 107,               /* sub_select  */
  YYSYMBOL_select_flag = 108,              /* select_flag  */
  YYSYMBOL_fromClause = 109,               /* fromClause  */
  YYSYMBOL_havingClause = 110,             /* havingClause  */
  YYSYMBOL_having_condition = 111,         /* having_condition  */
  YYSYMBOL_having_condition_list = 112,    /* having_condition_list  */
  YYSYMBOL_order_by = 113,                 /* order_by  */
  YYSYMBOL_order_condition = 114,          /* order_condition  */
  YYSYMBOL_order_condition_list = 115,     /* order_condition_list  */
  YYSYMBOL_is_asc = 116,                   /* is_asc  */
  YYSYMBOL_as_or_empty = 117,              /* as_or_empty  */
  YYSYMBOL_select_attr = 118,              /* select_attr  */
  YYSYMBOL_attr_list = 119,                /* attr_list  */
  YYSYMBOL_join = 120,                     /* join  */
  YYSYMBOL_join_list = 121,                /* join_list  */
  YYSYMBOL_inner_join_list = 122,          /* inner_join_list  */
  YYSYMBOL_on_clause = 123,                /* on_clause  */
  YYSYMBOL_where_tree = 124,               /* where_tree  */
  YYSYMBOL_or_condition = 125,             /* or_condition  */
  YYSYMBOL_and_condition = 126,            /* and_condition  */
  YYSYMBOL_where = 127,                    /* where  */
  YYSYMBOL_condition = 128,                /* condition  */
  YYSYMBOL_condition_list = 129,           /* condition_list  */
  YYSYMBOL_expression = 130,               /* expression  */
  YYSYMBOL_summand = 131,                  /* summand  */
  YYSYMBOL_factor = 132,                   /* factor  */
  YYSYMBOL_unary = 133,                    /* unary  */
  YYSYMBOL_term = 134,                     /* term  */
  YYSYMBOL_function = 135,                 /* function  */
  YYSYMBOL_func_para_list = 136,           /* func_para_list  */
  YYSYMBOL_summandOp = 137,                /* summandOp  */
  YYSYMBOL_factorOp = 138,                 /* factorOp  */
  YYSYMBOL_unaryOp = 139,                  /* unaryOp  */
  YYSYMBOL_comOp = 140,                    /* comOp  */
  YYSYMBOL_in_not_in = 141,                /* in_not_in  */
  YYSYMBOL_exists_not_exists = 142,        /* exists_not_exists  */
  YYSYMBOL_load_data = 143                 /* load_data  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   340

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  167
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  320

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   326


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   189,   189,   191,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   216,   221,   226,   232,   238,   244,   250,   256,
     262,   269,   274,   280,   282,   288,   295,   302,   309,   311,
     315,   321,   327,   336,   339,   340,   342,   343,   346,   347,
     348,   351,   358,   366,   368,   371,   376,   378,   383,   386,
     389,   392,   395,   399,   402,   408,   417,   433,   434,   439,
     443,   451,   452,   456,   461,   462,   466,   472,   488,   493,
     500,   508,   519,   520,   525,   532,   533,   538,   539,   544,
     550,   552,   557,   558,   559,   561,   563,   566,   570,   574,
     579,   581,   585,   591,   601,   614,   616,   621,   624,   636,
     638,   648,   649,   653,   656,   664,   667,   675,   676,   680,
     686,   695,   705,   716,   717,   723,   726,   734,   737,   742,
     745,   750,   753,   758,   762,   765,   769,   773,   777,   781,
     784,   787,   790,   796,   802,   809,   811,   816,   817,   820,
     821,   824,   825,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   840,   841,   844,   845,   849
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "UNIQUE", "INDEX", "SELECT", "GROUP", "ORDER",
  "BY", "ASC", "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE",
  "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK",
  "INT_T", "STRING_T", "TEXT_T", "FLOAT_T", "NULL_T", "NULL_V", "DATE_T",
  "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "SET", "ON",
  "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE", "GE", "NE", "AND",
  "OR", "IN_OP", "EXISTS_OP", "IS_OP", "NOT", "LIKE_OP", "AS", "INNER",
  "JOIN", "HAVING", "NUMBER", "FLOAT", "ID", "SSS", "STAR", "ADD", "SUB",
  "DIV", "TIME", "$accept", "commands", "command", "exit", "help", "sync",
  "begin", "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "id_list", "drop_index", "show_index", "create_table",
  "attr_def_list", "attr_def", "number", "type", "is_nullable", "ID_get",
  "insert", "insert_list", "insert_comma", "value_list", "value", "delete",
  "update", "id_value_pair_list", "id_value_pair", "group_by",
  "group_by_list", "select", "sub_select", "select_flag", "fromClause",
  "havingClause", "having_condition", "having_condition_list", "order_by",
  "order_condition", "order_condition_list", "is_asc", "as_or_empty",
  "select_attr", "attr_list", "join", "join_list", "inner_join_list",
  "on_clause", "where_tree", "or_condition", "and_condition", "where",
  "condition", "condition_list", "expression", "summand", "factor",
  "unary", "term", "function", "func_para_list", "summandOp", "factorOp",
  "unaryOp", "comOp", "in_not_in", "exists_not_exists", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326
};
#endif

#define YYPACT_NINF (-272)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-96)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -272,    15,  -272,    88,   100,    52,    22,   103,    11,    74,
      97,    94,    49,   124,   159,   161,   178,   138,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,   122,   176,   123,
     125,   127,    -4,  -272,  -272,  -272,    50,  -272,   170,  -272,
    -272,  -272,  -272,   156,    13,   -45,    73,  -272,  -272,  -272,
      92,   197,   198,   164,  -272,   137,   140,   167,  -272,  -272,
    -272,  -272,  -272,   165,   190,   148,   179,   219,   221,  -272,
      52,   212,    82,    77,   105,  -272,   171,   232,  -272,   172,
    -272,  -272,  -272,    92,  -272,  -272,    92,   105,  -272,  -272,
    -272,   177,   203,   204,   180,   181,   183,   201,   184,  -272,
    -272,   156,  -272,   224,   227,  -272,  -272,    13,   107,   228,
    -272,   170,    73,  -272,   249,   233,    36,   250,   209,   234,
     222,  -272,   235,   102,   191,   239,   240,  -272,   105,   241,
     196,  -272,   205,   199,  -272,   171,   225,  -272,  -272,   143,
    -272,   213,   215,   169,   248,  -272,   120,   180,   204,   264,
     183,   251,  -272,  -272,   -15,  -272,  -272,   -13,   253,   206,
    -272,   227,  -272,   170,   207,   216,   228,    36,   266,  -272,
    -272,  -272,  -272,   -18,  -272,   252,  -272,    36,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,   223,    80,  -272,   105,
     257,   269,  -272,  -272,  -272,   234,   278,   217,   235,   280,
    -272,   255,  -272,   226,  -272,   220,   261,  -272,  -272,   246,
    -272,  -272,   237,   242,  -272,   279,   229,  -272,  -272,   143,
     271,   215,  -272,  -272,  -272,  -272,    19,  -272,  -272,  -272,
     292,  -272,  -272,  -272,  -272,   274,   261,   236,   275,    36,
     216,    36,    36,   238,   243,   286,   252,   276,  -272,   252,
    -272,   -15,   282,   261,   297,   215,  -272,   242,  -272,    53,
     281,   254,   182,   294,  -272,  -272,  -272,   302,   288,   289,
    -272,   307,  -272,  -272,  -272,   247,   256,  -272,   243,  -272,
     258,   105,   133,  -272,   143,  -272,  -272,    72,   290,   254,
    -272,   291,   163,   252,   259,  -272,  -272,  -272,   133,  -272,
    -272,  -272,  -272,   293,   290,   291,   276,  -272,  -272,  -272
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    21,
      20,    15,    16,    17,    18,     9,    10,    11,    12,    13,
      14,     8,     5,     7,     6,     4,    19,     0,     0,     0,
       0,     0,     0,   139,   135,   136,   140,   138,   100,   151,
     152,   137,   126,    80,   100,   125,   127,   129,   131,   134,
       0,     0,     0,     0,    24,     0,     0,     0,    25,    26,
      27,    23,    22,     0,     0,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,    97,     0,     0,    96,     0,
      99,   147,   148,     0,   149,   150,     0,     0,   132,    30,
      29,     0,     0,   117,     0,     0,     0,     0,     0,    28,
      35,    80,   133,     0,   145,   141,   142,   100,   107,   105,
      77,   100,   128,   130,     0,     0,     0,     0,     0,    67,
       0,    51,    38,     0,     0,     0,     0,   144,     0,     0,
       0,   102,     0,     0,   103,     0,   111,    98,    36,     0,
     165,     0,   123,     0,     0,    65,     0,     0,   117,     0,
       0,     0,    44,    45,    48,    46,    47,    48,     0,     0,
      78,   145,   143,   100,     0,   107,   105,     0,    71,    64,
      60,    61,    62,     0,    63,    56,   166,     0,   118,   153,
     154,   155,   156,   157,   158,   163,   161,     0,   159,     0,
       0,     0,   122,    69,    70,    67,     0,     0,    38,     0,
      50,     0,    42,     0,    41,     0,    33,   146,   101,   109,
     104,   106,   112,   113,   115,     0,    82,    58,    59,     0,
       0,   123,   162,   164,   160,   119,     0,   120,    68,    66,
       0,    39,    37,    49,    43,     0,    33,     0,     0,     0,
     107,     0,     0,     0,     0,    87,    56,    53,   124,    56,
     167,    48,     0,    33,     0,   123,   108,   114,   116,    74,
       0,    85,     0,     0,    81,    57,    55,     0,     0,     0,
      40,     0,    34,    31,   110,     0,     0,    72,     0,    83,
       0,     0,     0,    52,     0,   121,    32,    74,    74,    85,
      84,    90,    92,    56,     0,    75,    73,    86,     0,    88,
      93,    94,    89,     0,    74,    90,    53,    76,    91,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -225,  -272,  -272,  -272,   109,   158,  -272,
    -272,  -163,  -272,  -272,     3,  -272,  -249,  -155,  -272,  -272,
     115,   166,  -272,  -271,  -272,  -145,  -272,   211,  -272,    37,
      27,  -272,    20,    12,  -272,    62,   260,   -51,   185,   153,
    -170,  -272,  -272,  -272,    81,   173,  -124,  -218,    -5,  -272,
     244,   -48,  -239,  -232,   162,  -272,  -272,  -272,    63,  -272,
    -272,  -272
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   248,    29,    30,    31,   161,   132,   245,
     167,   212,   133,    32,   277,   278,   230,   185,    33,    34,
     158,   129,   226,   287,    35,    52,    80,    87,   255,   271,
     289,   274,   301,   309,   312,    89,    53,    85,   119,   146,
     144,   250,   178,   222,   223,   127,   224,   188,   153,    55,
      56,    57,    58,    59,   139,    93,    96,    60,   199,   200,
     154,    36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,   203,   152,    90,   214,   220,    79,   275,   213,   202,
     279,   204,    98,   258,    64,     2,   210,    42,   210,     3,
       4,   262,   272,    91,    92,     5,   305,   306,    43,    79,
       6,     7,     8,     9,    10,    11,    84,    81,   282,    12,
      13,    14,   211,   317,   211,   227,   228,   284,   123,    15,
      16,   179,    68,   302,   313,   237,   272,    42,    17,    44,
      45,    46,    47,   231,    49,    50,   141,    51,    43,   302,
     147,    82,    88,    42,   256,    54,   285,   114,   -95,   117,
     266,   259,   180,   181,    43,   182,    83,    61,   183,   286,
     184,   150,    81,   151,    37,   285,    38,    39,   280,    44,
      45,    46,    47,    42,    49,    50,    40,    51,   304,    41,
      62,    65,    63,    97,    43,    44,    45,    46,    47,    48,
      49,    50,   218,    51,    43,   265,    42,    69,   268,   162,
     163,   164,   165,   171,   233,   166,    66,    43,   234,   303,
      94,   201,   115,    95,   116,    44,    45,    46,    47,   113,
      49,    50,   179,    51,    97,    44,    45,    46,    47,    67,
      49,    50,    70,    51,    71,    43,    88,   142,    44,    45,
      46,    47,   -95,    49,    50,   179,    51,   310,   311,   140,
     143,    72,    73,   180,   181,    75,   182,    74,    76,   183,
      77,   184,    78,    84,   235,    86,    44,    45,    46,    47,
      99,   100,   102,   101,    51,   103,   180,   181,   104,   182,
     105,   106,   183,   107,   184,   189,   190,   191,   192,   193,
     194,   108,   109,   195,   110,   196,   197,   198,   189,   190,
     191,   192,   193,   194,   112,   120,   118,   121,   196,   290,
     198,   125,   124,   134,   126,   128,   137,   130,   131,   135,
     138,   145,   148,   155,   149,   156,   168,   157,   160,   159,
     169,   173,   170,   172,   175,   177,   174,   187,   186,   201,
     207,   216,   219,   209,   215,   229,   142,   225,   236,    79,
     232,   239,   240,   242,   247,   246,   300,   243,   249,   244,
     251,   254,   253,   257,   252,   260,   261,   264,   273,   276,
     283,   263,    82,   269,   281,   293,   288,   292,   270,   294,
     296,   295,   297,   285,   308,   316,   234,   241,   208,   319,
     238,   298,   136,   205,   314,   299,   307,   318,   315,   221,
     176,   206,   267,   217,     0,   291,     0,   122,     0,     0,
     111
};

static const yytype_int16 yycheck[] =
{
       5,   156,   126,    54,   167,   175,    10,   256,    21,   154,
     259,   156,    60,   231,     3,     0,    31,    21,    31,     4,
       5,   246,   254,    68,    69,    10,   297,   298,    32,    10,
      15,    16,    17,    18,    19,    20,    23,    42,   263,    24,
      25,    26,    57,   314,    57,    63,    64,   265,    96,    34,
      35,    32,     3,   292,   303,   200,   288,    21,    43,    63,
      64,    65,    66,   187,    68,    69,   117,    71,    32,   308,
     121,    21,    59,    21,   229,    80,    23,    82,    65,    84,
     250,   236,    63,    64,    32,    66,    36,    65,    69,    36,
      71,    55,    97,    57,     6,    23,     8,     9,   261,    63,
      64,    65,    66,    21,    68,    69,     6,    71,    36,     9,
       7,    37,     9,    21,    32,    63,    64,    65,    66,    67,
      68,    69,   173,    71,    32,   249,    21,     3,   252,    27,
      28,    29,    30,   138,    54,    33,    39,    32,    58,   294,
      67,    21,    65,    70,    67,    63,    64,    65,    66,    67,
      68,    69,    32,    71,    21,    63,    64,    65,    66,    65,
      68,    69,     3,    71,     3,    32,    59,    60,    63,    64,
      65,    66,    65,    68,    69,    32,    71,    14,    15,   117,
     118,     3,    44,    63,    64,     9,    66,    65,    65,    69,
      65,    71,    65,    23,   199,    39,    63,    64,    65,    66,
       3,     3,    65,    39,    71,    65,    63,    64,    41,    66,
      45,    21,    69,    65,    71,    46,    47,    48,    49,    50,
      51,    42,     3,    54,     3,    56,    57,    58,    46,    47,
      48,    49,    50,    51,    22,     3,    65,    65,    56,    57,
      58,    38,    65,    42,    40,    65,    22,    66,    65,    65,
      23,    23,     3,     3,    21,    46,    65,    23,    23,    37,
      21,    65,    22,    22,    65,    40,    61,    52,    55,    21,
       6,    65,    65,    22,    21,    23,    60,    11,    21,    10,
      57,     3,    65,     3,    23,    65,   291,    32,    42,    63,
      53,    62,    13,    22,    52,     3,    22,    22,    12,    23,
       3,    65,    21,    65,    22,     3,    52,    13,    65,    21,
       3,    22,    65,    23,    23,    22,    58,   208,   160,   316,
     205,    65,   111,   157,    65,   288,   299,   315,   308,   176,
     145,   158,   251,   171,    -1,   272,    -1,    93,    -1,    -1,
      80
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    73,     0,     4,     5,    10,    15,    16,    17,    18,
      19,    20,    24,    25,    26,    34,    35,    43,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    86,
      87,    88,    95,   100,   101,   106,   143,     6,     8,     9,
       6,     9,    21,    32,    63,    64,    65,    66,    67,    68,
      69,    71,   107,   118,   130,   131,   132,   133,   134,   135,
     139,    65,     7,     9,     3,    37,    39,    65,     3,     3,
       3,     3,     3,    44,    65,     9,    65,    65,    65,    10,
     108,   130,    21,    36,    23,   119,    39,   109,    59,   117,
     119,    68,    69,   137,    67,    70,   138,    21,   133,     3,
       3,    39,    65,    65,    41,    45,    21,    65,    42,     3,
       3,   118,    22,    67,   130,    65,    67,   130,    65,   120,
       3,    65,   132,   133,    65,    38,    40,   127,    65,   103,
      66,    65,    90,    94,    42,    65,   109,    22,    23,   136,
     117,   119,    60,   117,   122,    23,   121,   119,     3,    21,
      55,    57,   128,   130,   142,     3,    46,    23,   102,    37,
      23,    89,    27,    28,    29,    30,    33,    92,    65,    21,
      22,   130,    22,    65,    61,    65,   120,    40,   124,    32,
      63,    64,    66,    69,    71,    99,    55,    52,   129,    46,
      47,    48,    49,    50,    51,    54,    56,    57,    58,   140,
     141,    21,   107,    99,   107,   103,   127,     6,    90,    22,
      31,    57,    93,    21,    93,    21,    65,   136,   119,    65,
     122,   121,   125,   126,   128,    11,   104,    63,    64,    23,
      98,   128,    57,    54,    58,   130,    21,   107,   102,     3,
      65,    89,     3,    32,    63,    91,    65,    23,    85,    42,
     123,    53,    52,    13,    62,   110,    99,    22,   129,    99,
       3,    22,    85,    65,    22,   128,   122,   126,   128,    65,
      65,   111,   135,    12,   113,    98,    23,    96,    97,    98,
      93,    22,    85,     3,   129,    23,    36,   105,    52,   112,
      57,   140,    13,     3,    21,    22,     3,    65,    65,   111,
     130,   114,   134,    99,    36,   105,   105,   112,    23,   115,
      14,    15,   116,    98,    65,   114,    22,   105,   115,    96
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    84,    85,    85,    86,    87,    88,    89,    89,
      90,    90,    90,    91,    92,    92,    92,    92,    93,    93,
      93,    94,    95,    96,    96,    97,    98,    98,    99,    99,
      99,    99,    99,    99,    99,   100,   101,   102,   102,   103,
     103,   104,   104,   104,   105,   105,   105,   106,   107,   108,
     109,   109,   110,   110,   111,   112,   112,   113,   113,   114,
     115,   115,   116,   116,   116,   117,   117,   118,   118,   118,
     119,   119,   119,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   124,   125,   125,   126,   126,   127,   127,   128,
     128,   128,   128,   129,   129,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   141,   141,   142,   142,   143
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     4,     3,
       3,    10,    11,     0,     3,     4,     5,     8,     0,     3,
       6,     3,     3,     1,     1,     1,     1,     1,     0,     2,
       1,     1,    10,     0,     6,     1,     0,     3,     2,     2,
       1,     1,     1,     1,     1,     5,     7,     0,     3,     3,
       3,     0,     4,     6,     0,     3,     5,     4,     5,     1,
       0,     7,     0,     3,     3,     0,     3,     0,     4,     2,
       0,     3,     0,     1,     1,     0,     1,     2,     4,     2,
       0,     5,     3,     2,     4,     0,     3,     0,     5,     0,
       3,     0,     2,     1,     3,     1,     3,     0,     3,     3,
       3,     6,     2,     0,     3,     1,     1,     1,     3,     1,
       3,     1,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     5,     4,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     1,     2,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 22: /* exit: EXIT SEMICOLON  */
#line 216 "yacc_sql.y"
                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_EXIT;//"exit";
    }
#line 1538 "yacc_sql.tab.c"
    break;

  case 23: /* help: HELP SEMICOLON  */
#line 221 "yacc_sql.y"
                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_HELP;//"help";
    }
#line 1546 "yacc_sql.tab.c"
    break;

  case 24: /* sync: SYNC SEMICOLON  */
#line 226 "yacc_sql.y"
                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SYNC;
    }
#line 1554 "yacc_sql.tab.c"
    break;

  case 25: /* begin: TRX_BEGIN SEMICOLON  */
#line 232 "yacc_sql.y"
                        {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_BEGIN;
    }
#line 1562 "yacc_sql.tab.c"
    break;

  case 26: /* commit: TRX_COMMIT SEMICOLON  */
#line 238 "yacc_sql.y"
                         {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_COMMIT;
    }
#line 1570 "yacc_sql.tab.c"
    break;

  case 27: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 244 "yacc_sql.y"
                           {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_ROLLBACK;
    }
#line 1578 "yacc_sql.tab.c"
    break;

  case 28: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 250 "yacc_sql.y"
                            {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1587 "yacc_sql.tab.c"
    break;

  case 29: /* show_tables: SHOW TABLES SEMICOLON  */
#line 256 "yacc_sql.y"
                          {
      CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SHOW_TABLES;
    }
#line 1595 "yacc_sql.tab.c"
    break;

  case 30: /* desc_table: DESC ID SEMICOLON  */
#line 262 "yacc_sql.y"
                      {
      CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1604 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE INDEX ID ON ID LBRACE ID id_list RBRACE SEMICOLON  */
#line 269 "yacc_sql.y"
                                                             {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_CREATE_INDEX;//"create_index";
        create_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 0);
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, (yyvsp[-3].string));
    }
#line 1614 "yacc_sql.tab.c"
    break;

  case 32: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE ID id_list RBRACE SEMICOLON  */
#line 274 "yacc_sql.y"
                                                                      {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_CREATE_INDEX;//"create_index";
        create_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 1);
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, (yyvsp[-3].string));
    }
#line 1624 "yacc_sql.tab.c"
    break;

  case 34: /* id_list: COMMA ID id_list  */
#line 282 "yacc_sql.y"
                       {
        create_index_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_index, (yyvsp[-1].string));
    }
#line 1632 "yacc_sql.tab.c"
    break;

  case 35: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 288 "yacc_sql.y"
                             {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_DROP_INDEX;//"drop_index";
        drop_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.drop_index, (yyvsp[-1].string));
    }
#line 1641 "yacc_sql.tab.c"
    break;

  case 36: /* show_index: SHOW INDEX FROM ID SEMICOLON  */
#line 295 "yacc_sql.y"
                                 {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_SHOW_INDEX;
        show_index_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.show_index, (yyvsp[-1].string));
    }
#line 1650 "yacc_sql.tab.c"
    break;

  case 37: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 302 "yacc_sql.y"
                                                                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_CREATE_TABLE;//"create_table";
        create_table_init_name(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, (yyvsp[-5].string));
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length = 0;
    }
#line 1661 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 311 "yacc_sql.y"
                                   {    }
#line 1667 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type LBRACE number RBRACE is_nullable  */
#line 315 "yacc_sql.y"
                                                 {
        AttrInfo attribute;
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, (yyvsp[-4].number), (yyvsp[-2].number), (yyvsp[0].number));
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
#line 1678 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type is_nullable  */
#line 321 "yacc_sql.y"
                              {
        AttrInfo attribute;
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, (yyvsp[-1].number), 4, (yyvsp[0].number));
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
#line 1689 "yacc_sql.tab.c"
    break;

  case 42: /* attr_def: ID_get TEXT_T is_nullable  */
#line 327 "yacc_sql.y"
                                {
        AttrInfo attribute;
        // TODO: try 4096?
        attr_info_init(&attribute, CONTEXT->pc[CONTEXT->cur].id, CHARS, 4097, (yyvsp[0].number));
        create_table_append_attribute(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.create_table, &attribute);
        CONTEXT->pc[CONTEXT->cur].value_length++;
    }
#line 1701 "yacc_sql.tab.c"
    break;

  case 43: /* number: NUMBER  */
#line 336 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 1707 "yacc_sql.tab.c"
    break;

  case 44: /* type: INT_T  */
#line 339 "yacc_sql.y"
          { (yyval.number)=INTS; }
#line 1713 "yacc_sql.tab.c"
    break;

  case 45: /* type: STRING_T  */
#line 340 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 1719 "yacc_sql.tab.c"
    break;

  case 46: /* type: FLOAT_T  */
#line 342 "yacc_sql.y"
              { (yyval.number)=FLOATS; }
#line 1725 "yacc_sql.tab.c"
    break;

  case 47: /* type: DATE_T  */
#line 343 "yacc_sql.y"
             { (yyval.number) = DATES; }
#line 1731 "yacc_sql.tab.c"
    break;

  case 48: /* is_nullable: %empty  */
#line 346 "yacc_sql.y"
                 { (yyval.number) = 0; }
#line 1737 "yacc_sql.tab.c"
    break;

  case 49: /* is_nullable: NOT NULL_V  */
#line 347 "yacc_sql.y"
                 { (yyval.number) = 0; }
#line 1743 "yacc_sql.tab.c"
    break;

  case 50: /* is_nullable: NULL_T  */
#line 348 "yacc_sql.y"
                 { (yyval.number) = 1; }
#line 1749 "yacc_sql.tab.c"
    break;

  case 51: /* ID_get: ID  */
#line 351 "yacc_sql.y"
       {
        char *temp=(yyvsp[0].string);
        snprintf(CONTEXT->pc[CONTEXT->cur].id, sizeof(CONTEXT->pc[CONTEXT->cur].id), "%s", temp);
    }
#line 1758 "yacc_sql.tab.c"
    break;

  case 52: /* insert: INSERT INTO ID VALUES LBRACE value value_list RBRACE insert_list SEMICOLON  */
#line 358 "yacc_sql.y"
                                                                               {
        CONTEXT->pc[CONTEXT->cur].ssql->flag=SCF_INSERT;//"insert";
        inserts_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, (yyvsp[-7].string));
        // 从前往后插，不是逆序
        inserts_add(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
    }
#line 1771 "yacc_sql.tab.c"
    break;

  case 54: /* insert_list: insert_comma LBRACE value value_list RBRACE insert_list  */
#line 368 "yacc_sql.y"
                                                              { }
#line 1777 "yacc_sql.tab.c"
    break;

  case 55: /* insert_comma: COMMA  */
#line 371 "yacc_sql.y"
          {
        inserts_add(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.insertion, CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
    }
#line 1787 "yacc_sql.tab.c"
    break;

  case 57: /* value_list: COMMA value value_list  */
#line 378 "yacc_sql.y"
                              {
        // CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++] = *$2;
    }
#line 1795 "yacc_sql.tab.c"
    break;

  case 58: /* value: SUB NUMBER  */
#line 383 "yacc_sql.y"
              {
        value_init_integer(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], -(yyvsp[0].number));
    }
#line 1803 "yacc_sql.tab.c"
    break;

  case 59: /* value: SUB FLOAT  */
#line 386 "yacc_sql.y"
               {
        value_init_float(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], -(yyvsp[0].floats));
    }
#line 1811 "yacc_sql.tab.c"
    break;

  case 60: /* value: NUMBER  */
#line 389 "yacc_sql.y"
             {
        value_init_integer(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], (yyvsp[0].number));
    }
#line 1819 "yacc_sql.tab.c"
    break;

  case 61: /* value: FLOAT  */
#line 392 "yacc_sql.y"
           {
        value_init_float(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], (yyvsp[0].floats));
    }
#line 1827 "yacc_sql.tab.c"
    break;

  case 62: /* value: SSS  */
#line 395 "yacc_sql.y"
         {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
        value_init_string(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], (yyvsp[0].string));
    }
#line 1836 "yacc_sql.tab.c"
    break;

  case 63: /* value: TIME  */
#line 399 "yacc_sql.y"
           {
        value_init_date(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++], (yyvsp[0].string));
    }
#line 1844 "yacc_sql.tab.c"
    break;

  case 64: /* value: NULL_V  */
#line 402 "yacc_sql.y"
             {
        value_init_null(&CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length++]);
    }
#line 1852 "yacc_sql.tab.c"
    break;

  case 65: /* delete: DELETE FROM ID where SEMICOLON  */
#line 408 "yacc_sql.y"
                                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_DELETE;//"delete";
        deletes_init_relation(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.deletion, (yyvsp[-2].string));
        deletes_set_conditions(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.deletion,
            CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
#line 1864 "yacc_sql.tab.c"
    break;

  case 66: /* update: UPDATE ID SET id_value_pair id_value_pair_list where SEMICOLON  */
#line 417 "yacc_sql.y"
                                                                   {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_UPDATE;//"update";
        updates_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.update, (yyvsp[-5].string), CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
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
#line 1884 "yacc_sql.tab.c"
    break;

  case 68: /* id_value_pair_list: COMMA id_value_pair id_value_pair_list  */
#line 434 "yacc_sql.y"
                                             {

    }
#line 1892 "yacc_sql.tab.c"
    break;

  case 69: /* id_value_pair: ID EQ value  */
#line 439 "yacc_sql.y"
                {
        Value value = CONTEXT->pc[CONTEXT->cur].values[CONTEXT->pc[CONTEXT->cur].value_length-1];
        updates_append_value_pair(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.update, (yyvsp[-2].string), value);
    }
#line 1901 "yacc_sql.tab.c"
    break;

  case 70: /* id_value_pair: ID EQ sub_select  */
#line 443 "yacc_sql.y"
                       {
        // NOTE: pc[0], or pc[CONTEXT->cur - 1]
        updates_append_select_pair(&CONTEXT->pc[0].ssql->sstr.update, (yyvsp[-2].string), &CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
    }
#line 1912 "yacc_sql.tab.c"
    break;

  case 71: /* group_by: %empty  */
#line 451 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].group_num = 0; }
#line 1918 "yacc_sql.tab.c"
    break;

  case 72: /* group_by: GROUP BY ID group_by_list  */
#line 452 "yacc_sql.y"
                               {
        Expr* expr = generate_id_expr_ptr(NULL, (yyvsp[-1].string));
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
#line 1927 "yacc_sql.tab.c"
    break;

  case 73: /* group_by: GROUP BY ID DOT ID group_by_list  */
#line 456 "yacc_sql.y"
                                      {
        Expr* expr = generate_id_expr_ptr((yyvsp[-3].string), (yyvsp[-1].string));
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
#line 1936 "yacc_sql.tab.c"
    break;

  case 75: /* group_by_list: COMMA ID group_by_list  */
#line 462 "yacc_sql.y"
                            {
        Expr* expr = generate_id_expr_ptr(NULL, (yyvsp[-1].string));
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
#line 1945 "yacc_sql.tab.c"
    break;

  case 76: /* group_by_list: COMMA ID DOT ID group_by_list  */
#line 466 "yacc_sql.y"
                                   {
        Expr* expr = generate_id_expr_ptr((yyvsp[-3].string), (yyvsp[-1].string));
        select_append_group_by_element(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, expr, &CONTEXT->pc[CONTEXT->cur].group_num);
    }
#line 1954 "yacc_sql.tab.c"
    break;

  case 77: /* select: SELECT select_attr fromClause SEMICOLON  */
#line 472 "yacc_sql.y"
                                           {
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
#line 1973 "yacc_sql.tab.c"
    break;

  case 78: /* sub_select: LBRACE select_flag select_attr fromClause RBRACE  */
#line 488 "yacc_sql.y"
                                                      {
        // 每次调用sub_select都需要cur--, selects_destroy
    }
#line 1981 "yacc_sql.tab.c"
    break;

  case 79: /* select_flag: SELECT  */
#line 493 "yacc_sql.y"
           {
        CONTEXT->cur++;
        memset(&CONTEXT->pc[CONTEXT->cur], 0, sizeof(ParserContextI));
        CONTEXT->pc[CONTEXT->cur].ssql = query_create();
    }
#line 1991 "yacc_sql.tab.c"
    break;

  case 80: /* fromClause: %empty  */
#line 500 "yacc_sql.y"
                {
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.expr_num = CONTEXT->pc[CONTEXT->cur].expr_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.join_num = CONTEXT->pc[CONTEXT->cur].join_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.condition_num = CONTEXT->pc[CONTEXT->cur].condition_length;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.group_by_num = CONTEXT->pc[CONTEXT->cur].group_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.having_num = CONTEXT->pc[CONTEXT->cur].having_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.order_by_num = CONTEXT->pc[CONTEXT->cur].order_num;
    }
#line 2004 "yacc_sql.tab.c"
    break;

  case 81: /* fromClause: FROM join join_list where_tree group_by havingClause order_by  */
#line 508 "yacc_sql.y"
                                                                    {
        selects_append_conditions(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].conditions, CONTEXT->pc[CONTEXT->cur].condition_length);
        selects_append_having(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].havingConditions, CONTEXT->pc[CONTEXT->cur].having_num);
        selects_append_order(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, CONTEXT->pc[CONTEXT->cur].orders, CONTEXT->pc[CONTEXT->cur].order_num);
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.expr_num = CONTEXT->pc[CONTEXT->cur].expr_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.join_num = CONTEXT->pc[CONTEXT->cur].join_num;
        CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection.group_by_num = CONTEXT->pc[CONTEXT->cur].group_num;
    }
#line 2017 "yacc_sql.tab.c"
    break;

  case 82: /* havingClause: %empty  */
#line 519 "yacc_sql.y"
              { CONTEXT->pc[CONTEXT->cur].having_num = 0; }
#line 2023 "yacc_sql.tab.c"
    break;

  case 83: /* havingClause: HAVING having_condition having_condition_list  */
#line 520 "yacc_sql.y"
                                                   {

    }
#line 2031 "yacc_sql.tab.c"
    break;

  case 84: /* having_condition: function comOp expression  */
#line 525 "yacc_sql.y"
                             {
        // TODO: need or operator?
        Condition condition;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], (yyvsp[-2].expr), (yyvsp[0].expr), -1, -1);
        CONTEXT->pc[CONTEXT->cur].havingConditions[CONTEXT->pc[CONTEXT->cur].having_num++] = condition;
    }
#line 2042 "yacc_sql.tab.c"
    break;

  case 86: /* having_condition_list: AND having_condition having_condition_list  */
#line 533 "yacc_sql.y"
                                                {

    }
#line 2050 "yacc_sql.tab.c"
    break;

  case 87: /* order_by: %empty  */
#line 538 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].order_num = 0; }
#line 2056 "yacc_sql.tab.c"
    break;

  case 88: /* order_by: ORDER BY order_condition order_condition_list  */
#line 539 "yacc_sql.y"
                                                    {
        
    }
#line 2064 "yacc_sql.tab.c"
    break;

  case 89: /* order_condition: term is_asc  */
#line 544 "yacc_sql.y"
                {
        OrderExpression order;
        order_init(&order, (yyvsp[-1].expr), (yyvsp[0].number));
        CONTEXT->pc[CONTEXT->cur].orders[CONTEXT->pc[CONTEXT->cur].order_num++] = order;
    }
#line 2074 "yacc_sql.tab.c"
    break;

  case 91: /* order_condition_list: COMMA order_condition order_condition_list  */
#line 552 "yacc_sql.y"
                                                 {

    }
#line 2082 "yacc_sql.tab.c"
    break;

  case 92: /* is_asc: %empty  */
#line 557 "yacc_sql.y"
                { (yyval.number) = 1; }
#line 2088 "yacc_sql.tab.c"
    break;

  case 93: /* is_asc: ASC  */
#line 558 "yacc_sql.y"
                { (yyval.number) = 1; }
#line 2094 "yacc_sql.tab.c"
    break;

  case 94: /* is_asc: DESC  */
#line 559 "yacc_sql.y"
                { (yyval.number) = 0; }
#line 2100 "yacc_sql.tab.c"
    break;

  case 97: /* select_attr: STAR attr_list  */
#line 566 "yacc_sql.y"
                  {
        Expr* starExpr = generate_id_expr_ptr(NULL, "*");
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, starExpr, &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
#line 2109 "yacc_sql.tab.c"
    break;

  case 98: /* select_attr: expression as_or_empty ID attr_list  */
#line 570 "yacc_sql.y"
                                         {
        set_alias((yyvsp[-3].expr), (yyvsp[-1].string));
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[-3].expr), &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
#line 2118 "yacc_sql.tab.c"
    break;

  case 99: /* select_attr: expression attr_list  */
#line 574 "yacc_sql.y"
                           {
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[-1].expr), &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
#line 2126 "yacc_sql.tab.c"
    break;

  case 101: /* attr_list: COMMA expression as_or_empty ID attr_list  */
#line 581 "yacc_sql.y"
                                                {
        set_alias((yyvsp[-3].expr), (yyvsp[-1].string));
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[-3].expr), &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
#line 2135 "yacc_sql.tab.c"
    break;

  case 102: /* attr_list: COMMA expression attr_list  */
#line 585 "yacc_sql.y"
                                 {
        selects_append_expression(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[-1].expr), &CONTEXT->pc[CONTEXT->cur].expr_num);
    }
#line 2143 "yacc_sql.tab.c"
    break;

  case 103: /* join: ID inner_join_list  */
#line 591 "yacc_sql.y"
                      {
        InnerJoinNode* startNode = make_join_node((yyvsp[-1].string), CONTEXT->pc[CONTEXT->cur].conditions, 0);
        if((yyvsp[0].joinNode) != NULL){
            set_join_node_left((yyvsp[0].joinNode), startNode);
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[0].joinNode), &CONTEXT->pc[CONTEXT->cur].join_num);
        }else{
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, startNode, &CONTEXT->pc[CONTEXT->cur].join_num);
        }
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
#line 2158 "yacc_sql.tab.c"
    break;

  case 104: /* join: ID as_or_empty ID inner_join_list  */
#line 601 "yacc_sql.y"
                                       {
        InnerJoinNode* startNode = make_join_node((yyvsp[-3].string), CONTEXT->pc[CONTEXT->cur].conditions, 0);
        JoinNodeSetAlias(startNode, (yyvsp[-1].string));
        if((yyvsp[0].joinNode) != NULL){
            set_join_node_left((yyvsp[0].joinNode), startNode);
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, (yyvsp[0].joinNode), &CONTEXT->pc[CONTEXT->cur].join_num);
        }else{
            selects_append_join_node(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection, startNode, &CONTEXT->pc[CONTEXT->cur].join_num);
        }
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
#line 2174 "yacc_sql.tab.c"
    break;

  case 106: /* join_list: COMMA join join_list  */
#line 616 "yacc_sql.y"
                          {
        // nothing to do
    }
#line 2182 "yacc_sql.tab.c"
    break;

  case 107: /* inner_join_list: %empty  */
#line 621 "yacc_sql.y"
             {
        (yyval.joinNode) = NULL;
    }
#line 2190 "yacc_sql.tab.c"
    break;

  case 108: /* inner_join_list: INNER JOIN ID on_clause inner_join_list  */
#line 624 "yacc_sql.y"
                                             {
        CONTEXT->pc[CONTEXT->cur].stackDeepth --;
        InnerJoinNode* node = make_join_node((yyvsp[-2].string), CONTEXT->pc[CONTEXT->cur].onClauseStack[CONTEXT->pc[CONTEXT->cur].stackDeepth], CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth]);
        CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth] = 0;
        if((yyvsp[0].joinNode) != NULL){
            set_join_node_left((yyvsp[0].joinNode), node);
            (yyval.joinNode) = (yyvsp[0].joinNode);
        }else{
            (yyval.joinNode) = node;
        }
    }
#line 2206 "yacc_sql.tab.c"
    break;

  case 110: /* on_clause: ON condition condition_list  */
#line 638 "yacc_sql.y"
                                 {
        // do something!!!!
        for(size_t i = 0 ; i < CONTEXT->pc[CONTEXT->cur].condition_length; ++i){
            CONTEXT->pc[CONTEXT->cur].onClauseStack[CONTEXT->pc[CONTEXT->cur].stackDeepth][i] = CONTEXT->pc[CONTEXT->cur].conditions[i];
        }
        CONTEXT->pc[CONTEXT->cur].onClauseConditionNum[CONTEXT->pc[CONTEXT->cur].stackDeepth++] = CONTEXT->pc[CONTEXT->cur].condition_length;
        CONTEXT->pc[CONTEXT->cur].condition_length = 0;
    }
#line 2219 "yacc_sql.tab.c"
    break;

  case 111: /* where_tree: %empty  */
#line 648 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].condition_length = 0; }
#line 2225 "yacc_sql.tab.c"
    break;

  case 112: /* where_tree: WHERE or_condition  */
#line 649 "yacc_sql.y"
                         {
        // NOTE: 根节点是最后一个，只要从根节点开始遍历
    }
#line 2233 "yacc_sql.tab.c"
    break;

  case 113: /* or_condition: and_condition  */
#line 653 "yacc_sql.y"
                  {
        (yyval.number) = (yyvsp[0].number);
    }
#line 2241 "yacc_sql.tab.c"
    break;

  case 114: /* or_condition: or_condition OR and_condition  */
#line 656 "yacc_sql.y"
                                    {
        Condition condition;
        condition_init(&condition, OR_OP, NULL, NULL, (yyvsp[-2].number), (yyvsp[0].number));
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2252 "yacc_sql.tab.c"
    break;

  case 115: /* and_condition: condition  */
#line 664 "yacc_sql.y"
              {
        (yyval.number) = (yyvsp[0].number);
    }
#line 2260 "yacc_sql.tab.c"
    break;

  case 116: /* and_condition: and_condition AND condition  */
#line 667 "yacc_sql.y"
                                  {
        Condition condition;
        condition_init(&condition, AND_OP, NULL, NULL, (yyvsp[-2].number), (yyvsp[0].number));
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2271 "yacc_sql.tab.c"
    break;

  case 117: /* where: %empty  */
#line 675 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].condition_length = 0; }
#line 2277 "yacc_sql.tab.c"
    break;

  case 118: /* where: WHERE condition condition_list  */
#line 676 "yacc_sql.y"
                                     {
        // nothing to do
    }
#line 2285 "yacc_sql.tab.c"
    break;

  case 119: /* condition: expression comOp expression  */
#line 680 "yacc_sql.y"
                               {
        Condition condition;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], (yyvsp[-2].expr), (yyvsp[0].expr), -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2296 "yacc_sql.tab.c"
    break;

  case 120: /* condition: expression in_not_in sub_select  */
#line 686 "yacc_sql.y"
                                      {
        Condition condition;
        Expr* select_expr = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], (yyvsp[-2].expr), select_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2310 "yacc_sql.tab.c"
    break;

  case 121: /* condition: expression in_not_in LBRACE value value_list RBRACE  */
#line 695 "yacc_sql.y"
                                                          {
        // in和comOp不放在一起防冲突
        Condition condition;
        Expr* list_expr = generate_list_expr_ptr(CONTEXT->pc[CONTEXT->cur].values, CONTEXT->pc[CONTEXT->cur].value_length);
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], (yyvsp[-5].expr), list_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        //临时变量清零
        CONTEXT->pc[CONTEXT->cur].value_length=0;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2325 "yacc_sql.tab.c"
    break;

  case 122: /* condition: exists_not_exists sub_select  */
#line 705 "yacc_sql.y"
                                   {
        Condition condition;
        Expr* select_expr = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
        condition_init(&condition, CONTEXT->pc[CONTEXT->cur].comps[--(CONTEXT->pc[CONTEXT->cur].comp_length)], NULL, select_expr, -1, -1);
        CONTEXT->pc[CONTEXT->cur].conditions[CONTEXT->pc[CONTEXT->cur].condition_length] = condition;
        (yyval.number) = CONTEXT->pc[CONTEXT->cur].condition_length++;
    }
#line 2339 "yacc_sql.tab.c"
    break;

  case 124: /* condition_list: AND condition condition_list  */
#line 717 "yacc_sql.y"
                                   {
        // nothing to do
    }
#line 2347 "yacc_sql.tab.c"
    break;

  case 125: /* expression: summand  */
#line 723 "yacc_sql.y"
            {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2355 "yacc_sql.tab.c"
    break;

  case 126: /* expression: sub_select  */
#line 726 "yacc_sql.y"
                 {
        (yyval.expr) = generate_subquery_expr_ptr(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        selects_destroy(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.selection);
        CONTEXT->cur--;
    }
#line 2365 "yacc_sql.tab.c"
    break;

  case 127: /* summand: factor  */
#line 734 "yacc_sql.y"
           {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2373 "yacc_sql.tab.c"
    break;

  case 128: /* summand: summand summandOp factor  */
#line 737 "yacc_sql.y"
                               {
        (yyval.expr) = generate_binary_expr_ptr((yyvsp[-2].expr), (yyvsp[0].expr), CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
#line 2381 "yacc_sql.tab.c"
    break;

  case 129: /* factor: unary  */
#line 742 "yacc_sql.y"
          {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2389 "yacc_sql.tab.c"
    break;

  case 130: /* factor: factor factorOp unary  */
#line 745 "yacc_sql.y"
                            {
        (yyval.expr) = generate_binary_expr_ptr((yyvsp[-2].expr), (yyvsp[0].expr), CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
#line 2397 "yacc_sql.tab.c"
    break;

  case 131: /* unary: term  */
#line 750 "yacc_sql.y"
         {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2405 "yacc_sql.tab.c"
    break;

  case 132: /* unary: unaryOp unary  */
#line 753 "yacc_sql.y"
                    {
        (yyval.expr) = generate_unary_expr_ptr((yyvsp[0].expr), CONTEXT->pc[CONTEXT->cur].exprops[--(CONTEXT->pc[CONTEXT->cur].exprop_length)]);
    }
#line 2413 "yacc_sql.tab.c"
    break;

  case 133: /* term: LBRACE expression RBRACE  */
#line 758 "yacc_sql.y"
                             {
        setBracketed((yyvsp[-1].expr), true);
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 2422 "yacc_sql.tab.c"
    break;

  case 134: /* term: function  */
#line 762 "yacc_sql.y"
               {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2430 "yacc_sql.tab.c"
    break;

  case 135: /* term: NUMBER  */
#line 765 "yacc_sql.y"
             {
        char * literal = int2string((yyvsp[0].number));
        (yyval.expr) = generate_const_expr_ptr(literal, INTS);
    }
#line 2439 "yacc_sql.tab.c"
    break;

  case 136: /* term: FLOAT  */
#line 769 "yacc_sql.y"
            {
        char* literal = float2string((yyvsp[0].floats));
        (yyval.expr) = generate_const_expr_ptr(literal, FLOATS);
    }
#line 2448 "yacc_sql.tab.c"
    break;

  case 137: /* term: TIME  */
#line 773 "yacc_sql.y"
           {
        char* literal = date2string((yyvsp[0].string));
        (yyval.expr) = generate_const_expr_ptr(literal, DATES);
    }
#line 2457 "yacc_sql.tab.c"
    break;

  case 138: /* term: SSS  */
#line 777 "yacc_sql.y"
          {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
        (yyval.expr) = generate_const_expr_ptr((yyvsp[0].string), CHARS);
    }
#line 2466 "yacc_sql.tab.c"
    break;

  case 139: /* term: NULL_V  */
#line 781 "yacc_sql.y"
             {
        (yyval.expr) = generate_const_expr_ptr(NULL, NULLS);
    }
#line 2474 "yacc_sql.tab.c"
    break;

  case 140: /* term: ID  */
#line 784 "yacc_sql.y"
         {
        (yyval.expr) = generate_id_expr_ptr(NULL, (yyvsp[0].string));
    }
#line 2482 "yacc_sql.tab.c"
    break;

  case 141: /* term: ID DOT ID  */
#line 787 "yacc_sql.y"
                {
        (yyval.expr) = generate_id_expr_ptr((yyvsp[-2].string), (yyvsp[0].string));
    }
#line 2490 "yacc_sql.tab.c"
    break;

  case 142: /* term: ID DOT STAR  */
#line 790 "yacc_sql.y"
                 {
        (yyval.expr) = generate_id_expr_ptr((yyvsp[-2].string), "*");
    }
#line 2498 "yacc_sql.tab.c"
    break;

  case 143: /* function: ID LBRACE expression func_para_list RBRACE  */
#line 796 "yacc_sql.y"
                                              {
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = (yyvsp[-2].expr);
        Expr* para = generate_func_expr_ptr((yyvsp[-4].string), CONTEXT->pc[CONTEXT->cur].para_list, CONTEXT->pc[CONTEXT->cur].para_num);
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        (yyval.expr) = para;
    }
#line 2509 "yacc_sql.tab.c"
    break;

  case 144: /* function: ID LBRACE STAR RBRACE  */
#line 802 "yacc_sql.y"
                           {
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = (yyval.expr) = generate_id_expr_ptr(NULL, "*");
        Expr* para = generate_func_expr_ptr((yyvsp[-3].string), CONTEXT->pc[CONTEXT->cur].para_list, CONTEXT->pc[CONTEXT->cur].para_num);
        CONTEXT->pc[CONTEXT->cur].para_num = 0;
        (yyval.expr) = para;
    }
#line 2520 "yacc_sql.tab.c"
    break;

  case 146: /* func_para_list: COMMA expression func_para_list  */
#line 811 "yacc_sql.y"
                                     {
        CONTEXT->pc[CONTEXT->cur].para_list[CONTEXT->pc[CONTEXT->cur].para_num++] = (yyvsp[-1].expr);
    }
#line 2528 "yacc_sql.tab.c"
    break;

  case 147: /* summandOp: ADD  */
#line 816 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_ADD; }
#line 2534 "yacc_sql.tab.c"
    break;

  case 148: /* summandOp: SUB  */
#line 817 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_SUB; }
#line 2540 "yacc_sql.tab.c"
    break;

  case 149: /* factorOp: STAR  */
#line 820 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_MUL; }
#line 2546 "yacc_sql.tab.c"
    break;

  case 150: /* factorOp: DIV  */
#line 821 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_DIV; }
#line 2552 "yacc_sql.tab.c"
    break;

  case 151: /* unaryOp: ADD  */
#line 824 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_ADD; }
#line 2558 "yacc_sql.tab.c"
    break;

  case 152: /* unaryOp: SUB  */
#line 825 "yacc_sql.y"
          { CONTEXT->pc[CONTEXT->cur].exprops[CONTEXT->pc[CONTEXT->cur].exprop_length++] = OP_SUB; }
#line 2564 "yacc_sql.tab.c"
    break;

  case 153: /* comOp: EQ  */
#line 829 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = EQUAL_TO; }
#line 2570 "yacc_sql.tab.c"
    break;

  case 154: /* comOp: LT  */
#line 830 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LESS_THAN; }
#line 2576 "yacc_sql.tab.c"
    break;

  case 155: /* comOp: GT  */
#line 831 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = GREAT_THAN; }
#line 2582 "yacc_sql.tab.c"
    break;

  case 156: /* comOp: LE  */
#line 832 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LESS_EQUAL; }
#line 2588 "yacc_sql.tab.c"
    break;

  case 157: /* comOp: GE  */
#line 833 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = GREAT_EQUAL; }
#line 2594 "yacc_sql.tab.c"
    break;

  case 158: /* comOp: NE  */
#line 834 "yacc_sql.y"
         { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_EQUAL; }
#line 2600 "yacc_sql.tab.c"
    break;

  case 159: /* comOp: LIKE_OP  */
#line 835 "yacc_sql.y"
              { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = LIKE; }
#line 2606 "yacc_sql.tab.c"
    break;

  case 160: /* comOp: NOT LIKE_OP  */
#line 836 "yacc_sql.y"
                  { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_LIKE; }
#line 2612 "yacc_sql.tab.c"
    break;

  case 161: /* comOp: IS_OP  */
#line 837 "yacc_sql.y"
            { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IS; }
#line 2618 "yacc_sql.tab.c"
    break;

  case 162: /* comOp: IS_OP NOT  */
#line 838 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IS_NOT; }
#line 2624 "yacc_sql.tab.c"
    break;

  case 163: /* in_not_in: IN_OP  */
#line 840 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = IN; }
#line 2630 "yacc_sql.tab.c"
    break;

  case 164: /* in_not_in: NOT IN_OP  */
#line 841 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_IN; }
#line 2636 "yacc_sql.tab.c"
    break;

  case 165: /* exists_not_exists: EXISTS_OP  */
#line 844 "yacc_sql.y"
                { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = EXISTS; }
#line 2642 "yacc_sql.tab.c"
    break;

  case 166: /* exists_not_exists: NOT EXISTS_OP  */
#line 845 "yacc_sql.y"
                    { CONTEXT->pc[CONTEXT->cur].comps[CONTEXT->pc[CONTEXT->cur].comp_length++] = NOT_EXISTS; }
#line 2648 "yacc_sql.tab.c"
    break;

  case 167: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 849 "yacc_sql.y"
                                                 {
        CONTEXT->pc[CONTEXT->cur].ssql->flag = SCF_LOAD_DATA;
        load_data_init(&CONTEXT->pc[CONTEXT->cur].ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
    }
#line 2657 "yacc_sql.tab.c"
    break;


#line 2661 "yacc_sql.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (scanner, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 854 "yacc_sql.y"

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
