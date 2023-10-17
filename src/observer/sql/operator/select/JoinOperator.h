//
// Created by zx on 10/23/22.
//

#ifndef MINIDB_JOINOPERATOR_H
#define MINIDB_JOINOPERATOR_H

#include "sql/operator/QueryOperator.h"
#include "sql/stmt/filter_stmt.h"

class JoinOperator: public QueryOperator {
public:
    JoinOperator(): relNum(0){}
    void addOperator(QueryOperator* oper){
      subOperators.emplace_back(oper);
      relNum ++;
    }
    RC evaluate(Relation * &) override;
    RC close() override;

    void setJoinFilter(FilterStmt* filterStmt){
      filterStmt_ = filterStmt;
    }

    Row nextRow();

    bool hasNext();

    void setSchemaOfFather(std::vector<Schema> schemaOfFather) override{
      for(auto it: subOperators){
        it->setSchemaOfFather(schemaOfFather);
      }
      schemaOfFather_ = schemaOfFather;
    }

    void setRowFromFather(Row rowFromFather) override{
      for(auto it: subOperators){
        it->setRowFromFather(rowFromFather);
      }
      rowFromFather_ = rowFromFather;
    }

private:
    std::vector<QueryOperator*> subOperators;
    FilterStmt* filterStmt_;
    int relNum;
    std::vector<Relation*> relations;
    std::vector<int> indexes;
    std::vector<int> relLengths;
    bool flag;
};


#endif //MINIDB_JOINOPERATOR_H
