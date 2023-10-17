//
// Created by zx on 10/24/22.
//

#ifndef MINIDB_AGGREGATEOPERATOR_H
#define MINIDB_AGGREGATEOPERATOR_H

#include <utility>

#include "sql/operator/QueryOperator.h"
#include "sql/expr/expresions/AggregateFunction.h"

class AggregateOperator: public QueryOperator{
public:
    AggregateOperator(QueryOperator* subOperator, std::vector<AggregateFunction*> aggregates)
      : subOperator_(subOperator), aggregates_(std::move(aggregates)){}

    RC evaluate(Relation * &) override;
    void setGroupBy(std::vector<BaseExpression*> groupBy){
      groupBy_ = groupBy;
    }
    RC close() override;
    void setSchemaOfFather(std::vector<Schema> schemaOfFather) override{
      subOperator_->setSchemaOfFather(schemaOfFather);
      schemaOfFather_ = schemaOfFather;
    }

    void setRowFromFather(Row rowFromFather) override{
      subOperator_->setRowFromFather(rowFromFather);
      rowFromFather_ = rowFromFather;
    }
private:
    QueryOperator* subOperator_;
    std::vector<AggregateFunction*> aggregates_;
    std::vector<BaseExpression*> groupBy_;
};


#endif //MINIDB_AGGREGATEOPERATOR_H
