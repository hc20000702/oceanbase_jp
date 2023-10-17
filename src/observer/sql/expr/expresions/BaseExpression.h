//
// Created by zx on 10/19/22.
//

#ifndef MINIDB_BASEEXPRESSION_H
#define MINIDB_BASEEXPRESSION_H

#include "rc.h"
#include "string"
#include "sql/parser/parse_defs.h"
#include "sql/expr/tuple_cell.h"
#include "storage/common/field.h"
#include "sql/expr/tuple.h"
#include "sql/expr/relation/Row.h"
#include "unordered_map"
// #include "sql/expr/relation/Relation.h"

class Relation;

class BaseExpression {
public :
    BaseExpression(){
      alias_ = nullptr;
    }
    virtual ~BaseExpression() {}
    virtual RC mapToRelation(Relation* relation) = 0;
    virtual RC calculate(Row& row) = 0;

    virtual int currentInt() {
      return intValue;
    };
    virtual float currentFloat() {
      return floatValue;
    }

    virtual const char* currentText(){
      return textValue;
    }

    virtual std::string toString() const = 0;
    virtual std::string displayName(bool, std::unordered_map<std::string, std::string>&) const = 0;
    virtual std::string displayName(bool) const = 0;
    virtual AttrType type() const = 0;

    virtual bool isCurrentNull() const {return isNull_;}

    virtual bool isId() const{ return false;}
    virtual bool isConst() const = 0;
    virtual bool isAggregateFunc() const{ return false; }
    virtual bool isRelation() const {return false;}
    virtual RC get_value(const Tuple &tuple, TupleCell &cell) const = 0;

    virtual Item* toItem();

    static RC compare(BaseExpression* left, BaseExpression* right, Row& row, CompOp, bool &result);

    // return: const - "*", single table - tablename, multi-table - nullptr
    virtual const char* fromSingleTable() const = 0;

    void setAlias(const char* alias){alias_ = alias;}
    const char * getAlias(){ return alias_; }

    bool isLeaf(){
      return isLeaf_;
    }

    void setLeaf(bool isleaf = true){
      isLeaf_ = isleaf;
    }

    virtual void setBracketed(bool flag){
      isBracketed_ = flag;
    }

    virtual bool isBracketed(){
      return isBracketed_;
    }

    virtual bool tryToFindColumn(Relation*);

    virtual bool containAggregate() const = 0;
    virtual std::vector<BaseExpression*> collectAllAggregate() {
      return std::vector<BaseExpression*>(0);
    }
protected:
    int intValue;
    float floatValue;
    const char* alias_;
    const char* textValue;
    bool isNull_ = false;
    bool isLeaf_;
    bool isBracketed_;
    int column = -1;
};

#endif  // MINIDB_BASEEXPRESSION_H
