/*----------------------------------------------------------------------------------------------*/
/* Description:     Main source code for Quantum decoder                                        */
/*                                                                                              */
/* Author:          Sunghye Park, Postech                                                       */
/*                                                                                              */
/* Created:         10/12/2021 (for ERC project)                                                */
/*----------------------------------------------------------------------------------------------*/

#ifndef __CIRCUIT__
#define __CIRCUIT__ 0
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;


namespace Qcircuit
{
    enum class GATETYPE
    {
        U, RX, RZ, CNOT, X, Y, Z, H, S, SDG, T, TDG
    };

    ///Circuit info (node, edge) 
    struct Gate
    {
        int id; 
        int control, target;                //logical qubits //if single qubit control = -1
        GATETYPE type;
            
        //// use just in QASM // double theta, double phi, double lambda //
        char output_type[128];
    };
  
    struct Circuit
    {
        vector<Gate> nodeset;   /* gate set */
    };

    struct Node;
    struct Edge;

    struct Node
    {
        private:
        ////variables
        static int global_id;
        int id;
        int weight;
        public:
        set<int> edges;

        public:
        ////constructors
        Node();
        Node(int weight);

        ////getters
        int getglobalid();
        int getid();
        int getweight();
        void setweight(int weight);

        static void init_global_id();
        
    };

    struct Edge
    {
        private:
        ////variables
        static int global_id;
        int id;
        int weight;
        public:
        Node* source;
        Node* target;

        public:
        ////constructors
        Edge();
        Edge(Node* source, Node* target, int weight);

        ////getters
        int getglobalid();
        int getid();
        int getweight();
        int getsourceid();
        int gettargetid();
        void setweight(int weight);
        void setsource(Node* source);
        void settarget(Node* target);

        static void init_global_id();

    };

    struct Graph
    {
        ////graph node and edge
        map<int, Node> nodeset;
        map<int, Edge> edgeset;
        
        map<int, int> nodeid;       //table index <--> graph node id
        int node_size;
        int** dist;
        
        /////////////////////
        int** degree;
        int* degreeN;
        int* doubleDegreeN;
        /////////////////////


        int center;

        public:
        ////constructors
        Graph();

        ////methods
        //graph.cpp
        void init_global_id();
        int  addnode(int weight = 1);
        bool deletenode(int id);
        bool addedge(int source, int target, int weight = 1);
        bool deleteedge(int id);

    };

    class QMapper
    {
        public:
        /////////////////////////// variables
            //file names
            string fileName_input;
            string fileName_output;

            //cost parameters
            double param_alpha;
            int param_beta;
            int param_gamma;
            vector<double> cost_weight_table;

            //quantity of logical, physical qubits
            unsigned int nqubits;   //qubit quantity of quantum circuit (logical  qubits)
            unsigned int positions; //qubit quantity of couping graph   (physical qubits)
            
            //Quantum circuit
            Circuit Dgraph;
            
            //Dlist
            vector<list<int> > Dlist_all;

            Circuit FinalCircuit; //final quantum circuit
            
            //from mapping.cpp
            int node_id;
            //print.cpp
            void nodeset_out(ofstream &of, Circuit graph);
            void node_print(int N, Circuit graph);

        /////////////////////////// functions
            //parser.cpp
            void parsing(int argc, char** argv);
            void QASM_parse(string filename);

            //initial_mapping.cpp
            void initial_mapping();

            //mapping.cpp
            void Circuit_Mapping(Circuit& dgraph);

            //outputwriter.cpp
            void FinalCircuit_info(Circuit& graph);
            void write_output(Circuit& graph);
    };
}

//----------------------------- for debug ----------------------------//

using namespace Qcircuit;

inline ostream& operator << (ostream& os, Node n)
{
    os << "Node[" << setw(3) << n.getid() << "] | weight: " << setw(2) << n.getweight();
    if(!n.edges.empty())
    {
        os << " | edge ids: ";
        for(auto e : n.edges)
            os << setw(3) << e << " ";
    }
    return os;
}

