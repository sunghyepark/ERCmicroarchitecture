//mappingFunction.cpp
#include "circuit.h"

#define DEBUG 0
#define DEBUG_d 0
//#define DEBUG_cost 1

#define cost_parameter_1 1  //optimal!! ///////////////////////////////////////////
#define cost_parameter_2 0
#define cost_parameter_3 0
#define cost_parameter_normal 0

using namespace std;
using namespace Qcircuit;

//// cost function ////
int Qcircuit::QMapper::cal_SWAP_effect(const int q1, const int q2, const int Q1, const int Q2)
{
    //q1 and q2 is logical qubit of gate
    //Q1 and Q2 is swapped physical qubit of coupling graph
    
    int controlQ = layout_L[q1];
    int targetQ = layout_L[q2];

    int dist_before_swap = coupling_graph.dist[controlQ][targetQ];


    int swapq1 = qubit_Q[Q1];
    int swapq2 = qubit_Q[Q2];

    if(controlQ == Q1 || controlQ == Q2)
        controlQ = (controlQ == Q1) ? Q2 : Q1;
    if(targetQ == Q1 || targetQ == Q2)
        targetQ = (targetQ == Q1) ? Q2 : Q1;

    int dist_after_swap  = coupling_graph.dist[controlQ][targetQ];

    int effect = dist_before_swap - dist_after_swap;

    //swap(layout_swapped[qubit_Q[Q1]], layout_swapped[qubit_Q[Q2]]);
    //int effect = coupling_graph.dist[layout_L[q1]][layout_L[q2]] - 
    //             coupling_graph.dist[layout_swapped[q1]][layout_swapped[q2]];
    //swap(layout_swapped[qubit_Q[Q1]], layout_swapped[qubit_Q[Q2]]);
    return effect;
}

double Qcircuit::QMapper::cal_MCPE(const pair<int, int> p, Circuit& dgraph)
{
    int q1 = qubit_Q[p.first];
    int q2 = qubit_Q[p.second];

    double MCPE = 0;
    int dist = 0;
    double power = 1.0;

#if DEBUG
    cout << "** cal_MCPE (" << q1 << "," << q2 << ") ** " << endl; 
#endif
    if(q1 < nqubits)
        for(auto& gateid : Dlist[q1])
        {
            Gate &g = dgraph.nodeset[gateid];
            int control = g.control;
            int target  = g.target;
            int effect  = cal_SWAP_effect(control, target, p.first, p.second);
//#if DEBUG_cost
//            cout << "effect: " << effect << " (" << control << "," << target << ")" << endl;
//#endif
            if(effect < 0) break;
#if cost_parameter_1
            if(power < 0.0000001) break;
            MCPE += effect * power;
            power *= param_alpha;
#elif cost_parameter_2
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#elif cost_parameter_3
            if(dist >= param_beta) break;
            MCPE += effect;
#elif cost_parameter_normal
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#else
            MCPE += effect;
#endif
            dist++;
        }
    /////////////////////////////////////////////////////////////////////////
    dist = 0;
    power = 1.0;
    if(q2 < nqubits)
        for(auto& gateid : Dlist[q2])
        {
            Gate &g = dgraph.nodeset[gateid];
            int control = g.control;
            int target  = g.target;
            int effect  = cal_SWAP_effect(control, target, p.first, p.second);
//#if DEBUG_cost
//            cout << "effect: " << effect << " (" << control << "," << target << ")" << endl;
//#endif
            if(effect < 0) break;
#if cost_parameter_1
            if(power < 0.0000001) break;
            MCPE += effect * power;
            power *= param_alpha;
#elif cost_parameter_2
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#elif cost_parameter_3
            if(dist >= param_beta) break;
            MCPE += effect;
#elif cost_parameter_normal
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#else
            MCPE += effect;
#endif
            dist++;
        }

#if DEBUG
    cout << "-> MCPE: " << MCPE << endl << endl;
#endif
    //return MCPE;
    return MCPE*10;
}

