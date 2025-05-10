/**
 * @file
 *
 * Implementation of vector-at-a-time execution for relational operators.
 *
 */

/* TODO: Add openVec(), nextVec() and closeVec() implementations for vector-at-a-time processing */

#include "primitives.h"
#include "Operators.h"

void ScanOp::openVec() {
    cursor = 0;
}

Relation* ScanOp::nextVec() {
    if (cursor >= tableSize) {
        return nullptr;
    }
    Relation* result = new Relation();
    *result = allocateRelation(BLOCK_SIZE);
    
    size_t remaining = tableSize - cursor;
    size_t scanSize = (remaining < BLOCK_SIZE) ? remaining : BLOCK_SIZE;
    
    result->len = scanLong(table + cursor, result->r, scanSize);
    cursor += result->len;
    return result;
}

void ScanOp::closeVec() {
    cursor = 0;
}

void SelectionOp::openVec() {
    child->openVec();
}

Relation* SelectionOp::nextVec() {
    Relation* in = child->nextVec();
    if (in == nullptr) {
        return nullptr;
    }
    Relation* out = new Relation();
    *out = allocateRelation(in->len);
    switch (type) {
        case PredicateType::EQUALS:
            out->len = compareEquals(in->r, compareConstant, out->r, in->len);
            break;
        case PredicateType::EQUALS_NOT:
            out->len = compareNotEquals(in->r, compareConstant, out->r, in->len);
            break;
        case PredicateType::SMALLER:
            out->len = compareSmaller(in->r, compareConstant, out->r, in->len);
            break;
    }
    return out;
}

void SelectionOp::closeVec() {
    child->closeVec();
}

void AggregationOp::openVec() {
    child->openVec();
    hasMoreTuples = true;
}

Relation* AggregationOp::nextVec() {
    if (!hasMoreTuples) {
        return nullptr;
    }
    Relation* out = new Relation();
    *out = allocateRelation(1);
    out->len = 1;
    out->r[0] = 0;
    
    while (true) {
        Relation* in = child->nextVec();
        if (in == nullptr) {
            break;
        }
        
        switch (type) {
            case ReduceType::COUNT:
                out->r[0] += aggCount(in->r, in->len);
                break;
            case ReduceType::SUM:
                out->r[0] += aggSum(in->r, in->len);
                break;
        }
        freeRelation(*in);
    }
    
    hasMoreTuples = false;
    return out;
}

void AggregationOp::closeVec() {
    child->closeVec();
}


