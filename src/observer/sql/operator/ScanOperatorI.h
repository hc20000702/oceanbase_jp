//
// Created by zx on 10/26/22.
//

#ifndef MINIDB_SCANOPERATORI_H
#define MINIDB_SCANOPERATORI_H

#include "operator.h"

class ScanOperatorI : public Operator{
public:
    virtual const Table* getTable() = 0;
    virtual ~ScanOperatorI() = default;

    virtual RC open() = 0;
    virtual RC next() = 0;
    virtual RC close() = 0;

    virtual Tuple * current_tuple() = 0;
};


#endif //MINIDB_SCANOPERATORI_H
