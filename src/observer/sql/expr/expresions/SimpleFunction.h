//
// Created by zx on 10/24/22.
//

#ifndef MINIDB_SIMPLEFUNCTION_H
#define MINIDB_SIMPLEFUNCTION_H

#include "FunctionExpression.h"

class SimpleFunction: public FunctionExpression {
public:
    virtual RC mapToRelation(Relation* relation) = 0;
    virtual RC calculate(Row& row) = 0;

    virtual std::string toString() const = 0;
    virtual std::string displayName(bool) const = 0;

    virtual AttrType type() const = 0;
    virtual RC get_value(const Tuple &tuple, TupleCell &cell) const;

    virtual const char * fromSingleTable() const = 0;
    virtual RC setSubExpressions(std::vector<BaseExpression*>) = 0;
    virtual bool containAggregate() const override { return false;}

    virtual std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const = 0;
};

class Length : public SimpleFunction{
public:
    Length() = default;

    RC setSubExpressions(std::vector<BaseExpression*>) override;
    RC mapToRelation(Relation* relation) override;
    RC calculate(Row& row) override;
    std::string displayName(bool) const override;
    AttrType type() const override;
    RC get_value(const Tuple &tuple, TupleCell &cell) const override;
    const char * fromSingleTable() const override;
    std::string toString() const override;
    bool isConst() const override{return expression_->isConst();}
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const override;

private:
    BaseExpression* expression_;
};

class Round : public SimpleFunction{
public:
    Round() = default;
    RC setSubExpressions(std::vector<BaseExpression*>) override;
    RC mapToRelation(Relation* relation) override;
    RC calculate(Row& row) override;
    std::string displayName(bool) const override;
    AttrType type() const override;
    const char * fromSingleTable() const override;
    std::string toString() const override;
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const override;
    bool isConst() const override{
      if(int_){
        return float_->isConst() && int_->isConst();
      }else{
        return float_->isConst();
      }
    }

private:
    BaseExpression* float_;
    BaseExpression* int_;
    const static int defaultInt;
};

class DateFormat : public SimpleFunction{
public:
    DateFormat() = default;
    RC setSubExpressions(std::vector<BaseExpression*>) override;
    RC mapToRelation(Relation* relation) override;
    RC calculate(Row& row) override;
    std::string displayName(bool) const override;
    AttrType type() const override;
    const char * fromSingleTable() const override;
    std::string toString() const override;
    bool isConst() const override{
      return date->isConst() && format->isConst();
    }
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const override;

private:
    BaseExpression* date;
    BaseExpression* format;
    const static std::vector<std::string> monthMap;
    const static std::vector<std::string> dayMap;
};
#endif //MINIDB_SIMPLEFUNCTION_H
