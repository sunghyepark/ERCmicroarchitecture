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
    /////initial mapping
    enum class INITIALTYPE
    {
        IDENTITY_MAPPING,
        RANDOM_MAPPING,
        GRAPH_MATCHING_MIX_UPDATE,
        GRAPH_MATCHING_MIX,
        GRAPH_MATCHING_NUMBER,
        GRAPH_MATCHING_ORDER,
        NEW_GRAPH_MATCHING,
        TCAD_PROCESSING,
    };

    enum class INITIAL_GRAPH
    {
        MIX,
        NUMBER,
        ORDER
    };

    /////coupling graph
    enum class ARCHITECTURE
    {
        IBM_Q_London,
        IBM_Q_16_R,
        IBM_Q_16_Melbourne,
        IBM_Q_20,
        IBM_Q_20_Tokyo,
        IBM_Q_53,
        LINEAR_N,
        TOYEXAMPLE,
        TOYEXAMPLE_2
    };

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

        //graphFunction.cpp
        void build_dist_table();
        void print_dist_table();
        void delete_dist_table();
        /////////////////////
        void build_degree_table();
        void print_degree_table();
        void delete_degree_table();
        void build_degree_n();
        void build_doubledegree_n();
        void remove_degree_n(int n);
        /////////////////////
        int bfs(int start, int end);
        int bfs_weight(int start, int end);
        int cal_graph_center();
        int cal_graph_center_weight();
        pair<int, bool> graph_characteristics(bool i);
    };

    class QMapper
    {
        public:
        /////////////////////////// variables
            //file names
            string fileName_input;
            string fileName_output;
            string fileName_reverse;

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
            Circuit Dgraph_reverse;
            Circuit Dgraph_cnot;
            
            Graph interaction_subgraph_order;
            Graph interaction_subgraph_number;
            Graph interaction_mixgraph;
            
            //Coupling graph
            Graph coupling_graph;

            //Dlist
            vector<list<int> > Dlist;
            vector<list<int> > Dlist_all;

            //mapping
            map<int, int> layout_L;  //L[logical_qubit] = physical_qubit
            map<int, int> qubit_Q;   //Q[physical_qubit] = logical_qubit

            //map<int, int> layout_swapped;
            ////for post-processing
            map<int, int> new_layout_L;
            map<int, int> new_qubit_Q;
            
            //Final quantum circuit
            Circuit TempCircuit; //for test

            Circuit FinalCircuit; //final quantum circuit
            Circuit Best_FinalCircuit; //best final quantum circuit
            
            //from mapping.cpp
            int node_id;
            int originalDepth;

            int add_cnot_num;
            int add_swap_num;
            int add_bridge_num;
            int add_depth;
            vector<int> CircuitDepth;

            int least_cnot_num;     //least cnot_num
            int least_swap_num;     //least swap_num
            int least_bridge_num;   //least bridge_num
            int least_depth;   //least bridge_num

            //print.cpp
            void nodeset_out(ofstream &of, Circuit graph);
            void node_print(int N, Circuit graph);

        /////////////////////////// functions
            //parser.cpp
            void parsing(int argc, char** argv);
            void QASM_parse(string filename);

            //couplinggraph.cpp
            void select_coupling_graph(ARCHITECTURE archi);
            void build_graph_IBM_Q_London();        
            void build_graph_IBM_Q_16_R();          
            void build_graph_IBM_Q_16_Melbourne();  
            void build_graph_IBM_Q_20();            
            void build_graph_IBM_Q_20_Tokyo();      
            void build_graph_IBM_Q_53();            
            void build_graph_toyexample();          
            void build_graph_toyexample_2();          
            void build_graph_linear();

            //initial_mapping.cpp
            void initial_mapping(ARCHITECTURE archi, bool prepro, bool postpro, bool BRIDGE_MODE, INITIALTYPE initial_type);
            
            void graph_matching_processing_v2(ARCHITECTURE archi);
            void graph_matching_processing_v3(ARCHITECTURE archi, INITIAL_GRAPH graphtype);
            void graph_matching_processing_v4(ARCHITECTURE archi, INITIAL_GRAPH graphtype);
            void new_graph_matching_processing(ARCHITECTURE archi, INITIAL_GRAPH graphtype);
            
            void graph_matching_processing(ARCHITECTURE archi);
            void identical_mapping();
            void random_mapping();
            void pre_processing(ARCHITECTURE archi);
            
            void print_layout(const map<int, int> &layout);
            void print_qubit(const map<int, int> &qubit);
            void print_layout();
            void print_qubit();

            //mappingPreparation.cpp -> initial_mapping.cpp
            int sort_double_degree(vector<int>& candi_loc, Graph& graph);
            /////////////
            int sort_degree_return(vector<int>& candi_loc, Graph& graph);
            void sort_degree(vector<int>& candi_loc, Graph& graph);
            void sort_distance(int current_qc, vector<int>& candi_loc, vector<int>& ref_loc, Graph& coupling_graph, Graph& interaction_graph);
            
            void make_Dlist(Circuit& dgraph);
            void make_Dlist_all(Circuit& dgraph);
            void make_CNOT(bool i);
            Graph make_interactionGraph(bool i);
            Graph make_interactionNumberGraph(bool i);
            Graph make_interactionSubgraph_num(bool i, int n);
            Graph make_interactionSubgraph_order(bool i, int n);
            Graph make_interactionMixgraph(bool i, int n);
            
            void bfs_queue_gen(queue<int>& queue, Graph& graph, int start, bool order);
            void bfs_queue_gen_2(queue<int>& queue, Graph& graph_order, Graph& graph_num, int start);
            
            void make_ref_loc(vector<int>& ref_loc, Graph& graph, int start, bool order);
            void make_candi_loc(vector<int>& candi_loc, vector<int>& ref_loc, Graph& graph);
            void make_candi_loc_2(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2, int& degree);
            void make_candi_loc_3(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2, int& degree);
            void make_candi_loc_degree(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2);
            void make_candi_loc_dist(int current_qc, vector<int>& candi_loc, vector<int>& ref_loc, Graph& coupling_graph, Graph& interaction_graph, bool equal_order);
            
            void make_candi_loc_3(int current_q, Graph& graph, vector<pair<int,int>>& candi_loc, int& degree);


            //mapping.cpp
            void Circuit_Mapping(Circuit& dgraph, ARCHITECTURE archi, bool forward = false, bool prepro = false, bool postpro = false, bool BRIDGE_MODE=false);
            void update_front_n_act_list(list<int>& front_list, list<int>& act_list, vector<bool>& frozen);
            void find_singlequbit_list(int gateid, list<int>& singlequbit_list, Circuit& dgraph);
            void update_act_dist2_list(list<int>& act_dist2_list, list<int>& act_list, Circuit& dgraph);
            //bool check_direct_act_list(list<int>& act_list, vector<bool>& frozen, Circuit& dgraph, vector<int>& CircuitDepth);
            bool check_direct_act_list(list<int>& act_list, list<int>& singlequbit_list, vector<bool>& frozen, Circuit& dgraph);
            void generate_candi_list(list<int>& act_list, vector< pair< pair<int, int>, int> >& candi_list, Circuit& dgraph);
            void generate_candi_list_n(list<int>& act_list, vector< pair< pair<int, int>, int> >& candi_list, Circuit& dgraph);
            //void update_depth(&vector<int> vec_depth);

            //mappingFunction.cpp
            int cal_SWAP_effect(const int q1, const int q2, const int Q1, const int Q2);
            double cal_MCPE(const pair<int, int> p, Circuit& dgraph);
            double cal_MCPE_n(const pair<int, int> p, Circuit& dgraph, int actNum);
            int count_max(const vector< pair< pair<int, int>, pair<int, int> > >& v);
            pair<int, int> find_max_cost_SWAP(vector< pair< pair<int, int>, pair<int, int> > >& v, list<int>& act_list,
                                              vector< pair< pair<int, int>, pair<int, int> > >& history);
            void find_max_cost_SWAP_2(pair<int, int>& SWAP, int& gateid, double& max_cost,
                    vector< pair< pair<int, int>, pair<int, double> > >& v, list<int>& act_list,
                    vector< pair< pair<int, int>, pair<int, double> > >& history);

            void layout_swap(const int b1, const int b2);
            void layout_swap(const int b1, const int b2, map<int, int>& layout);
            void add_cnot(int c_qubit, int t_qubit, Circuit& graph);
            void add_swap(int b1, int b2, Circuit& graph);
            void add_bridge(int qs, int qt, Circuit& graph);

            //outputwriter.cpp
            void FinalCircuit_info(Circuit& graph, bool final_circuit = false);
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
/*
inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, int > > v)
{
    // <dist, weight> , nodeid(table idx)
    for(auto kv : v)
    {
        os << " dist: "              << setw(3) << kv.first.first;
        os << " weight: "            << setw(3) << kv.first.second;
        os << " nodeid(table idx): " << setw(3) << kv.second;
        os << endl;
    }
    return os;
}
*/

inline ostream& operator << (ostream& os, const pair<int, int> p)
{
    return os << "pair: " << p.first << " " << p.second;
}

inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, int> > v)
{
    for(auto kv : v)
        os << "SWAP: " << kv.first.first << " " << kv.first.second << " act_gate_id: " << kv.second << endl;
    return os;
}

inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, pair<int, int>> > v)
//inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, pair<int, double>> > v)
{
    for(auto kv : v)
    {
        os << "SWAP: " << kv.first.first << " " << kv.first.second;
        os << " act_gate_id: " << kv.second.first << " MCPE cost: " << kv.second.second << endl;
    }
    return os;
}

inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, pair<int, double>> > v)
//inline ostream& operator << (ostream& os, const vector< pair< pair<int, int>, pair<int, double>> > v)
{
    for(auto kv : v)
    {
        os << "SWAP: " << kv.first.first << " " << kv.first.second;
        os << " act_gate_id: " << kv.second.first << " MCPE cost: " << kv.second.second << endl;
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
