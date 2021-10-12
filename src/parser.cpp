//parser.cpp

#include "circuit.h"
#include "QASMtoken.hpp"
#include "QASMscanner.hpp"
#include "QASMparser.h"

#define PRINT_OPTION 1
#define PRINT_NODE 1
#define DEBUG_PARSER 1

using namespace std;
using namespace Qcircuit;

void Qcircuit::QMapper::parsing(int argc, char** argv)
{
    if(argc != 3)
    {
        cout << "ERROR: invalid arguments" << endl;
        exit(1);
    }

    fileName_input   = argv[1];
    fileName_output  = argv[2];


#if PRINT_OPTION
    cout << "*** Option ***" << endl;
    cout << "Input file   : "  << fileName_input << endl;
    cout << "Output file  : "  << fileName_output << endl;
    cout << endl;
#endif

    QASM_parse(fileName_input);
}

void Qcircuit::QMapper::QASM_parse(string filename)
{
    //Graph selection (post processing)
    Circuit* circuit;
#if DEBUG_PARSER
        cout << "*** Parsing Input File ***" << endl;
#endif
        circuit = &Dgraph;

    //Start Parser
    QASMparser* parser = new QASMparser(filename);
    parser->Parse();
    nqubits = parser->getNqubits();
    circuit->nodeset = parser->getGatelists();
#if PRINT_NODE
    cout << endl << "*** Node set of Dgraph ***" << endl;
    cout << circuit->nodeset << endl;
#endif
    delete parser;
}
