//mapping preparation.cpp
#include "circuit.h"

#define DEBUG 0
#define DEBUG_loc 0
#define DEBUG_mapping_preparation 0

#define DEBUG_NEW_INITIAL_D 0

#define Mixgraph_exponential 1
#define Mixgraph_linear 0


using namespace std;
using namespace Qcircuit;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool compare_cost(pair<int, int> a, pair<int, int> b)
{
    return a.first < b.first;
}

bool compare_cost_d(pair<int, int> a, pair<int, int> b)
{
    return a.first > b.first;
}

bool compare_graph_bfs(pair< pair<int, int>, int> a, pair< pair<int, int>, int> b)
{
    if(a.first.first == b.first.first)
        return a.first.second < b.first.second;
    else
        return a.first.first < b.first.first;
}

bool compare_graph_bfs_number(pair< pair<int, int>, int> a, pair< pair<int, int>, int> b)
{
    if(a.first.first == b.first.first)
        return a.first.second > b.first.second;
    else
        return a.first.first < b.first.first;
}


bool compare(const pair<int, int>& a, const pair<int, int>& b)
{
    //if the first number is same
    if(a.first == b.first)
        return a.second < b.second;
    return a.first > b.first;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Qcircuit::QMapper::sort_double_degree(vector<int>& candi_loc, Graph& graph)
{
#if DEBUG_NEW_INITIAL_D
    cout << "* sort_degree *" << endl;
#endif
    vector<pair<int, int>> degree_candi_loc;
    vector<int> temp_candi_loc;
    int degree;
    int min_dist = 1;
    int return_degree = 0;

#if DEBUG_NEW_INITIAL_D
    cout << "initial candi loc: ";
#endif
    for(auto v : candi_loc)
    {
        degree = graph.doubleDegreeN[v];
#if DEBUG_NEW_INITIAL_D
        cout << v << "(degree: " << degree << "), ";
#endif
        degree_candi_loc.push_back(make_pair(degree, v));
        return_degree += degree;
    }
#if DEBUG_NEW_INITIAL_D
    cout << endl;
#endif
    
    sort(degree_candi_loc.begin(), degree_candi_loc.end(), compare);
    for(auto v : degree_candi_loc)
        temp_candi_loc.push_back(v.second);
    candi_loc = temp_candi_loc;

    return return_degree;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Qcircuit::QMapper::sort_degree_return(vector<int>& candi_loc, Graph& graph)
{
#if DEBUG_loc
    cout << "* sort_degree *" << endl;
#endif
    vector<pair<int, int>> degree_candi_loc;
    vector<int> temp_candi_loc;
    int degree;
    int min_dist = 1;
    int return_degree = 0;

#if DEBUG_loc
    cout << "initial candi loc: ";
#endif
    for(auto v : candi_loc)
    {

        degree = 0;
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == v) continue;
            if(graph.dist[v][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                degree++;
        }
#if DEBUG_loc
        cout << v << "(degree: " << degree << "), ";
#endif        
        degree_candi_loc.push_back(make_pair(degree, v));
        return_degree += degree;
    }
    cout << endl;
    



    sort(degree_candi_loc.begin(), degree_candi_loc.end(), compare);
    for(auto v : degree_candi_loc)
    {
        temp_candi_loc.push_back(v.second);
    }
    candi_loc = temp_candi_loc;

    return return_degree;
}

void Qcircuit::QMapper::sort_degree(vector<int>& candi_loc, Graph& graph)
{
#if DEBUG_loc
    cout << "* sort_degree *" << endl;
#endif
    vector<pair<int, int>> degree_candi_loc;
    vector<int> temp_candi_loc;
    int degree;
    int min_dist = 1;
    
#if DEBUG_loc
    cout << "initial candi loc: ";
#endif
    for(auto v : candi_loc)
    {

        degree = 0;
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == v) continue;
            if(graph.dist[v][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                degree++;
        }
#if DEBUG_loc
        cout << v << "(degree: " << degree << "), ";
#endif        
        degree_candi_loc.push_back(make_pair(degree, v));
    }
    cout << endl;
    



    sort(degree_candi_loc.begin(), degree_candi_loc.end(), compare);
    for(auto v : degree_candi_loc)
    {
        temp_candi_loc.push_back(v.second);
    }
    candi_loc = temp_candi_loc;
}

void Qcircuit::QMapper::sort_distance(int current_qc, vector<int>& candi_loc, vector<int>& ref_loc, Graph& coupling_graph, Graph& interaction_graph)
{
#if DEBUG_loc
    cout << endl;
    cout << "* sort_distance *" << endl;
#endif
    vector<pair<int, int>> distance_candi_loc;
    vector<int> initial_candi_loc;
    int distance_sum, weight;


    for(int i = 0; i < coupling_graph.node_size; i++)
    {
        if(qubit_Q.count(i) && qubit_Q[i] == -1)
            initial_candi_loc.push_back(i);
    }
#if DEBUG_loc
    cout << "initial candi loc: " << endl;
#endif

    for(auto v : initial_candi_loc)
    {
        cout << "* v: " << v << endl;
        distance_sum = 0;
        int qubit_r, weight, distance;
        for(auto r : ref_loc)
        {
            qubit_r = qubit_Q[r];
            cout << "current_qc: " << current_qc << ", qubit_r: " << qubit_r << endl;
            for(auto edge : interaction_graph.edgeset)
            {
                const int sourceid  = edge.second.getsourceid();
                const int targetid  = edge.second.gettargetid();
                if(sourceid == qubit_r && targetid == current_qc)
                    weight = edge.second.getweight();
                else if(sourceid == current_qc && targetid == qubit_r)
                    weight = edge.second.getweight();
                else continue;
            }
            distance = coupling_graph.dist[v][r];
            cout << "- " << r << " (dist: " << distance << ", weight: " << weight << endl;
            distance_sum += weight * distance;
        }
        distance_candi_loc.push_back(make_pair(distance_sum, v));
#if DEBUG_loc
        cout << "(dist_sum: " << distance_sum << ")" << endl;
#endif        
    }



    sort(distance_candi_loc.begin(), distance_candi_loc.end(), compare_cost);
    initial_candi_loc.clear();
    for(auto v : distance_candi_loc)
    {
        initial_candi_loc.push_back(v.second);
    }
    candi_loc = initial_candi_loc;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Qcircuit::QMapper::make_Dlist(Circuit& dgraph)
{
#if DEBUG_mapping_preparation
    cout << "*** Dlist (only CNOT) generated ***" << endl;
#endif
    Dlist.clear(); 
    for(int i = 0; i<nqubits; i++)
    {
        list<int> temp;
        Dlist.push_back(temp);
    }

    for(const auto& gate : dgraph.nodeset)
    {
        if(gate.type != GATETYPE::CNOT) continue;   //only cnot --> dlist
        const int   id      = gate.id;
        const int   target  = gate.target;
        const int   control = gate.control;

        Dlist[target].push_back(id);
        Dlist[control].push_back(id);
    }
#if DEBUG
    cout << Dlist << endl;
#endif
}

void Qcircuit::QMapper::make_Dlist_all(Circuit& dgraph)
{
#if DEBUG_mapping_preparation
    cout << "*** Dlist (all) generated ***" << endl;
#endif
    Dlist_all.clear(); 
    for(int i = 0; i<nqubits; i++)
    {
        list<int> temp;
        Dlist_all.push_back(temp);
    }

    for(const auto& gate : dgraph.nodeset)
    {
        if(gate.type == GATETYPE::CNOT)   // cnot --> dlist
        {
            const int   id      = gate.id;
            const int   target  = gate.target;
            const int   control = gate.control;

            Dlist_all[target].push_back(id);
            Dlist_all[control].push_back(id);
        }
        else   // single-qubit gate --> dlist
        {
            const int   id      = gate.id;
            const int   target  = gate.target;
            //control = -1
            Dlist_all[target].push_back(id);
        }
    }
#if DEBUG
    cout << Dlist_all << endl;
#endif
}

void Qcircuit::QMapper::make_CNOT(bool i)
{
    Dgraph_cnot.nodeset.clear();

    vector<int> CircuitDepth2(nqubits, 0);
    if(i)
    {
        //cout << " ** CNOT set ** " << endl;
        for(const auto& gate : Dgraph.nodeset)
        {
            if(gate.type != GATETYPE::CNOT) continue;
            const int target  = gate.target;
            const int control = gate.control;
            
            add_cnot(control, target, Dgraph_cnot);
            add_cnot_num--;

            //make original Depth
            int depth_t = CircuitDepth2[target];
            int depth_c = CircuitDepth2[control];
            //int maxDepth = (depth_t >= depth_c) ? depth_t : depth_c;
            int maxDepth = max(depth_t, depth_c);
            CircuitDepth2[control] = maxDepth + 1;
            CircuitDepth2[target]  = maxDepth + 1;
        }
    }
    originalDepth = *max_element(CircuitDepth2.begin(), CircuitDepth2.end());
}

Graph Qcircuit::QMapper::make_interactionGraph(bool i)
{
#if DEBUG_mapping_preparation
    cout << "*** Generate Interaction graph ***" << endl;
#endif
    
    bool** gen_edge = new bool*[nqubits]; //edge generated (for interaction graph)
    for(int i = 0; i < nqubits; i++)
        gen_edge[i] = new bool[nqubits];

    //gen_edge initialize
    for(int i = 0; i < nqubits; i++)
        for(int j = 0; j < nqubits; j++)
            if(i == j)
                gen_edge[i][j] = true;
            else
                gen_edge[i][j] = false;
    Graph g;
    for(int i = 0; i < nqubits; i++)
        g.addnode();
    //for(const auto& gate : Dgraph.nodeset)
    for(const auto& gate : Dgraph_cnot.nodeset)
    {
        if(gate.type != GATETYPE::CNOT) continue;
        const int id = gate.id;
        const int target = gate.target;
        const int control = gate.control;
        if(gen_edge[target][control]) continue;
        g.addedge(target, control, id);
        gen_edge[target][control] = true;
        gen_edge[control][target] = true;
    }
    
    for(int i = 0; i < nqubits; i++)
        delete[] gen_edge[i];
    delete[] gen_edge;

    return g;
}

Graph Qcircuit::QMapper::make_interactionNumberGraph(bool i)
{
#if DEBUG_mapping_preparation
    cout << "*** Generate Interaction Number graph ***" << endl;
#endif

    bool** gen_edge = new bool*[nqubits]; //edge generated (for interaction graph)
    for(int i = 0; i < nqubits; i++)
        gen_edge[i] = new bool[nqubits];

    //gen_edge initialize
    for(int i = 0; i < nqubits; i++)
        for(int j = 0; j < nqubits; j++)
            if(i == j)
                gen_edge[i][j] = true;
            else
                gen_edge[i][j] = false;

    Graph g;
    for(int i = 0; i < nqubits; i++)
        g.addnode();
    //for(const auto& gate : Dgraph.nodeset)
    for(const auto& gate : Dgraph_cnot.nodeset)
    {
        if(gate.type != GATETYPE::CNOT) continue;
        const int id = gate.id;
        const int target = gate.target;
        const int control = gate.control;
        if(gen_edge[target][control])
        {
            for(auto& e : g.edgeset)
            {
                if(e.second.gettargetid()==target && e.second.getsourceid()==control)
                {
                    //cout << "weight["<< e.second.gettargetid() << "]["<< e.second.getsourceid() << "]: " << e.second.getweight() << endl;
                    int weight = e.second.getweight();
                    e.second.setweight(weight+1);
                }
                else if(e.second.gettargetid()==control && e.second.getsourceid()==target)
                {
                    //cout << "weight["<< e.second.gettargetid() << "]["<< e.second.getsourceid() << "]: " << e.second.getweight() << endl;
                    int weight = e.second.getweight();
                    e.second.setweight(weight+1);
                }

                else continue;
            }
            //int weight = gen_edge[target][control].getweight();
            //cout << "weight: " << weight << endl;
        }
        else
        g.addedge(target, control, 1);
        gen_edge[target][control] = true;
        gen_edge[control][target] = true;

    }
    
    for(int i = 0; i < nqubits; i++)
        delete[] gen_edge[i];
    delete[] gen_edge;

    return g;
}

Graph Qcircuit::QMapper::make_interactionSubgraph_num(bool i, int n)
{
#if DEBUG_mapping_preparation
    cout << "*** Generate Interaction Sub_Number graph ***" << endl;
#endif

    bool** gen_edge = new bool*[nqubits]; //edge generated (for interaction graph)
    for(int i = 0; i < nqubits; i++)
        gen_edge[i] = new bool[nqubits];

    //gen_edge initialize
    for(int i = 0; i < nqubits; i++)
        for(int j = 0; j < nqubits; j++)
            if(i == j)
                gen_edge[i][j] = true;
            else
                gen_edge[i][j] = false;

    Graph g;
    for(int i = 0; i < nqubits; i++)
        g.addnode();


    int m = Dgraph_cnot.nodeset.size() / n;
    //int m = param_beta;
    int remainder = Dgraph_cnot.nodeset.size() % n;
    for(int i=0; i<n; i++)
    {
        //cout << "i: " << i << endl;
        //make sub_interaction num graph
        for(int id = m*i; id< m*i+m; id++)
        {
            const int target  = Dgraph_cnot.nodeset[id].target;
            const int control = Dgraph_cnot.nodeset[id].control;
            if(gen_edge[target][control]) continue;
            gen_edge[target][control] = true;
            gen_edge[control][target] = true;
        }
        if(i==n-1 && remainder!=0)
        {
            for(int id = m*n; id<Dgraph_cnot.nodeset.size(); id++)
            {
                const int target  = Dgraph_cnot.nodeset[id].target;
                const int control = Dgraph_cnot.nodeset[id].control;
                if(gen_edge[target][control]) continue;
                gen_edge[target][control] = true;
                gen_edge[control][target] = true;
            }
        }
        //add edge
        for(int target=0; target<nqubits; target++)
        {
            for(int control=target+1; control<nqubits; control++)
            {
                bool completed = false;
                if(gen_edge[target][control])
                {
                    //cout << "target: " << target << ", control: " << control << endl;
                    for(auto& e : g.edgeset)
                    {
                        if(e.second.gettargetid()==target && e.second.getsourceid()==control)
                        {
                            //cout << "weight["<< e.second.gettargetid() << "]["<< e.second.getsourceid() << "]: " << e.second.getweight() << endl;
                            int weight = e.second.getweight();
                            e.second.setweight(weight+1);
                            completed = true;
                        }
                        else if(e.second.gettargetid()==control && e.second.getsourceid()==target)
                        {
                            //cout << "weight["<< e.second.gettargetid() << "]["<< e.second.getsourceid() << "]: " << e.second.getweight() << endl;
                            int weight = e.second.getweight();
                            e.second.setweight(weight+1);
                            completed = true;
                        }
                        else continue;
                    }
                    if(completed == false)
                        g.addedge(target, control, 1);
                }
            }
        }
        //gen_edge initialize
        for(int i = 0; i < nqubits; i++)
            for(int j = 0; j < nqubits; j++)
                if(i == j)
                    gen_edge[i][j] = true;
                else
                    gen_edge[i][j] = false;
    }
    for(int i = 0; i < nqubits; i++)
        delete[] gen_edge[i];
    delete[] gen_edge;

    return g;
}

Graph Qcircuit::QMapper::make_interactionSubgraph_order(bool i, int n)
{
#if DEBUG_mapping_preparation
    cout << "*** Generate Interaction Sub_Order graph ***" << endl;
#endif

    bool** gen_edge = new bool*[nqubits]; //edge generated (for interaction graph)
    for(int i = 0; i < nqubits; i++)
        gen_edge[i] = new bool[nqubits];

    //gen_edge initialize
    for(int i = 0; i < nqubits; i++)
        for(int j = 0; j < nqubits; j++)
            if(i == j)
                gen_edge[i][j] = true;
            else
                gen_edge[i][j] = false;

    Graph g;
    for(int i = 0; i < nqubits; i++)
        g.addnode();


    int m = Dgraph_cnot.nodeset.size() / n;
    //int m = param_beta;
    int remainder = Dgraph_cnot.nodeset.size() % n;
    for(int i=0; i<n; i++)
    {
        //cout << "i: " << i << endl;
        //make sub_interaction num graph
        for(int id = m*i; id< m*i+m; id++)
        {
            const int target  = Dgraph_cnot.nodeset[id].target;
            const int control = Dgraph_cnot.nodeset[id].control;
            if(gen_edge[target][control]) continue;
            g.addedge(target, control, i+1);
            gen_edge[target][control] = true;
            gen_edge[control][target] = true;
        }
        if(i==n-1 && remainder!=0)
        {
            for(int id = m*n; id<Dgraph_cnot.nodeset.size(); id++)
            {
                const int target  = Dgraph_cnot.nodeset[id].target;
                const int control = Dgraph_cnot.nodeset[id].control;
                if(gen_edge[target][control]) continue;
                gen_edge[target][control] = true;
                gen_edge[control][target] = true;
            }
        }
    }
    for(int i = 0; i < nqubits; i++)
        delete[] gen_edge[i];
    delete[] gen_edge;

    return g;
}

Graph Qcircuit::QMapper::make_interactionMixgraph(bool i, int n)
{
#if DEBUG_mapping_preparation
    cout << "*** Generate Interaction Mix graph ***" << endl;
#endif
    
    bool** gen_edge = new bool*[nqubits]; //edge generated (for interaction graph)
    for(int i = 0; i < nqubits; i++)
        gen_edge[i] = new bool[nqubits];

    //gen_edge initialize
    for(int i = 0; i < nqubits; i++)
        for(int j = 0; j < nqubits; j++)
            if(i == j)
                gen_edge[i][j] = true;
            else
                gen_edge[i][j] = false;

    Graph g;
    for(int i = 0; i < nqubits; i++)
        g.addnode();

    int dgraphSize = Dgraph_cnot.nodeset.size();
    int m = dgraphSize / n;
    int remainder = dgraphSize % n;
    //cout << "Dgraph_nodeset.size: " << dgraphSize << endl;
    //cout << "Division size (n): " << n << endl;
    //cout << "Share size (m): " << m << endl;
    //cout << "Remainder size: " << remainder << endl;
    
    vector<double> costWeightTable;
    double costParam = 0.9;
    double cost = 1.0;
#if Mixgraph_exponential
    costWeightTable.clear();
    for(int i=0; i<n+1; i++)
    {
        costWeightTable.push_back(cost);
        cost *= costParam;
    }
    
#endif
//#if Mixgraph_linear
//    costWeightTable.clear();
//    for(int i=0; i<n; i++)
//    {
//        
//    }
//#endif
    
    //cout << "** Generate MixGraph ** " << endl;
    int for_i=0;
    int for_n=0;
    double weight_cost;
    for(const auto& gate : Dgraph_cnot.nodeset)
    {
        //cout << "for_i: " << for_i << endl;
        if(for_i==m)
        {
            for_i=0;
            for_n++;
            //cout << "for_n: " << for_n << endl;
            //cout << "weight_cost: " << weight_cost << endl;
        }
        weight_cost = costWeightTable[for_n];
        const int id = gate.id;
        const int target = gate.target;
        const int control = gate.control;
        if(gen_edge[target][control])
        {
            for(auto& e : g.edgeset)
            {
                if(e.second.gettargetid()==target && e.second.getsourceid()==control)
                {
                    int weight = e.second.getweight();
                    //e.second.setweight(weight+1);
                    e.second.setweight(weight+weight_cost*10);
                }
                else if(e.second.gettargetid()==control && e.second.getsourceid()==target)
                {
                    int weight = e.second.getweight();
                    //e.second.setweight(weight+1);
                    e.second.setweight(weight+weight_cost*10);
                }

                else continue;
            }
        }
        else
        //g.addedge(target, control, 1);
        g.addedge(target, control, weight_cost*10);
        gen_edge[target][control] = true;
        gen_edge[control][target] = true;

        for_i++;
    }

    for(int i = 0; i < nqubits; i++)
        delete[] gen_edge[i];
    delete[] gen_edge;

    return g;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Qcircuit::QMapper::bfs_queue_gen(queue<int>& queue, Graph& graph, int start, bool order)
{
    vector< pair< pair<int, int>, int> >  bfs_sort_vector; // <dist, weight> , nodeid(table idx)
    for(int i=0; i<nqubits; i++)
    {
        if(layout_L[i]!=-1 && i!=start)
            continue;
        bfs_sort_vector.push_back(make_pair(make_pair(graph.dist[start][i], graph.bfs_weight(start, i)), i));
    }

    if(order) //order: small -> large
        sort(bfs_sort_vector.begin(), bfs_sort_vector.end(), compare_graph_bfs);
    else //number: large -> small
        sort(bfs_sort_vector.begin(), bfs_sort_vector.end(), compare_graph_bfs_number);
   
    queue.push(start);  //for graph matching 


    for(int i=0; i<bfs_sort_vector.size(); i++)
        queue.push(bfs_sort_vector[i].second);
    
    if(queue.front() == start) queue.pop();
}

bool compare_graph_sub(pair< pair<int, int>, pair<int, int>> a, pair< pair<int, int>, pair<int, int>> b)
{
    if(a.first.first == b.first.first)
    {
        if(a.first.second == b.first.second)
        {
            return a.second.first > b.second.first; // number: large
        }
        else
        {
            return a.first.second < b.first.second; // order: small
        }
    }
    else
    {
        return a.first.first < b.first.first; //bfs dist: small
    }
}

void Qcircuit::QMapper::bfs_queue_gen_2(queue<int>& queue, Graph& graph_order, Graph& graph_number, int start)
{
    vector< pair< pair<int, int>, pair<int, int> > >  bfs_sort_vector; // <dist, order_weight>, <order_number, nodeid(table idx)>
    for(int i=0; i<nqubits; i++)
    {
        bfs_sort_vector.push_back(make_pair(make_pair(graph_order.dist[start][i], graph_order.bfs_weight(start, i)), make_pair(graph_number.bfs_weight(start, i), i)));
    }

    sort(bfs_sort_vector.begin(), bfs_sort_vector.end(), compare_graph_sub);
   
    queue.push(start);  //for graph matching 


    for(int i=0; i<bfs_sort_vector.size(); i++)
        queue.push(bfs_sort_vector[i].second.second);
    
    if(queue.front() == start) queue.pop();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Qcircuit::QMapper::make_ref_loc(vector<int>& ref_loc, Graph& graph, int start, bool order)
{
    vector<pair<int, int>> temp; //edge weight, id
    //convert table idx to graph node id
    int g_id = graph.nodeid[start];
    Node& node = graph.nodeset[g_id];
    for(auto e_id : node.edges)
    {
        Edge& edge = graph.edgeset[e_id];
        int id = (edge.source->getid() == start) ? edge.target->getid() : edge.source->getid();
        if(layout_L[id] != -1) temp.push_back(make_pair(edge.getweight(), layout_L[id]));
    }
    if(order)
        sort(temp.begin(), temp.end(), compare_cost);
    else
        sort(temp.begin(), temp.end(), compare_cost_d);

    for(auto v : temp)
        ref_loc.push_back(v.second);
#if DEBUG    
    cout << " ref_loc: ";
    for(auto v : temp)
    { 
        cout << v.second << ", ";
    }
    cout << endl;
#endif
}


void Qcircuit::QMapper::make_candi_loc(vector<int>& candi_loc, vector<int>& ref_loc, Graph& graph)
{
    int start = ref_loc[0];
    set<int> initial_candi_loc;
    int min_dist = 1;
    do
    {
        //auto sorted by ascending order
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == start) continue;
            if(graph.dist[start][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                initial_candi_loc.insert(i);
        }
        min_dist++;
    } while(initial_candi_loc.empty());
#if DEBUG    
    cout << "initial candi loc: ";
    for(auto v : initial_candi_loc)
    {
        cout << v << ", ";
    }
    cout << endl;
#endif

    for(int i = 1; i < ref_loc.size(); i++)
    {
        vector<pair<int, int>> candi_loc_dist; //dist, phy Q
        for(auto val : initial_candi_loc)
        {
            int d = graph.dist[ref_loc[i]][val];
            candi_loc_dist.push_back(make_pair(d, val));
        }
        sort(candi_loc_dist.begin(), candi_loc_dist.end(), compare_cost);
        
        int min_dist = candi_loc_dist[0].first;
        for(auto pair : candi_loc_dist)
        {
            if(pair.first != min_dist)
            {
                if(initial_candi_loc.size() == 1) break;
                initial_candi_loc.erase(pair.second);
            }
        }
#if DEBUG    
        cout << " update candi loc: (min_dist: " << min_dist << "): ";
        for(auto v : initial_candi_loc)
        {
            cout << v << ", ";
        }
        cout << endl;
#endif
    }

    for(auto val : initial_candi_loc)
        candi_loc.push_back(val);
}

void Qcircuit::QMapper::make_candi_loc_2(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2, int& degree)
{
    int start = current_q;
    set<int> initial_candi_loc;
    int min_dist = 1;

    do
    {
        //auto sorted by ascending order
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == start) continue;
            if(graph.dist[start][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                initial_candi_loc.insert(i);
        }
        min_dist++;
    } while(initial_candi_loc.empty());
#if DEBUG_loc    
    cout << " initial candi loc: ";
    for(auto v : initial_candi_loc)
    {
        cout << v << ", ";
    }
    cout << endl;
#endif
    
    set<int>::iterator it;
    it = initial_candi_loc.begin();
    //cout << *it << endl;
    candi_loc_1.push_back(*it);
    initial_candi_loc.erase(it);
    
    // ** candi_loc ** // 
    for(auto val : initial_candi_loc)
    {
        //cout << val << endl;
        if(graph.dist[*it][val]==1)
            candi_loc_1.push_back(val);
        else
            candi_loc_2.push_back(val);
    }

    if(candi_loc_1.size() < candi_loc_2.size())
        swap(candi_loc_1, candi_loc_2);

    // ** degree ** //
    degree = candi_loc_1.size() + candi_loc_2.size();
    
    // ** location ** //

}

void Qcircuit::QMapper::make_candi_loc_3(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2, int& degree)
{
    int start = current_q;
    set<int> initial_candi_loc;
    int min_dist = 1;

    do
    {
        //auto sorted by ascending order
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == start) continue;
            if(graph.dist[start][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                initial_candi_loc.insert(i);
        }
        min_dist++;
    } while(initial_candi_loc.empty());
#if DEBUG_loc    
    cout << " initial candi loc: ";
    for(auto v : initial_candi_loc)
    {
        cout << v << ", ";
    }
    cout << endl;
#endif
    
    set<int>::iterator it;
    it = initial_candi_loc.begin();
    //cout << *it << endl;
    candi_loc_1.push_back(*it);
    initial_candi_loc.erase(it);
    
    // ** candi_loc ** // 
    for(auto val : initial_candi_loc)
    {
        //cout << val << endl;
        if(graph.dist[*it][val]==1)
            candi_loc_1.push_back(val);
        else
            candi_loc_2.push_back(val);
    }

    int degree_1 = sort_degree_return(candi_loc_1, coupling_graph);
    int degree_2 = sort_degree_return(candi_loc_2, coupling_graph);
    
    if(degree_1 < degree_2)
        swap(candi_loc_1, candi_loc_2);

    // ** degree ** //
    degree = candi_loc_1.size() + candi_loc_2.size();
    
    // ** location ** //

}



/////////////////////////////

void Qcircuit::QMapper::make_candi_loc_degree(int current_q, Graph& graph, vector<int>& candi_loc_1, vector<int>& candi_loc_2)
{
    int start = current_q;
    set<int> initial_candi_loc;
    int min_dist = 1;

    int fullLayout=0;
    for(int i=0; i<graph.node_size; i++)
        if(layout_L[i]!=-1) fullLayout++;
    if(fullLayout == graph.node_size) return;

    //make initial candi_loc
    do
    {
        //auto sorted by ascending order
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == start) continue;
            if(graph.dist[start][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                initial_candi_loc.insert(i);
        }
        min_dist++;
    } while(initial_candi_loc.empty());
    //cout << " initial candi loc: ";
    //for(auto v : initial_candi_loc)
    //    cout << v << ", ";
    //cout << endl;

    set<int>::iterator it;
    it = initial_candi_loc.begin();
    //cout << *it << endl;
    candi_loc_1.push_back(*it);
    initial_candi_loc.erase(it);
    
    // ** candi_loc ** // 
    for(auto val : initial_candi_loc)
    {
        //cout << val << endl;
        if(graph.dist[*it][val]==1)
            candi_loc_1.push_back(val);
        else
            candi_loc_2.push_back(val);
    }

    //sort degree
    int degree_1 = sort_double_degree(candi_loc_1, coupling_graph);
    int degree_2 = sort_double_degree(candi_loc_2, coupling_graph);
    
    if(degree_1 < degree_2)
        swap(candi_loc_1, candi_loc_2);
}

/////////////////////////////






void Qcircuit::QMapper::make_candi_loc_dist(int current_qc, vector<int>& candi_loc, vector<int>& ref_loc, Graph& coupling_graph, Graph& interaction_graph, bool equal_order)
{
#if DEBUG_loc
    cout << endl;
    cout << "* make_candi_loc_distance *" << endl;
#endif
    vector<pair<int, int>> distance_candi_loc;
    vector<int> initial_candi_loc;

    //initial candi_loc
    for(int i = 0; i < coupling_graph.node_size; i++)
    {
        if(qubit_Q.count(i) && qubit_Q[i] == -1)
            initial_candi_loc.push_back(i);
    }
#if DEBUG_loc
    cout << "initial candi loc: " << initial_candi_loc << endl;
#endif

    //sub_num --> candi_loc
    if(equal_order)
    {
        for(auto r : ref_loc)
        {
            //cout << "* r: " << r << endl;
            for(auto v : initial_candi_loc)
            {
                int dist = coupling_graph.dist[v][r];
                distance_candi_loc.push_back(make_pair(dist, v));
                //cout << "- v: " << v << ", dist: " << dist << endl;
            }
            sort(distance_candi_loc.begin(), distance_candi_loc.end(), compare_cost);
            initial_candi_loc.clear();
            for(auto v : distance_candi_loc)
            {
                if(v.first == distance_candi_loc[0].first)
                    initial_candi_loc.push_back(v.second);
            }
            distance_candi_loc.clear();
#if DEBUG_loc
            cout << "initial candi loc: " << initial_candi_loc << endl;
#endif
        }
    }
    //dist_sum --> candi_loc
    else
    {
        for(auto v : initial_candi_loc)
        {
            //cout << "* v: " << v << endl;
            int dist_sum = 0;
            int dist;
            for(auto r : ref_loc)
            {
                dist = coupling_graph.dist[v][r];
                dist_sum += dist;
                //cout << "- r: " << r << ", dist: " << dist << endl; 
            }
            //cout << "--> dist_sum: " << dist_sum << endl;
            distance_candi_loc.push_back(make_pair(dist_sum, v));
        }
        sort(distance_candi_loc.begin(), distance_candi_loc.end(), compare_cost);
        initial_candi_loc.clear();
        for(auto v : distance_candi_loc)
        {
            if(v.first == distance_candi_loc[0].first)
                initial_candi_loc.push_back(v.second);
        }
        distance_candi_loc.clear();
#if DEBUG_loc
        cout << "initial candi loc: " << initial_candi_loc << endl;
#endif
    
    }


    candi_loc = initial_candi_loc;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Qcircuit::QMapper::make_candi_loc_3(int current_q, Graph& graph, vector<pair<int,int>>& candi_loc, int& degree)
{
    int start = current_q;
    set<int> initial_candi_loc;
    int min_dist = 1;

    do
    {
        //auto sorted by ascending order
        for(int i = 0; i < graph.node_size; i++)
        {
            if(i == start) continue;
            if(graph.dist[start][i] > min_dist) continue;
            if(qubit_Q.count(i) && qubit_Q[i] == -1)
                initial_candi_loc.insert(i);
        }
        min_dist++;
    } while(initial_candi_loc.empty());
#if DEBUG    
    cout << " initial candi loc: ";
    for(auto v : initial_candi_loc)
    {
        cout << v << ", ";
    }
    cout << endl;
#endif
   
    // ** degree ** //
    degree = initial_candi_loc.size();
    
    for(auto val : initial_candi_loc)
    {
        int relative_loc, relative_num;
        relative_loc = val - current_q;
        if(relative_loc == -1)
            relative_num = 1;
        else if(relative_loc == -5)
            relative_num = 2;
        else if(relative_loc == 1)
            relative_num = 3;
        else if(relative_loc == 5)
            relative_num = 4;
        else if(relative_loc == -6)
            relative_num = 5;
        else if(relative_loc == -4)
            relative_num = 6;
        else if(relative_loc == 6)
            relative_num = 7;
        else if(relative_loc == 4)
            relative_num = 8;
        else
            relative_num = 0;
        
        //cout << "val: " << val << endl;
        //cout << "relative_loc: " << relative_loc << endl;
        //cout << "relative_num: " << relative_num << endl;

        candi_loc.push_back(make_pair(relative_num, val));
    }
    
    
}
