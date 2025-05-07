/**
 * @file
 *
 * Base operator methods and initialization of static variables.
 *
 * Based on code from Henning Funke (TU-Dortmund)
 *
 * @authors: Jana Giceva <jana.giceva@in.tum.de>, Alexander Beischl <beischl@in.tum.de>
 */

/* TODO: Add execution of query plan with vector-at-a-time.
 * HINT: Start by inspecting the main function. */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <unistd.h>

#include "DBData.h"
#include "Operators.h"


#ifndef RELATION_LEN
    #define RELATION_LEN 200000000
#endif


/**
  * @brief Class to time executions.
  */
class Timer {
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    /**
      * @brief Take timestamp at a position in the code
      */
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    /**
      * @brief Get milliseconds from timestamp to now
      */
    double get() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        return diff.count() * 1000;
    }
};


/**
  * @brief Output header line for csv
  */
void csvHeader () {
    std::cout << std::endl << "RELATION_LEN, tVolcano, tOperatorAtATime" << std::endl;
}


/**
  * @brief Output timings of different execution models as csv line
  */
void csvStats ( double tVolc, double tOp ) {
    std::cout << std::fixed;
    std::cout << std::setprecision(1);
    std::cout <<  RELATION_LEN << ", " << tVolc 
              << ", " << tOp << std::endl;
}


/**
  * @brief Print query result
  */
void printRelation ( Relation r, bool onlyTupleCount = false ) {
    std::cout << "result " << r.len << " tuples" << std::endl; 
    if ( onlyTupleCount ) return;
    size_t i=0;
    for ( ; i<10 && i<r.len; i++ )
        std::cout << r.r[i] << std::endl;
    if ( i < (r.len-1) ) 
        std::cout << "[...]" << std::endl;
}


/**
  * @brief Execute query plan given by root with Volcano (Tuple-at-a-time)
  */
double execVolcano ( RelOperator* root ) { 
    Timer tVolc = Timer();
    Relation rel = allocateRelation ( root->getSize() );
    PullDriver::volcano ( root, &rel );
    printRelation ( rel );
    freeRelation ( rel ); 
    return tVolc.get();
}


/**
  * @brief Execute query plan given by root with Operator-at-a-time
  */
double execOperatorAtATime ( RelOperator* root ) { 
    Timer tOp = Timer();
    Relation resultRelation = root->getRelation();
    printRelation ( resultRelation );
    return tOp.get();
}


/**
  * @brief Parse arguments, generate/read relation, build query plan, and execute.
    The different execution models are selected by arguments or by default we execute all.
  */
int main ( int argc, char* argv[] ) {

    // parse arguments
    bool doVol=false, doOp=false;
    std::string args;
    for (int i = 1; i < argc; ++i) {
        args = args.append ( argv[i] );
    }
    if ( args.find ( "vol" ) != std::string::npos ) doVol = true;
    if ( args.find ( "op" ) != std::string::npos ) doOp = true;
    if ( ! ( doVol || doOp ) ) {
        doVol = true; doOp = true;
    }

    // load or generate relation data
    const char* dbFile = "db.dat";
    Relation relation;
    if ( !loadData ( &relation, dbFile, RELATION_LEN ) ) {
        std::cout << "Generating data.." << std::endl;
        genData ( &relation, dbFile, RELATION_LEN );
    }

    // build plan
    RelOperator* root = new AggregationOp ( AggregationOp::SUM,
        new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 77,
            new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 30,
                new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 99,
                    new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 42,
                        new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 11,
                            new ScanOp ( relation.r, RELATION_LEN )
                        )
                    )
                )
            )
        )
    );

/* --  alternative query plans --
    RelOperator* root = new SelectionOp ( SelectionOp::PredicateType::EQUALS, 11,
        new ScanOp ( relation.r, relation.len )
    );
    
    RelOperator* root = new AggregationOp ( AggregationOp::SUM,
        new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 43,
            new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 42,
                new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 11,
                    new SelectionOp ( SelectionOp::PredicateType::EQUALS_NOT, 12,
                        new ScanOp ( relation.r, relation.len )
                    )
                )
            )
        )
    );

    RelOperator* root = new AggregationOp ( AggregationOp::SUM,
        new ScanOp ( relation.r, RELATION_LEN )
    );
*/    

    double tVol=0.0, tOp=0.0;

    if ( doVol )  tVol  = execVolcano ( root );
    if ( doOp )   tOp  = execOperatorAtATime ( root );

    csvHeader ();
    csvStats ( tVol, tOp );

    root->deletePlan();
}



















