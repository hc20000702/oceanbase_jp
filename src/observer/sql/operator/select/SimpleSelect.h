//
// Created by zx on 10/27/22.
//

#ifndef MINIDB_SIMPLESELECT_H
#define MINIDB_SIMPLESELECT_H

#include "sql/operator/QueryOperator.h"

class SimpleSelect: public QueryOperator{
public:
    SimpleSelect(std::vector<BaseExpression*> expressions) :expressions_(expressions){}
    RC evaluate(Relation * &) override;
    RC close() {return RC::SUCCESS;}
    bool isMultiTable(){return false;}

    void setSchemaOfFather(std::vector<Schema> schemaOfFather) {
      return;
    }

    void setRowFromFather(Row rowFromFather) {
      return;
    }
private:
    std::vector<BaseExpression*> expressions_;

};


#endif //MINIDB_SIMPLESELECT_H