inline ostream& operator << (ostream& os, Edge e)
{
    os << "Edge[" << setw(3) << e.getid() << "] | weight: " << setw(2) << e.getweight();
    os << " | " << setw(3) << e.getsourceid() << " <--> " << setw(3) << e.gettargetid();
    return os;
}

inline ostream& operator << (ostream& os, map<int, Node> nodeset)
{
    for(auto& kv : nodeset)
        os << kv.second << endl;
    return os;
}

inline ostream& operator << (ostream& os, map<int, Edge> edgeset)
{
    for(auto& kv : edgeset)
        os << kv.second << endl;
    return os;
}

inline ostream& operator << (ostream& os, Graph graph)
{
    os << "||Graph||" << endl;
    os << "||Node Set||" << endl;
    os << graph.nodeset << endl;
    os << "||Edge Set||" << endl;
    os << graph.edgeset << endl;
    return os;
}

inline ostream& operator << (ostream& os, const GATETYPE& type)
{
    switch(type)
    {
        case GATETYPE::U    : return os << "U"  ;
        case GATETYPE::RX   : return os << "RX" ;
        case GATETYPE::RZ   : return os << "RZ" ;
        case GATETYPE::CNOT : return os << "CX" ;
        case GATETYPE::X    : return os << "X"  ;
        case GATETYPE::Y    : return os << "Y"  ;
        case GATETYPE::Z    : return os << "Z"  ;
        case GATETYPE::H    : return os << "H"  ;
        case GATETYPE::S    : return os << "S"  ;
        case GATETYPE::SDG  : return os << "SDG";
        case GATETYPE::T    : return os << "T"  ;
        case GATETYPE::TDG  : return os << "TDG";
        default             : return os;
    }
}

inline ostream& operator << (ostream& os, const Gate& gate)
{
    os << "Gate";
    if(gate.control == -1) //U, RX, RZ gate
        os << "(" << setw(5) << gate.id << ")\t" << setw(2) << gate.type << " " << "q[" << gate.target << "]";
    else //CNOT gate
        os << "(" << setw(5) << gate.id << ")\t" << setw(2) << gate.type << " " << "q[" << gate.control << "], q[" << gate.target << "]";
    return os;   
}

inline ostream& operator << (ostream& os, const vector<Gate>& nodeset)
{
    for(auto gate : nodeset)
        os << gate << endl;
    return os;
}

inline ostream& operator << (ostream& os, const queue<int> q)
{
    queue<int> test = q;
    while(!test.empty())
    {
        os << test.front() << " ";
        test.pop();
    }
    return os;
}


inline ostream& operator << (ostream& os, const vector<list<int>>& list)
{
    for(int n=0; n<list.size(); n++)
    {
        os << "n[" << setw(4) << n << "]: " << "size: " << list[n].size() << " ";
        for(auto ll : list[n])
        {
            os << "-> " << ll; 
        }
        os << endl;
    }
    return os;
}

inline ostream& operator << (ostream& os, const map<int, vector<int>>& edgemap)
{
    for(auto it : edgemap)
    {
        vector<int> ids = it.second;
        os << "Gate[" << setw(5) << it.first << "]: ";
        for(auto id : ids)
        {
            os << setw(5) << id << " ";
        }
        os << endl;
    }
    return os;
}

inline ostream& operator << (ostream& os, const vector<int>& list)
{
    for(auto v : list)
        os << v << " ";
    return os;
}

inline ostream& operator << (ostream& os, const vector<bool>& list)
{
    for(auto v : list)
        os << v << " ";
    return os;
}

inline ostream& operator << (ostream& os, const map<int, int>& m)
{
    for(auto kv : m)
        os << "map[" << setw(3) << kv.first << "] = " << kv.second << endl;
    return os;
}

inline ostream& operator << (ostream& os, const list<int>& llist)
{
    for(auto v : llist)
        os << v << " ";
    return os;
}

inline ostream& operator << (ostream& os, const vector<pair<int, int> >& llist)
{
    os << "{ ";
    for(auto v : llist)
        os << "(" << v.first << ", " << v.second << ")  ";
    os << "}" << endl;
    return os;
}

inline ostream& operator << (ostream& os, const vector<vector<int>>& list)
{
    for(auto v : list)
        os << v << endl;
    return os;
}


#endif
