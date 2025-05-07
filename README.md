# Readme 
This is the second assignment of *Data Processing On Modern Hardware*.
Please have a look at the assignment sheet you find on the course's webpage or in our Moodle.

## Introduction
- **Fork** this repo to do the assignment.
- There's a GitLab CI that will check if all required files exist.
- Passing the CI does not mean that you get the exam point! We'll double-check uploads.

## Part 1
 - Compare the tuple-at-a-time, operator-at-a-time execution models.
 - Use a profiler to monitor your system resources.

## Part 2: Vector-at-a-time

Implement vector-at-a-time interface for DBMS operators

## Part 3: Report

Visualise and briefly explain your results in a one page report (report.pdf).

## Files you need to modify:
* `BaseOperator.h`
* `Operators.h`
* `OperatorsVector.cpp`
* `WeeDB.cpp`

Hint: take a look at `primitives.h`

## Description of this project:

This is a program to analyze different database execution
engines. Compile by executing the command
```
mkdir build
cd build
cmake ../
make
```

To execute the program run from within the build-directory:
```
./weedb
```
or use `./weedb vol` or `./weedb op` to use specific execution
techniques, i.e. the former for tuple-at-a-time (Volcano) and the
latter for operator-at-a-time. The program output contains query 
timing and results.

The database is kept in the memory mapped file `db.dat`, which is
generated on the first run. This allows consecutive executions 
without data re-generation. To free space or to generate new data 
you can simply delete the file. To use the program without memory 
mapped files, you can adjust the functionality from the file
`DBData.cpp` to work on plain arrays.

For query execution, you can specify different queries as 
chain/tree of relational operators. E.g. for the query

```
SELECT SUM(x) FROM rel WHERE x < 10
```

you can execute the query plan
```
RelOperator* root = new AggregationOp ( AggregationOp::SUM,
  new SelectionOp ( SelectionOp::PredicateType::SMALLER, 10,
      new ScanOp ( rel.r, rel.len )
  )
);
```

Note: In case you encounter any errors regarding cmake, you might need to run:
```
pip install cmake
```