//////////////////////////////////
double Qcircuit::QMapper::cal_MCPE_n(const pair<int, int> p, Circuit& dgraph, int actNum)
{
    int q1 = qubit_Q[p.first];
    int q2 = qubit_Q[p.second];

    double MCPE = 0;
    int dist = 0;
    double power = 1.0;

    int ForBreak1 = 0;
#if DEBUG
    cout << "** cal_MCPE (" << q1 << "," << q2 << ") ** " << endl; 
#endif
    if(q1 < nqubits)
    {
        int forN = 0;
        for(auto& gateid : Dlist[q1])
        {
            Gate &g = dgraph.nodeset[gateid];
            int control = g.control;
            int target  = g.target;
            int effect  = cal_SWAP_effect(control, target, p.first, p.second);
//#if DEBUG_cost
//            cout << "effect: " << effect << " (" << control << "," << target << ")" << endl;
//#endif
            if(actNum==1 && forN==0)
            {
                if(effect<0)
                {
                    MCPE=-10;
                    break;
                }
            }
            //if(effect < 0) break;
            if(effect < 0) ForBreak1++;
            if(ForBreak1 >= 5) break;
#if cost_parameter_1
            if(power < 0.0000001) break;
            MCPE += effect * power;
            power *= param_alpha;
#elif cost_parameter_2
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#elif cost_parameter_3
            if(dist >= param_beta) break;
            MCPE += effect;
#elif cost_parameter_normal
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#else
            MCPE += effect;
#endif
            dist++;
            forN++;
        }
    }
    /////////////////////////////////////////////////////////////////////////
    dist = 0;
    power = 1.0;
    ForBreak1 = 0;
    if(q2 < nqubits)
    {
        int forN = 0;
        for(auto& gateid : Dlist[q2])
        {
            Gate &g = dgraph.nodeset[gateid];
            int control = g.control;
            int target  = g.target;
            int effect  = cal_SWAP_effect(control, target, p.first, p.second);
//#if DEBUG_cost
//            cout << "effect: " << effect << " (" << control << "," << target << ")" << endl;
//#endif
            if(actNum==1 && forN==0)
            {
                if(effect<0)
                {
                    MCPE=-10;
                    break;
                }
            }
            //if(effect < 0) break;
            if(effect < 0) ForBreak1++;
            if(ForBreak1 >= 5) break;
#if cost_parameter_1
            if(power < 0.0000001) break;
            MCPE += effect * power;
            power *= param_alpha;
#elif cost_parameter_2
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#elif cost_parameter_3
            if(dist >= param_beta) break;
            MCPE += effect;
#elif cost_parameter_normal
            if(dist >= param_beta) break;
            MCPE += effect * cost_weight_table[dist];
#else
            MCPE += effect;
#endif
            dist++;
            forN++;
        }
    }
#if DEBUG
    cout << "-> MCPE: " << MCPE << endl << endl;
#endif
    //return MCPE;
    return MCPE*10;
}


//////////////////////////////////



int Qcircuit::QMapper::count_max(const vector< pair< pair<int, int>, pair<int, int> > >& v)
{
    if(v.empty()) return 0;
    int count = 0;
    int max_val = INT_MIN;
    for(auto& kv : v)
    {
        pair<int, int> SWAP = kv.first;
        int gateID = kv.second.first;
        int cost = kv.second.second;
        if(max_val < cost)
            max_val = cost;
    }
    for(auto& kv : v)
        if(max_val == kv.second.second) count++;
    cout << "max_value is " << max_val << endl;
    cout << "count: " << count << endl;
    return count;
}

bool compare_by_cost(pair< pair<int, int>, pair<int, int> > p1, pair< pair<int, int>, pair<int, int> > p2)
{
    //MCPE cost descending order
    return p1.second.second > p2.second.second;
}

bool compare_by_act_list(pair< pair<int, int>, pair<int, int> > p1, pair< pair<int, int>, pair<int, int> > p2)
{
    //act_list ascending order
    //MCPE cost descending order
    //SWAP pair ascending order
    
    if(p1.second.first > p2.second.first)               return false;
    else if(p1.second.first < p2.second.first)          return true;
    else
    {
        if(p1.second.second > p2.second.second)         return true;
        else if(p1.second.second < p2.second.second)    return false;
        else return p1.first.first < p2.first.first;
    }
}

bool compare_by_test(pair< pair<int, int>, pair<int, int> > p1, pair< pair<int, int>, pair<int, int> > p2)
{
    //MCPE cost descending order
    //act_list ascending order
    //SWAP pair ascending order
    
    if(p1.second.second > p2.second.second)               return true;
    else if(p1.second.second < p2.second.second)          return false;
    else
    {
        if(p1.second.first > p2.second.first)         return false;
        else if(p1.second.first < p2.second.first)    return true;
        else return p1.first.first < p2.first.first;
    }
}

