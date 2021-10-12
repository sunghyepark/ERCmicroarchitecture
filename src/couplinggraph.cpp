//couplinggraph.cpp
#include "circuit.h"
#define PRINT_coupling_graph_name 0
using namespace std;
using namespace Qcircuit;

void Qcircuit::QMapper::select_coupling_graph(ARCHITECTURE archi)
{
    switch(archi)
    {
        case ARCHITECTURE::IBM_Q_London:
            build_graph_IBM_Q_London();
            break;
        case ARCHITECTURE::IBM_Q_16_R:
            build_graph_IBM_Q_16_R();
            break;
        case ARCHITECTURE::IBM_Q_16_Melbourne:
            build_graph_IBM_Q_16_Melbourne();
            break;
        case ARCHITECTURE::IBM_Q_20:
            build_graph_IBM_Q_20();
            break;
        case ARCHITECTURE::IBM_Q_20_Tokyo:
            build_graph_IBM_Q_20_Tokyo();
            break;
        case ARCHITECTURE::IBM_Q_53:
            build_graph_IBM_Q_53();
            break;
        case ARCHITECTURE::LINEAR_N:
            build_graph_linear();
            break;
        case ARCHITECTURE::TOYEXAMPLE:
            build_graph_toyexample();
            break;
        case ARCHITECTURE::TOYEXAMPLE_2:
            build_graph_toyexample_2();
            break;
        default:
            cout << "No architecture specified!" << endl;
            exit(1);
    }
}

// ****************  Integration 2020 ("Optimization of Quantum Circuit ...")  **************** //
//build a graph representing the coupling map of IBM Q 16 Rueschlikon V1.0.0 (ibmqx3)
void Qcircuit::QMapper::build_graph_IBM_Q_16_R()
{    
    positions = 16;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(3, 14);
    coupling_graph.addedge(15, 0);
    coupling_graph.addedge(15, 14);
    coupling_graph.addedge(4, 3);
    coupling_graph.addedge(4, 5);
    coupling_graph.addedge(13, 4);
    coupling_graph.addedge(13, 14);
    coupling_graph.addedge(12, 13);
    coupling_graph.addedge(12, 5);
    coupling_graph.addedge(12, 11);
    coupling_graph.addedge(6, 11);
    coupling_graph.addedge(6, 7);
    coupling_graph.addedge(11, 10);
    coupling_graph.addedge(7, 10);
    coupling_graph.addedge(8, 7);
    coupling_graph.addedge(9, 8);
    coupling_graph.addedge(9, 10);
    cout <<  " # This graph is: IBM Q 16 Rueschlikon V1.0.0 (ibmqx3)" << endl << endl;
}

// ****************  ASPLOS2019, TCAD2019  **************** //
//build a graph representing the coupling map of IBM Q 20
void Qcircuit::QMapper::build_graph_IBM_Q_20()
{   
    positions = 20;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(0, 5);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 6);
    coupling_graph.addedge(1, 7);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(2, 6);
    coupling_graph.addedge(2, 7);
    coupling_graph.addedge(3, 4);
    coupling_graph.addedge(3, 8);
    coupling_graph.addedge(3, 9);
    coupling_graph.addedge(4, 8);
    coupling_graph.addedge(4, 9);
    coupling_graph.addedge(5, 6);
    coupling_graph.addedge(5, 10);
    coupling_graph.addedge(5, 11);
    coupling_graph.addedge(6, 7);
    coupling_graph.addedge(6, 10);
    coupling_graph.addedge(6, 11);
    coupling_graph.addedge(7, 8);
    coupling_graph.addedge(7, 12);
    coupling_graph.addedge(7, 13);
    coupling_graph.addedge(8, 9);
    coupling_graph.addedge(8, 12);
    coupling_graph.addedge(8, 13);
    coupling_graph.addedge(9, 14);
    coupling_graph.addedge(10, 11);
    coupling_graph.addedge(10, 15);
    coupling_graph.addedge(11, 12);
    coupling_graph.addedge(11, 16);
    coupling_graph.addedge(11, 17);
    coupling_graph.addedge(12, 13);
    coupling_graph.addedge(12, 16);
    coupling_graph.addedge(12, 17);
    coupling_graph.addedge(13, 14);
    coupling_graph.addedge(13, 18);
    coupling_graph.addedge(13, 19);
    coupling_graph.addedge(14, 18);
    coupling_graph.addedge(14, 19);
    coupling_graph.addedge(15, 16);
    coupling_graph.addedge(16, 17);
    coupling_graph.addedge(17, 18);
    coupling_graph.addedge(18, 19);
