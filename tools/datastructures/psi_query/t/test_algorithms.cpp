/*
 * For simple tests on 1 file
#define BOOST_TEST_DYN_LINK//Delete this after tests
#define BOOST_TEST_MODULE Algorithms_test//Delete this after tests
#include <boost/test/unit_test.hpp>//Delete this after tests
*/
#include "../../../../tests/tests.hpp"
#include "../DFSA.hpp"
#include "../Algorithms.hpp"



BOOST_AUTO_TEST_CASE(algorithms_test)
{
	psi::DFSA<> fsa;
    psi::State q0 = fsa.addState();
    psi::State q1 = fsa.addState();
    psi::State q2 = fsa.addState();
    psi::State q3 = fsa.addState();
    
    
    fsa.setStartState(q0);
    fsa.setEndState(q3);
    
    fsa.addArc(q0, psi::Arc<>('a', q1));
    fsa.addArc(q0, psi::Arc<>('b', q1));
    fsa.addArc(q0, psi::Arc<>('c', q1));
    
    fsa.addArc(q1, psi::Arc<>('a', q1));
    fsa.addArc(q1, psi::Arc<>('b', q2));
    fsa.addArc(q1, psi::Arc<>('c', q3));
    
    fsa.addArc(q2, psi::Arc<>('a', q1));
    fsa.addArc(q2, psi::Arc<>('b', q3));
    fsa.addArc(q2, psi::Arc<>('c', q3));
    
    fsa.addArc(q3, psi::Arc<>('a', q1));
    fsa.addArc(q3, psi::Arc<>('b', q3));
    fsa.addArc(q3, psi::Arc<>('c', q3));
	
	psi::Reverser<psi::DFSA<> > reverser(fsa); //Warning signed and unsigned
	
	//Check the size of sets////////////////////////////////////////////
	BOOST_CHECK_EQUAL(fsa.getEndStates().size() , reverser.startFn().size());
	//BOOST_CHECK_EQUAL(fsa.getStartStates().size() , reverser.finalFn().size());
	////////////////////////////////////////////////////////////////////
	
	
	
	//Checking reverser start-end states////////////////////////////////
	std::set< psi::DFSA<>::state_type >::iterator reverserIt;
	std::set< psi::DFSA<>::state_type >::iterator fsaIt;
	
	for ( reverserIt = reverser.startFn().begin() ; reverserIt != reverser.startFn().end(); ++reverserIt ) {
		BOOST_CHECK_EQUAL( fsa.isEndState( *reverserIt ) , true );
	}
	
	//I dont check the size of fsa it should be equal in previous test
	for ( reverserIt = reverser.startFn().begin() , fsaIt = fsa.getEndStates().begin() ; reverserIt != reverser.startFn().end() ; ++reverserIt , ++fsaIt ) {
		BOOST_CHECK_EQUAL( *reverserIt , *fsaIt );
	}
	
	//End states
	
	for ( fsaIt = fsa.getStartStates().begin() ; fsaIt != fsa.getStartStates().end(); ++fsaIt ) {
		BOOST_CHECK_EQUAL( reverser.finalFn( *fsaIt ) ,  true );
	}
	
	////////////////////////////////////////////////////////////////////
}


