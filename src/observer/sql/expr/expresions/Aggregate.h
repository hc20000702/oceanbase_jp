//
// Created by zx on 10/24/22.
//

#ifndef MINIDB_AGGREGATE_H
#define MINIDB_AGGREGATE_H

#include "rc.h"
#include "sql/expr/relation/Relation.h"
#include "BaseExpression.h"
#include "unordered_map"

class Aggregate {
public:
    RC evaluate(Relation*&);

    void setGroupBy(std::vector<BaseExpression*> groupBy){
      groupBy_ = groupBy;
    }

    virtual RC beforeStart() = 0;
    virtual RC treatRow() = 0;
    virtual RC afterDealAllRow() = 0;
    virtual std::vector<Item*> getColumn();
    void setSource(Relation* r){ source = r;}
    virtual RC dealWithEmptyRelation() {
      std::string emptyTable = "emptyTable";
      order.emplace_back(emptyTable);
      Item* nullItem = new NullItem;
      context.insert(std::make_pair(emptyTable, nullItem));
      return RC::SUCCESS;
    }

    virtual void cacheClear(){
      order.clear();
      context.clear();
    }

protected:
    Relation* source;
    std::vector<BaseExpression*> groupBy_;
    Row currentRow;
    std::unordered_map<std::string, Item*> context;
    std::vector<std::string> order;
};


#endif //MINIDB_AGGREGATE_H
