
#include "circuit.h"

using namespace std;
using namespace Qcircuit;

void Qcircuit::QMapper::FinalCircuit_info(Circuit& graph)
{
    //final circuit
    cout << "*** Final Circuit ***" << endl;
    cout << graph.nodeset << endl;
    cout << " ==> Print Final Circuit Successfully Finished" << endl;
}    
    


void Qcircuit::QMapper::write_output(Circuit& graph)
{
    ofstream of(fileName_output);
    //of << "========== output file ==========" << endl;
    of << "OPEMQASM 2.0;" << endl;
    of << "include \"qelib1.inc\";" << endl;
    //of << "qreg q[16];" << endl; 
    //of << "creg c[16];" << endl; 
    of << "qreg q[" << nqubits << "];" << endl; 
    of << "creg c[" << nqubits << "];" << endl; 
    
    //nodeset_out(of, FinalCircuit);
    for (auto g : graph.nodeset)
    {
        if(g.control==-1)
        {
            of << g.type <<  g.output_type << " " << "q[" << g.target << "];" << endl;
        }
        else
        {
            of << g.type << " " << "q[" << g.control << "],q[" << g.target << "];" << endl; 
        }
    }
    //cout << " Nodeset out is done" << endl << endl;
    
    of.close();

#if PRINT_OUTPUT_info
    cout << " ==> Output File (.qasm) Is Successfully Generated" << endl;
#endif

}

