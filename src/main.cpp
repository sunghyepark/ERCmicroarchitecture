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

//coupling graph
#define ARCHI ARCHITECTURE::IBM_Q_20            //ICCAD Paper
#define INITIAL INITIALTYPE::GRAPH_MATCHING_MIX_UPDATE  //ICCAD Paper
//#define INITIAL INITIALTYPE::GRAPH_MATCHING_MIX
//#define INITIAL INITIALTYPE::NEW_GRAPH_MATCHING

#define PRE_PROCESSING 1
#define POST_PROCESSING 0       //ICCAD Paper
#define PROCESS_NUM 7   //for test

#define PRINT_mapping_process 1
#define DEBUG_main 1

#define BRIDGE_MODE 1 //ICCAD Paper ///////////////////////////

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
    mapper.initial_mapping(ARCHI, PRE_PROCESSING, false, false, INITIAL);
    mapper.Circuit_Mapping(mapper.Dgraph, ARCHI, true, PRE_PROCESSING, false, false);
   
    mapper.FinalCircuit_info(mapper.FinalCircuit, false);
    mapper.write_output(mapper.FinalCircuit);
    measure.stop_clock("mapping");
//    //mapping
//#if PRINT_mapping_process
//    cout << "========================  1st mapping  =============================" << endl;
//    //cost parameter ----------------------
//    cout << "Cost_parameter:\t";
//    cout << "alpha: " << mapper.param_alpha << ", beta: " << mapper.param_beta << ", gamma: " << mapper.param_gamma << endl;    
//#endif    
//    bool forward = true;
//
//    if(BRIDGE_MODE)
//    {
//        // No Bridge
//#if PRINT_mapping_process
//        cout << "========================  1) NO_BRIDGE  =============================" << endl;
//#endif
//        mapper.initial_mapping(ARCHI, PRE_PROCESSING, false, false, INITIAL);
//        mapper.Circuit_Mapping(mapper.Dgraph, ARCHI, forward, PRE_PROCESSING, false, false);
//        measure.stop_clock("Mapping_NoBridge");    
//#if PRINT_mapping_process
//        mapper.print_layout();
//        mapper.print_qubit();
//#endif
//        mapper.least_cnot_num = mapper.add_cnot_num;
//        mapper.least_swap_num = mapper.add_swap_num;
//        mapper.least_bridge_num = mapper.add_bridge_num;
//        mapper.least_depth = mapper.add_depth;
//        mapper.Best_FinalCircuit.nodeset = mapper.FinalCircuit.nodeset;
//       
//        mapper.FinalCircuit_info(mapper.FinalCircuit, false);
//        mapper.write_output(mapper.FinalCircuit);
//        
//        //cout << "least_cnot_num : " << mapper.least_cnot_num << endl << endl;
//
//        // Bridge
//#if PRINT_mapping_process
//        cout << "========================  2) BRIGE  =============================" << endl;
//#endif
//        mapper.initial_mapping(ARCHI, PRE_PROCESSING, false, true, INITIAL);
//        mapper.Circuit_Mapping(mapper.Dgraph, ARCHI, forward, PRE_PROCESSING, false, true);
//        measure.stop_clock("Mapping_Bridge");
//#if PRINT_mapping_process
//        mapper.print_layout();
//        mapper.print_qubit();
//#endif
//
//        mapper.FinalCircuit_info(mapper.FinalCircuit, false);
//        //mapper.write_output(mapper.FinalCircuit);
//
//    }
//    else
//    {
//        mapper.initial_mapping(ARCHI, PRE_PROCESSING, false, false, INITIAL);
//        mapper.Circuit_Mapping(mapper.Dgraph, ARCHI, forward, PRE_PROCESSING, false, BRIDGE_MODE);
//        measure.stop_clock("Mapping");
//    
//#if PRINT_mapping_process
//        mapper.print_layout();
//        mapper.print_qubit();
//#endif
//        mapper.FinalCircuit_info(mapper.FinalCircuit, false);
//        //mapper.write_output(mapper.FinalCircuit);
//    }
//
//#if POST_PROCESSING
//#if PRINT_mapping_process
//    cout << "========================  Post Processing  =============================" << endl;
//#endif
//    //best circuit information
//    mapper.FinalCircuit_info(mapper.Best_FinalCircuit, true);
//    mapper.write_output(mapper.Best_FinalCircuit);
//    if(!BRIDGE_MODE)
//    {
//        mapper.least_cnot_num = mapper.add_cnot_num;
//        mapper.least_swap_num = mapper.add_swap_num;
//        mapper.least_bridge_num = mapper.add_bridge_num;
//        mapper.least_depth = mapper.add_depth;
//        mapper.Best_FinalCircuit.nodeset = mapper.FinalCircuit.nodeset;
//    }
//
//    for(int i=0; i<PROCESS_NUM; i++)
//    {
//        string str1 = to_string(i+1)+"_Reverse mapping";
//        string str2 = to_string(i+1)+"_Forward mapping";
//
//        /*for reverse circuit*/
//#if PRINT_mapping_process
//        cout <<"======================== " << i+1 << "'s reverse mapping ===============================" << endl;
//#endif
//        forward = false;
//        mapper.initial_mapping(ARCHI, PRE_PROCESSING, true, BRIDGE_MODE, INITIAL);
//        mapper.Circuit_Mapping(mapper.Dgraph_reverse, ARCHI, forward, PRE_PROCESSING, true, BRIDGE_MODE);
//#if DEBUG_main
//        measure.stop_clock(str1);
//#endif
//
//        /*for final mapping*/
//#if PRINT_mapping_process
//        cout <<  "======================== " << i+1 << "'s forward mapping ===============================" << endl;
//#endif
//        forward = true;
//        mapper.initial_mapping(ARCHI, PRE_PROCESSING, true, BRIDGE_MODE, INITIAL);
//        mapper.Circuit_Mapping(mapper.Dgraph, ARCHI, forward, PRE_PROCESSING, true, BRIDGE_MODE);
//#if DEBUG_main
//        measure.stop_clock(str2);
//#endif
//
//        //final circuit information
//        mapper.FinalCircuit_info(mapper.FinalCircuit, false);
//
//
//    }
//#if PRINT_mapping_process
//    cout << "=======================================================" << endl << endl;
//#endif
//
//
//#endif //POST_PROCESSING
//    
//    
//#if POST_PROCESSING || BRIDGE_MODE
//    cout << "****************** best_circuit_information ****************" << endl;
//    //best circuit information
//    mapper.FinalCircuit_info(mapper.Best_FinalCircuit, true);
//    mapper.write_output(mapper.Best_FinalCircuit);
//#endif

    measure.stop_clock("END Program");
    cout << endl <<  "*** Time & Memory Measure ***" << endl;
    measure.print_clock();
    measure.printMemoryUsage();



    return 0;
}