pair<int, int> Qcircuit::QMapper::find_max_cost_SWAP(vector< pair< pair<int, int>, pair<int, int> > >& v, list<int>& act_list,
                                                     vector< pair< pair<int, int>, pair<int, int> > >& history)
{
    //int count = count_max(v);
    //if(count > 1)
    //    sort(v.begin(), v.end(), compare_by_act_list);
    //else
    //    sort(v.begin(), v.end(), compare_by_act_list);

    sort(v.begin(), v.end(), compare_by_cost);
    
    int index = 0;
    pair< pair<int, int>, pair<int, int> >& ref = v[index];

    for(auto pair : history)
    {
        if(pair == ref)
            index++;
    }

    if(index >= v.size()) index = v.size()-1;
#if DEBUG
    cout << "sorted" << endl;
    cout << v << endl;
#endif

    return v[index].first;
}


//void Qcircuit::QMapper::find_max_cost_SWAP_2(pair<int, int>& SWAP, int& gateid, int& max_cost,
//                                             vector< pair< pair<int, int>, pair<int, int> > >& v, list<int>& act_list,
//                                             vector< pair< pair<int, int>, pair<int, int> > >& history)
void Qcircuit::QMapper::find_max_cost_SWAP_2(pair<int, int>& SWAP, int& gateid, double& max_cost,
                                             vector< pair< pair<int, int>, pair<int, double> > >& v, list<int>& act_list,
                                             vector< pair< pair<int, int>, pair<int, double> > >& history)
{
    sort(v.begin(), v.end(), compare_by_cost);
    
    int index = 0;
    //pair< pair<int, int>, pair<int, int> >& ref = v[index];
    pair< pair<int, int>, pair<int, double> >& ref = v[index];

    for(auto pair : history)
    {
        if(pair == ref)
            index++;
    }

    if(index >= v.size()) index = v.size()-1;
#if DEBUG
    cout << "sorted" << endl;
    cout << v << endl;
#endif

    //////////////////
    SWAP = v[index].first;
    gateid = v[index].second.first;
    max_cost = v[index].second.second;
}



//// swap & bridge ////
void Qcircuit::QMapper::layout_swap(const int b1, const int b2)
{
    int& q1 = layout_L[b1];
    int& q2 = layout_L[b2];

    swap(q1, q2);
    swap(qubit_Q[q1], qubit_Q[q2]);
#if DEBUG_d
    //print physical mapping
    print_layout(layout_L);
    print_qubit(qubit_Q);
#endif
}

void Qcircuit::QMapper::layout_swap(const int b1, const int b2, map<int, int>& layout)
{
    swap(layout[b1], layout[b2]);
#if DEBUG_d
    //print physical mapping
    print_layout(layout_L);
    print_qubit(qubit_Q);
#endif
}

void Qcircuit::QMapper::add_cnot(int c_qubit, int t_qubit, Circuit& graph)
{
    Qcircuit::Gate cnot;
    
    cnot.id = node_id;
    cnot.control = c_qubit;
    cnot.target = t_qubit;
    cnot.type = GATETYPE::CNOT;
    
    graph.nodeset.push_back(cnot);

    add_cnot_num++;
    node_id++;
}

void Qcircuit::QMapper::add_swap(int b1, int b2, Circuit& graph)
{
    add_cnot(b1, b2, graph);
    add_cnot(b2, b1, graph);
    add_cnot(b1, b2, graph);

    add_swap_num++;
}


void Qcircuit::QMapper::add_bridge(int qs, int qt, Circuit& graph)
{
    int qb;
    int bs, bt, b;
    /*qs: control, qt: target*/

    for(int i=0; i<positions; i++)
    {
        if(coupling_graph.dist[qs][i] == 1)
        {
            if(coupling_graph.dist[qt][i] == 1)
            {
                qb = i;
            }
            else continue;
        }
        else continue;
    }

#if DEBUG_d
        cout << "qs: " << qs;
        cout << ", qt: " << qt;
        cout << ", qb: " << qb << endl;
#endif

    bs = qubit_Q[qs];
    bt = qubit_Q[qt];
    b = qubit_Q[qb];
    
    add_cnot(b, bt, graph);
    add_cnot(bs, b, graph);
    add_cnot(b, bt, graph);
    add_cnot(bs, b, graph);
    
    add_cnot_num--;
    add_bridge_num++;
}



