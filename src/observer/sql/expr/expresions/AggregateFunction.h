//
// Created by zx on 10/23/22.
//

#ifndef MINIDB_AGGREGATEFUNCTION_H
#define MINIDB_AGGREGATEFUNCTION_H

#include "FunctionExpression.h"
#include "Aggregate.h"
#include "sql/expr/relation/Relation.h"
#include "unordered_map"
#include "sql/expr/expresions/ConstExpression.h"

class AggregateFunction:
    public FunctionExpression,
    public Aggregate
{
public:
    RC mapToRelation(Relation* relation) override{
      relation_ = relation;
      if(tryToFindColumn(relation)){
        return SUCCESS;
      }
      RC rc;
      rc = subExpression_->mapToRelation(relation);
      if(rc!=SUCCESS) return rc;
      for(auto it: groupBy_){
        rc = it->mapToRelation(relation);
        if(rc!=SUCCESS) return rc;
      }
      return RC::SUCCESS;
    }

    virtual RC calculate(Row& row) {
      if(column!=-1){
        if(row.at(column)->isNull()){
          isNull_ = true;
          return RC::SUCCESS;
        }
        switch (type_) {
          case INTS:
          case DATES:
            intValue = row.at(column)->getInt();
            break;
          case FLOATS:
            floatValue = row.at(column)->getFloat();
            break;
          case CHARS:
            textValue = row.at(column)->getText();
            break;
          default:
            return RC::UNKNOWN_TYPE;
        }
        return RC::SUCCESS;
      }
      return RC::SHOULD_NOT_BE_CALLED;
    }

    virtual std::string toString() const = 0;
    virtual std::string displayName(bool) const = 0;
    virtual std::string displayName(bool, std::unordered_map<std::string, std::string>&) const = 0;
    virtual AttrType inferType() const {
      return subExpression_->type();
    }

    virtual AttrType type() const{
      return type_;
    }

    virtual RC get_value(const Tuple &tuple, TupleCell &cell) const {
      return RC::SHOULD_NOT_BE_CALLED;
    }

    const char* fromSingleTable() const override{
      return subExpression_->fromSingleTable();
    }

    bool isAggregateFunc() const override {
      return true;
    }

    BaseExpression* getSubExpression(){
      return subExpression_;
    }

    bool containAggregate() const override {return true;}
    virtual std::vector<BaseExpression*> collectAllAggregate() override{
      std::vector<BaseExpression*> vector;
      vector.emplace_back(this);
      return vector;
    }
    virtual bool isConst() const {
      return false;
    };

public:
    virtual RC beforeStart() = 0;
    virtual RC treatRow() = 0;
    virtual RC afterDealAllRow() = 0;

protected:
    Relation* relation_;
    BaseExpression* subExpression_;
    bool assigned = false;
    AttrType type_;
};

class Identity: public AggregateFunction{
public:
    // FunctionExpression
    Identity(BaseExpression* subExpression) {
      subExpression_ = subExpression;
      type_ = inferType();
    }

    std::string toString() const override {return subExpression_->toString();}
    std::string displayName(bool flag) const override{return subExpression_->displayName(flag);}
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return subExpression_->displayName(flag, map);
    }
    bool isConst() const override{return subExpression_->isConst();}
public:
    // Aggregate
    RC beforeStart() override;
    RC treatRow() override;
    RC afterDealAllRow() override;
};

class Max : public AggregateFunction{
public:
    // FunctionExpression
    Max(BaseExpression* subExpression);

    std::string toString() const override;
    std::string displayName(bool) const override;
    bool isConst() const override{return subExpression_->isConst();}
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "MAX(" + subExpression_->displayName(flag, map) + ")";
    }

public:
    // Aggregate
    RC treatRow() override;
    RC beforeStart() override;
    RC afterDealAllRow() override;

};

class Min: public AggregateFunction{
public:
    // FunctionExpression
    Min(BaseExpression* subExpression){
      subExpression_ = subExpression;
      type_ = inferType();
    }

    std::string toString() const override;
    std::string displayName(bool) const override;
    bool isConst() const override{return subExpression_->isConst();}
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "MIN(" + subExpression_->displayName(flag, map) + ")";
    }
public:
    // Aggregate
    RC beforeStart() override;
    RC treatRow() override;
    RC afterDealAllRow() override;
};

class Avg: public AggregateFunction{
public:
    // FunctionExpression
    Avg(BaseExpression* subExpression){
      subExpression_ = subExpression;
      type_ = FLOATS;
    }

    std::string toString() const override{
      return "AVG(" + subExpression_->toString() + ")";
    }
    std::string displayName(bool flag) const override{
      return "AVG(" + subExpression_->displayName(flag) + ")";
    }
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "AVG(" + subExpression_->displayName(flag, map) + ")";
    }
    AttrType type() const{
      return FLOATS;
    }

public:
    // Aggregate
    RC beforeStart() override;
    RC treatRow() override;
    RC afterDealAllRow() override;

    void cacheClear() override{
      sum.clear();
      count.clear();
    }
private:
    std::map<std::string, float> sum;
    std::map<std::string, int> count;
    AttrType subExprType;
};

class Sum: public AggregateFunction{
public:
    Sum(BaseExpression* subExpression){
      subExpression_ = subExpression;
      type_ = inferType();
    }

    std::string toString() const override{
      return "SUM(" + subExpression_->toString() + ")";
    }
    std::string displayName(bool flag) const override{
      return "SUM(" + subExpression_->displayName(flag) + ")";
    }
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "SUM(" + subExpression_->displayName(flag, map) + ")";
    }
    AttrType type() const override{
      return type_;
    }
    bool isConst() const override{return subExpression_->isConst();}
public:
    // Aggregate
    RC beforeStart() override;
    RC treatRow() override;
    RC afterDealAllRow() override;
    void cacheClear() override{
      sumFloat.clear();
      sumInt.clear();
      count.clear();
    }

private:
    std::map<std::string, int> sumInt;
    std::map<std::string, float> sumFloat;
    std::map<std::string, int> count;
    AttrType subExprType;
};

class Count: public AggregateFunction{
public:
    Count(BaseExpression* subExpression){
      alias_ = nullptr;
      subExpression_ = subExpression;
      type_ = INTS;
    }

    std::string toString() const override{
      return "COUNT(" + subExpression_->toString() + ")";
    }
    std::string displayName(bool flag) const override{
      return "COUNT(" + subExpression_->displayName(flag) + ")";
    }
    bool isConst() const override{return false;}
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "COUNT(" + subExpression_->displayName(flag, map) + ")";
    }
public:
    // Aggregate
    RC beforeStart() override;
    RC treatRow() override;
    RC afterDealAllRow() override;
    RC dealWithEmptyRelation() override{
      std::string emptyTable = "emptyTable";
      order.emplace_back(emptyTable);
      context[emptyTable] = new IntItem(0, false);
      return RC::SUCCESS;
    }

    void cacheClear() override {
      count.clear();
    }
private:
    std::map<std::string, int> count;
};

class CountStar: public Count{
public:
    CountStar() : Count(new ConstExpression("1", INTS)) {
      type_ = INTS;
    }
    std::string toString() const override{
      return "COUNT(*)";
    }
    std::string displayName(bool flag) const override{
      return "COUNT(*)";
    }
    std::string displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
      return "COUNT(*)";
    }
};
#endif //MINIDB_AGGREGATEFUNCTION_H