#if PRINT_coupling_graph_name
    cout <<  " # This graph is: IBM Q 20" << endl << endl;
#endif
}

//build a graph representing the coupling map of IBM Q 20 Tokyo
void Qcircuit::QMapper::build_graph_IBM_Q_20_Tokyo()
{    
    positions = 20;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(0, 5);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 6);
    coupling_graph.addedge(1, 7);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(2, 6);
    coupling_graph.addedge(3, 8);
    coupling_graph.addedge(3, 9);
    coupling_graph.addedge(4, 8);
    coupling_graph.addedge(4, 9);
    coupling_graph.addedge(5, 6);
    coupling_graph.addedge(5, 10);
    coupling_graph.addedge(5, 11);
    coupling_graph.addedge(6, 7);
    coupling_graph.addedge(6, 10);
    coupling_graph.addedge(6, 11);
    coupling_graph.addedge(7, 8);
    coupling_graph.addedge(7, 12);
    coupling_graph.addedge(7, 13);
    coupling_graph.addedge(8, 9);
    coupling_graph.addedge(8, 12);
    coupling_graph.addedge(8, 13);
    coupling_graph.addedge(10, 11);
    coupling_graph.addedge(10, 15);
    coupling_graph.addedge(11, 12);
    coupling_graph.addedge(11, 16);
    coupling_graph.addedge(11, 17);
    coupling_graph.addedge(12, 13);
    coupling_graph.addedge(12, 16);
    coupling_graph.addedge(13, 14);
    coupling_graph.addedge(13, 18);
    coupling_graph.addedge(13, 19);
    coupling_graph.addedge(14, 18);
    coupling_graph.addedge(14, 19);
    coupling_graph.addedge(15, 16);
    coupling_graph.addedge(16, 17);
    cout <<  " # This graph is: IBM Q 20 Tokyo" << endl << endl;
}

//build a graph representing the coupling map of IBM Q 53
void Qcircuit::QMapper::build_graph_IBM_Q_53()
{    
    positions = 53;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(0, 5);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(3, 4);
    coupling_graph.addedge(4, 6);
    coupling_graph.addedge(5, 9);
    coupling_graph.addedge(6, 13);
    coupling_graph.addedge(7, 8);
    coupling_graph.addedge(7, 16);
    coupling_graph.addedge(8, 9);
    coupling_graph.addedge(9, 10);
    coupling_graph.addedge(10, 11);
    coupling_graph.addedge(11, 12);
    coupling_graph.addedge(11, 17);
    coupling_graph.addedge(12, 13);
    coupling_graph.addedge(13, 14);
    coupling_graph.addedge(14, 15);
    coupling_graph.addedge(15, 18);
    coupling_graph.addedge(16, 19);
    coupling_graph.addedge(17, 23);
    coupling_graph.addedge(18, 27);
    coupling_graph.addedge(19, 20);
    coupling_graph.addedge(20, 21);
    coupling_graph.addedge(21, 22);
    coupling_graph.addedge(21, 28);
    coupling_graph.addedge(22, 23);
    coupling_graph.addedge(23, 24);
    coupling_graph.addedge(24, 25);
    coupling_graph.addedge(25, 26);
    coupling_graph.addedge(25, 29);
    coupling_graph.addedge(26, 27);
    coupling_graph.addedge(28, 32);
    coupling_graph.addedge(29, 36);
    coupling_graph.addedge(30, 31);
    coupling_graph.addedge(30, 39);
    coupling_graph.addedge(31, 32);
    coupling_graph.addedge(32, 33);
    coupling_graph.addedge(33, 34);
    coupling_graph.addedge(34, 35);
    coupling_graph.addedge(34, 40);
    coupling_graph.addedge(35, 36);
    coupling_graph.addedge(36, 37);
    coupling_graph.addedge(37, 38);
    coupling_graph.addedge(38, 41);
    coupling_graph.addedge(39, 42);
    coupling_graph.addedge(40, 46);
    coupling_graph.addedge(41, 50);
    coupling_graph.addedge(42, 43);
    coupling_graph.addedge(43, 44);
    coupling_graph.addedge(44, 45);
    coupling_graph.addedge(44, 51);
    coupling_graph.addedge(45, 46);
    coupling_graph.addedge(46, 47);
    coupling_graph.addedge(47, 48);
    coupling_graph.addedge(48, 49);
    coupling_graph.addedge(48, 52);
    coupling_graph.addedge(49, 50);
    cout <<  " # This graph is: IBM Q 53 Rochester" << endl << endl;
}

