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
    
    result->len = scanLong(table, result->r, BLOCK_SIZE);
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
    while (true) {
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
    while (true) {
        Relation* in = child->nextVec();
        if (in == nullptr) {
            return nullptr;
        }
        Relation* out = new Relation();
        *out = allocateRelation(in->len);
        switch (type) {
            case ReduceType::COUNT:
                out->len = aggCount(in->r, in->len);
                break;
            case ReduceType::SUM:
                out->len = aggSum(in->r, in->len);
                break;
        }
        hasMoreTuples = false;
        return out;
    }
}

void AggregationOp::closeVec() {
    child->closeVec();
}


