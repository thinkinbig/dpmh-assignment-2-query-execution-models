#include <gtest/gtest.h>

#include "DBData.h"
#include "Operators.h"
#include "Util.h"

TEST(WeeDB, VectorAtATime_Select) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root = new SelectionOp(SelectionOp::PredicateType::EQUALS, 11,
                                      new ScanOp(relation.r, relation.len));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(17));
  for (size_t i = 0; i < rel.len; i++)
    ASSERT_EQ(rel.r[i], 11);
}

TEST(WeeDB, VectorAtATime_SelectNE) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root = new SelectionOp(SelectionOp::PredicateType::EQUALS_NOT, 11,
                                      new ScanOp(relation.r, relation.len));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(1983));
  for (size_t i = 0; i < rel.len; i++)
    ASSERT_NE(rel.r[i], 11);
}

TEST(WeeDB, VectorAtATime_SelectSmaller) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root = new SelectionOp(SelectionOp::PredicateType::SMALLER, 11,
                                      new ScanOp(relation.r, relation.len));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(214));
  for (size_t i = 0; i < rel.len; i++)
    ASSERT_TRUE(rel.r[i] < 11);
}

TEST(WeeDB, VectorAtATime_AggregateSum) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root = new AggregationOp(AggregationOp::SUM, new ScanOp(relation.r, relation_len));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(1));
  ASSERT_EQ(rel.r[0], 99325);
}

TEST(WeeDB, VectorAtATime_AggregateCount) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root = new AggregationOp(AggregationOp::COUNT, new SelectionOp(SelectionOp::PredicateType::EQUALS, 11, new ScanOp(relation.r, relation.len)));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(1));
  ASSERT_EQ(rel.r[0], 17);
}

TEST(WeeDB, VectorAtATime_ComplexOperator) {
  Relation relation;

  auto dbFile = getDir(__FILE__) + "/data/db.dat";
  size_t relation_len = 2000;
  loadData(&relation, dbFile.c_str(), relation_len);

  RelOperator *root =  new AggregationOp(AggregationOp::SUM, new SelectionOp(SelectionOp::PredicateType::EQUALS_NOT, 43, new SelectionOp(SelectionOp::PredicateType::EQUALS_NOT, 42, new SelectionOp(SelectionOp::PredicateType::EQUALS_NOT, 11, new SelectionOp(SelectionOp::PredicateType::EQUALS_NOT, 12, new ScanOp(relation.r, relation.len))))));

  Relation rel = allocateRelation(root->getSize());
  PullDriver::vector(root, &rel);
  ASSERT_EQ(rel.len, size_t(1));
  ASSERT_EQ(rel.r[0], 97174);
}
