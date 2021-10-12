/*----------------------------------------------------------------------------------------------*/
/* Description:     Main source code for Quantum decoder                                        */
/*                                                                                              */
/* Author:          Sunghye Park, Postech                                                       */
/*                                                                                              */
/* Created:         10/12/2021 (for ERC project)                                                */
/*----------------------------------------------------------------------------------------------*/

#include "circuit.h"
#include "QASMtoken.hpp"
#include "QASMscanner.hpp"
#include "QASMparser.h"
#include "mymeasure.h"

using namespace std;
using namespace Qcircuit;

Qcircuit::QMapper mapper;
static CMeasure measure;

int main(int argc, char** argv)
{
    cout << endl;
    cout << "========================================" << endl;
    cout << "    Quantum Decoder by Sunghye Park     " << endl;
    cout << "========================================" << endl;
    cout << endl;

    measure.start_clock();
    
    //parsing
    mapper.parsing(argc, argv);
    measure.stop_clock("QASM parsing & generating DAG");
   
    //QASM decoding
    //mapper.initial_mapping();
    mapper.Circuit_Mapping(mapper.Dgraph);
   
    mapper.FinalCircuit_info(mapper.FinalCircuit);
    mapper.write_output(mapper.FinalCircuit);
    measure.stop_clock("mapping");

    //program end
    measure.stop_clock("END Program");
    cout << endl <<  "*** Time & Memory Measure ***" << endl;
    measure.print_clock();
    measure.printMemoryUsage();



    return 0;
}
