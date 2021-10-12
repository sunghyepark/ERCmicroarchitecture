//mapping.cpp
#include "circuit.h"

using namespace std;
using namespace Qcircuit;

void Qcircuit::QMapper::Circuit_Mapping(Circuit& dgraph)
{

//    make_Dlist_all(dgraph);
//    cout << "*** Dlist (all) *** " << endl;
//    cout << Dlist_all << endl;
    
    FinalCircuit.nodeset.clear();
    //cout << dgraph.nodeset.size() << endl;
    for(auto& gate : dgraph.nodeset)
    {
        FinalCircuit.nodeset.push_back(gate);
    }
}