//build a graph representing the coupling map of IBM Q 16 Melbourne
void Qcircuit::QMapper::build_graph_IBM_Q_16_Melbourne()
{    
    positions = 16;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(0, 14);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 13);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(2, 12);
    coupling_graph.addedge(3, 4);
    coupling_graph.addedge(3, 11);
    coupling_graph.addedge(4, 5);
    coupling_graph.addedge(4, 10);
    coupling_graph.addedge(5, 6);
    coupling_graph.addedge(5, 9);
    coupling_graph.addedge(6, 8);
    coupling_graph.addedge(7, 8);
    coupling_graph.addedge(8, 9);
    coupling_graph.addedge(9, 10);
    coupling_graph.addedge(10, 11);
    coupling_graph.addedge(11, 12);
    coupling_graph.addedge(12, 13);
    coupling_graph.addedge(13, 14);
    cout <<  " # This graph is: IBM Q 16 Melbourne" << endl << endl;
}

//build a IBM q Londone
void Qcircuit::QMapper::build_graph_IBM_Q_London()
{    
    positions = 5;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 3);
    coupling_graph.addedge(3, 4);
    cout <<  " # This graph is: IBM q London" << endl << endl;
}

//build a graph_example
void Qcircuit::QMapper::build_graph_toyexample()
{    
    //positions = 4;
    //for(int i=0; i<positions; i++)
    //    coupling_graph.addnode();
    //coupling_graph.addedge(1, 2);
    //coupling_graph.addedge(2, 3);
    //coupling_graph.addedge(2, 4);
    positions = 5;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 2);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 4);
    coupling_graph.addedge(2, 3);
    coupling_graph.addedge(2, 4);
    coupling_graph.addedge(3, 4);
    cout <<  " # This graph is: example architecture_1" << endl << endl;
}

//build a graph_example_2
void Qcircuit::QMapper::build_graph_toyexample_2()
{    
    positions = 6;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    coupling_graph.addedge(0, 1);
    coupling_graph.addedge(0, 3);
    coupling_graph.addedge(1, 2);
    coupling_graph.addedge(1, 4);
    coupling_graph.addedge(2, 5);
    coupling_graph.addedge(3, 4);
    coupling_graph.addedge(4, 5);
    cout <<  " # This graph is: example architecture_2" << endl << endl;
}

//build a graph representing the linear NN structure
void Qcircuit::QMapper::build_graph_linear()
{
    positions = nqubits;
    for(int i=0; i<positions; i++)
        coupling_graph.addnode();
    for(int i=0; i<nqubits-1; i++)
        coupling_graph.addedge(i, i+1);
    cout <<  " # This graph is: linear NN graph" << endl << endl;
}
