//mapping.cpp
#include "circuit.h"

#define DEBUG_mapping 1
#define PRINT_Dlist 1
#define PRINT_init_mapping 1
#define PRINT_Gatelist 1
#define DEBUG_cost 0

#define MAPPING 1
#define MAPPING_TCAD 0
#define MAPPING_OUR 1   //ICCAD Paper


#define Dlist_all_mode 1 //for single-qubit gate


//#define BRIDGE_MODE 1 //ICCAD Paper!! //////////////////////////////////////////
//==> main.cpp
#define cost_prameter_1 1 //ICCAD Paper!! //////////////////////////////////
#define cost_parameter_2 0
#define cost_parameter_normal 0

//#define Test_SWAPcost 0
using namespace std;
using namespace Qcircuit;

void Qcircuit::QMapper::Circuit_Mapping(Circuit& dgraph, ARCHITECTURE archi, bool forward, bool prepro, bool postpro, bool BRIDGE_MODE)
{
//    //make Dlist
//    make_Dlist(dgraph);
//
//#if PRINT_Dlist
//    cout << "*** Dlist (only CNOT) *** " << endl;
//    cout << Dlist << endl;
//#endif

//    make_Dlist_all(dgraph);
//    cout << "*** Dlist (all) *** " << endl;
//    cout << Dlist_all << endl;
    
    FinalCircuit.nodeset.clear();
    //cout << dgraph.nodeset.size() << endl;
    for(auto& gate : dgraph.nodeset)
    {
        FinalCircuit.nodeset.push_back(gate);
    }



//#if MAPPING
//#if DEBUG_mapping    
//    cout << endl << "*** Circuit mapping Start ***" << endl;
//#endif    
// 
//    //cost_weight_table
//    cost_weight_table.clear();
//#if cost_parameter_1
//    double cost = 1.0
//    //cost_weight_table.push_back(cost);
//    //while(cost >= 0.0000001)
//    //{
//    //    cost *= param_alpha;
//    //    cost_weight_table.push_back(cost);
//    //}
//    //cout << "cost_weight_table: " << cost_weight_table << endl;
//#elif cost_parameter_2
//    cost_weight_table.push_back(cost);
//    for(int i=1; i<param_beta; i++)
//    {
//        cost -= 1/param_beta;
//        cost_weight_table.push_back(cost);
//    }
//#elif cost_parameter_normal
//    double cost=1.0;
//    for(int i=0; i<param_beta; i++)
//    {
//        cost *= param_alpha;
//        cost_weight_table.push_back(cost);
//    }
//#else
//    cost_weight_table.clear();
//#endif
//
//
//    //initialize --------------------------
//    list<int> fron_list;
//    list<int> act_list;
//    list<int> act_dist2_list;
//    vector<bool> frozen(nqubits, 0);
//    list<int> singlequbit_list;
//
//    int loop_end = 0;
//    
//    //initialize for post processing --------------------------
//    FinalCircuit.nodeset.clear(); 
//    TempCircuit.nodeset.clear(); 
//    node_id = dgraph.nodeset.size(); //for add SWAP
//    add_cnot_num = 0;
//    add_swap_num = 0;
//    add_bridge_num = 0;
//
//    ////vector<int> CircuitDepth(nqubits, 0);
//    //CircuitDepth.clear();
//    //for(int i=0; i<nqubits; i++)
//    //    CircuitDepth.push_back(0);
//
//    add_depth = 0;
//
//    vector< pair< pair<int, int>, pair<int, double> > > MCPE_flag; // SWAP candidate, act_gate_id, cost
//    int history_size = 2;
//
//    //do while front_list[] is not empty
//    do{
//#if DEBUG_mapping
//        cout << "-------------------------------------------------------------" << endl;
//#endif
//
//        bool complete_act_list = true;
////////// TCAD Mapping version
//#if MAPPING_TCAD
//            update_front_n_act_list(fron_list, act_list, frozen);
//            //complete_act_list = check_direct_act_list(act_list, frozen, dgraph, CircuitDepth);
//            complete_act_list = check_direct_act_list(act_list, singlequbit_list, frozen, dgraph);
//            act_list.sort();
//#endif
//
//////// Our mapping version
//#if MAPPING_OUR
//       do{
//            #if DEBUG_mapping
//            cout << "<1. update front and act list>" << endl;
//            #endif
//            update_front_n_act_list(fron_list, act_list, frozen);
//            #if PRINT_Gatelist
//            cout << "fron_list[]: " << fron_list << endl;
//            cout << "act_list[]: " << act_list << endl;
//            cout << "frozen: " << frozen << endl << endl;
//            #endif
//
//            #if DEBUG_mapping
//            cout << "<2. check direct act list>" << endl;
//            #endif
//            //complete_act_list = check_direct_act_list(act_list, frozen, dgraph, CircuitDepth);
//            complete_act_list = check_direct_act_list(act_list, singlequbit_list, frozen, dgraph);
//            #if PRINT_Gatelist
//            cout << "fron_list[]: " << fron_list << endl;
//            cout << "act_list[]: " << act_list << endl;
//            cout << "frozen: " << frozen << endl << endl;
//            #endif
//    
//            #if DEBUG_mapping
//            cout << "<3. sort act list>" << endl;
//            #endif
//            act_list.sort();
//            #if PRINT_Gatelist
//            cout << "act_list[]: " << act_list << endl << endl;
//            #endif
//       }while(complete_act_list);
//#endif
///////////////////////////
//
//        //(3) SWAP
//        //if(!fron_list.empty() && act_list.empty()) continue;
//        vector< pair<pair<int, int>, int> > candi_list; // SWAP candidate, act_gate_id
//        generate_candi_list(act_list, candi_list, dgraph);
//#if PRINT_Gatelist
//        cout << "candi list : " << endl << candi_list << endl;
//#endif
//
//       
//        ////////// if act_gate's distance = 2
//        if(BRIDGE_MODE)
//        {
//            update_act_dist2_list(act_dist2_list, act_list, dgraph);
//            //#if PRINT_Gatelist
//            //cout << endl << " (4) Bridge gate? " << endl;
//            //cout << "act_dist2_list : " << act_dist2_list << endl;
//            //#endif
//        }
//
//        //MCPE_cost
//        vector< pair< pair<int, int>, pair<int, double> > > MCPE_test; // SWAP candidate, act_gate_id, cost
//        for(auto kv : candi_list)
//        {
//            pair<int, int> SWAP_pair = kv.first;
//            double cost = cal_MCPE(SWAP_pair, dgraph);
//
//            MCPE_test.push_back(make_pair(SWAP_pair, make_pair(kv.second, cost)));
//        }
//        //#if PRINT_Gatelist
//        //cout << MCPE_test << endl;
//        //#endif        
//
//        
//        if(!act_list.empty())
//        {
//            ////////////////////////////////////////////////////////////////////
//            if(BRIDGE_MODE)
//            {
//                pair<int, int> SWAP;
//                int gateid;
//                double max_cost;
//                find_max_cost_SWAP_2(SWAP, gateid, max_cost, MCPE_test, act_list, MCPE_flag);
//                //#if DEBUG_mapping
//                //cout << "SWAP: " << SWAP << endl;
//                //cout << "gateid: " << gateid << endl;
//                //cout << "max_cost: " << max_cost << endl;
//                //#endif
//                
//                std::list<int>::iterator it = std::find(act_dist2_list.begin(), act_dist2_list.end(), gateid);
//                if(it != act_dist2_list.end() && max_cost <= param_gamma*10) 
//                {
//                    #if DEBUG_mapping
//                    cout << "\t\t** Bridge for " << gateid << "!!!!" << endl;
//                    #endif
//                    
//                    add_bridge(SWAP.first, SWAP.second, FinalCircuit);
//                    
//                    // push bridge                
//                    int control = dgraph.nodeset[gateid].control;
//                    int target  = dgraph.nodeset[gateid].target;
//                    Dlist[control].pop_front();
//                    Dlist[target ].pop_front();
//                    frozen[control] = false;
//                    frozen[target ] = false;
//                    
//                    #if Dlist_all_mode
//                    Dlist_all[control].pop_front();
//                    Dlist_all[target ].pop_front();
//                    #endif
//
//                    ////////add depth
//                    //int depthQubit1 = CircuitDepth[control];
//                    //int depthQubit2 = CircuitDepth[target];
//                    //int maxDepth    = max(depthQubit1, depthQubit2);
//                    ////CircuitDepth[control] = maxDepth + 4;
//                    ////CircuitDepth[target]  = maxDepth + 4;
//                
//                    act_list.erase( remove(act_list.begin(), act_list.end(), gateid) );
//                }
//                else
//                {
//                    #if DEBUG_mapping
//                    cout << "** SWAP for " << gateid << "!!!!" << endl;
//                    #endif
//                    if(MCPE_flag.size() > history_size)
//                        MCPE_flag.erase(MCPE_flag.begin());
//                    MCPE_flag.push_back(MCPE_test[0]);
//                    
//                    //#if PRINT_Gatelist
//                    //cout << "we swap " << SWAP << endl;
//                    //#endif
//                    int f_SWAPqubit = qubit_Q[SWAP.first];
//                    int s_SWAPqubit = qubit_Q[SWAP.second];
//                    add_swap(f_SWAPqubit,    s_SWAPqubit, FinalCircuit);
//                    layout_swap(f_SWAPqubit, s_SWAPqubit);
//                
//                    //////add depth
//                    //int depthQubit1 = CircuitDepth[f_SWAPqubit];
//                    //int depthQubit2 = CircuitDepth[s_SWAPqubit];
//                    //int maxDepth    = max(depthQubit1, depthQubit2);
//                    ////CircuitDepth[f_SWAPqubit] = maxDepth + 3;
//                    ////CircuitDepth[s_SWAPqubit] = maxDepth + 3;
//                }
//            }
//            ////////////////////////////////////////////////////////////////////
//            else
//            {
//                pair<int, int> SWAP;
//                int gateid;
//                double max_cost;
//                find_max_cost_SWAP_2(SWAP, gateid, max_cost, MCPE_test, act_list, MCPE_flag);
//    
//#if DEBUG_mapping
//                cout << "SWAP: " << SWAP << endl;
//                cout << "gateid: " << gateid << endl;
//                cout << "max_cost: " << max_cost << endl;
//#endif
//                //pair<int, int> SWAP = find_max_cost_SWAP(MCPE_test, act_list, MCPE_flag);
//                //pair<int, double> SWAP = find_max_cost_SWAP(MCPE_test, act_list, MCPE_flag);
//                if(MCPE_flag.size() > history_size)
//                    MCPE_flag.erase(MCPE_flag.begin());
//                MCPE_flag.push_back(MCPE_test[0]);
//#if PRINT_Gatelist
//                cout << "\t\t** WE SWAP!! " << SWAP << endl;
//#endif
//                    int f_SWAPqubit = qubit_Q[SWAP.first];
//                    int s_SWAPqubit = qubit_Q[SWAP.second];
//                    add_swap(f_SWAPqubit,    s_SWAPqubit, FinalCircuit);
//                    layout_swap(f_SWAPqubit, s_SWAPqubit);
//                    
//                    ////add depth
//                    //int depthQubit1 = CircuitDepth[f_SWAPqubit];
//                    //int depthQubit2 = CircuitDepth[s_SWAPqubit];
//                    //int maxDepth    = max(depthQubit1, depthQubit2);
//                    ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                    ////CircuitDepth[f_SWAPqubit] = maxDepth + 3;
//                    ////CircuitDepth[s_SWAPqubit] = maxDepth + 3;
//                    ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//            }
//        }
//        loop_end = 0; 
//        for(int q=0; q<nqubits; q++)
//        {
//            if(Dlist[q].empty()) loop_end++;
//        }
//
//#if Dlist_all_mode
//        if(loop_end==nqubits)
//        {
//            for(int q=0; q<nqubits; q++)
//            {
//                for(auto& id : Dlist_all[q])
//                {
//                    singlequbit_list.push_back(id);
//                }
//            }
//            singlequbit_list.sort();
//            #if DEBUG_mapping
//            cout << "singlequbit_list: " <<  singlequbit_list << endl;
//            #endif
//            for(auto& id : singlequbit_list)
//                FinalCircuit.nodeset.push_back(dgraph.nodeset[id]);
//        }
//#endif
//
//#if DEBUG_mapping
//        cout << "loop_end!!!!!!!!!!!!!!: " << loop_end <<  endl;
//#endif
//    }while( true && (!fron_list.empty()|| loop_end!=nqubits) );
//
//
//    //final circuit depth
//    //add_depth = *max_element(CircuitDepth.begin(), CircuitDepth.end()) - originalDepth;
//
//
//#if DEBUG_mapping
//    cout << " ==> Look-ahead heuristic algorithm Successfully Finished" << endl;
//#endif    
//
//    if(BRIDGE_MODE && !postpro)
//    {
//        if(least_cnot_num < add_cnot_num)
//        {
//            layout_L = new_layout_L;
//            qubit_Q  = new_qubit_Q;
//        }
//    }
//    
//    //mapping update
//    new_layout_L = layout_L;
//    new_qubit_Q  = qubit_Q;
//
//    //for postprocessing
//    if(forward)
//    {
//        if(least_cnot_num > add_cnot_num)
//        {
//            least_cnot_num = add_cnot_num;    
//            least_swap_num = add_swap_num;
//            least_bridge_num = add_bridge_num;
//            least_depth = add_depth;
//            
//            Best_FinalCircuit.nodeset = FinalCircuit.nodeset;
//        }
//    }
//#endif
}

// flist, alist, f
void Qcircuit::QMapper::update_front_n_act_list(list<int>& front_list, list<int>& act_list, vector<bool>& frozen)
{
    for(int q = 0; q < nqubits; q++)
    {
        list<int>& Dlist_line = Dlist[q];
        if(!frozen[q])
        {
            if(Dlist_line.empty()) continue;
            int gateid = Dlist_line.front();
            
            if( find(front_list.begin(), front_list.end(), gateid) != front_list.end() )
            {
                front_list.erase( remove(front_list.begin(), front_list.end(), gateid));
                act_list.push_back(gateid);
            }
            else
            {
                front_list.push_back(gateid);
            }
            frozen[q] = true;
        }
    }
}


void Qcircuit::QMapper::find_singlequbit_list(int gateid, list<int>& singlequbit_list, Circuit& dgraph)
{
    singlequbit_list.clear();
    int control = dgraph.nodeset[gateid].control;
    int target  = dgraph.nodeset[gateid].target;
    //cout << "gateid: " << gateid << endl << endl;
    int erase_control = 0;
    int erase_target = 0;
    
    //for control //////////////////////////////
    //cout << Dlist_all[control] << endl;
    //cout << "control: " << control << "==>";
    for(auto& id : Dlist_all[control])
    {
        if(id == gateid) {
            erase_control++;
            break;
        }
        else {
            singlequbit_list.push_back(id);
            //cout << id << "-> ";
            erase_control++;
        }
    }
    //cout << endl;
    //cout << erase_control << endl;
    for(int i=0; i<erase_control; i++) Dlist_all[control].pop_front();
    //cout << Dlist_all[control] << endl;
    //cout << endl;


    //for target ////////////////////////////////
    //cout << Dlist_all[target] << endl;
    //cout << "target: " << target << "==>";
    for(auto& id : Dlist_all[target])
    {
        if(id == gateid) {
            erase_target++;
            break;
        }
        else {
            singlequbit_list.push_back(id);
            //cout << id << "-> ";
            erase_target++;
        }
    }
    //cout << endl;
    //cout << erase_target << endl;
    for(int i=0; i<erase_target; i++) Dlist_all[target].pop_front();
    //cout << Dlist_all[target] << endl;
    //cout << endl;
    
    singlequbit_list.sort();
    #if DEBUG_mapping
    cout << "singlequbit_list: " <<  singlequbit_list << endl;
    #endif

}

void Qcircuit::QMapper::update_act_dist2_list(list<int>& act_dist2_list, list<int>& act_list, Circuit& dgraph)
{
    act_dist2_list.clear();
    for(auto& gateid : act_list)
    {
        int control = dgraph.nodeset[gateid].control;
        int target  = dgraph.nodeset[gateid].target;
        int Q_control = layout_L[control];
        int Q_target  = layout_L[target];
        if(coupling_graph.dist[Q_control][Q_target] == 2)
            act_dist2_list.push_back(gateid);
        
    }
}

//bool Qcircuit::QMapper::check_direct_act_list(list<int>& act_list, vector<bool>& frozen, Circuit& dgraph, vector<int>& CircuitDepth)
bool Qcircuit::QMapper::check_direct_act_list(list<int>& act_list, list<int>& singlequbit_list, vector<bool>& frozen, Circuit& dgraph)
{
    bool complete_act_list = false;
    vector<int> act_list_erase;
    for(auto& gateid : act_list)
    {
        int control = dgraph.nodeset[gateid].control;
        int target  = dgraph.nodeset[gateid].target;
        int Q_control = layout_L[control];
        int Q_target  = layout_L[target];
        //CNOT
        if(coupling_graph.dist[Q_control][Q_target] == 1)
        {
            act_list_erase.push_back(gateid);
            Dlist[control].pop_front();
            Dlist[target ].pop_front();
            frozen[control] = false;
            frozen[target ] = false;

            #if Dlist_all_mode
            find_singlequbit_list(gateid, singlequbit_list, dgraph);
            for(auto& id : singlequbit_list)
                FinalCircuit.nodeset.push_back(dgraph.nodeset[id]);
            #endif
            
            FinalCircuit.nodeset.push_back(dgraph.nodeset[gateid]);
            complete_act_list = true;
            #if DEBUG_mapping
            cout << "\t\t @@@ FinalCircuit: " << gateid << endl;
            #endif
            
            
            ////add depth
            //int depthQubit1 = CircuitDepth[control];
            //int depthQubit2 = CircuitDepth[target];
            //int maxDepth    = max(depthQubit1, depthQubit2);
            //CircuitDepth[control] = maxDepth + 1;
            //CircuitDepth[target]  = maxDepth + 1;
        }
//#if Dlist_all_mode //for single-qubit gate
//        //Single-qubit
//        if(dgraph.nodeset[gateid].type != GATETYPE::CNOT)
//        {
//            act_list_erase.push_back(gateid);
//            Dlist[target ].pop_front();
//            frozen[target ] = false;
//
//            FinalCircuit.nodeset.push_back(dgraph.nodeset[gateid]);
//            complete_act_list = true;
//        }
//#endif        
    }
    for(auto gateid : act_list_erase)
        act_list.erase( remove(act_list.begin(), act_list.end(), gateid) );

    return complete_act_list;
}

void Qcircuit::QMapper::generate_candi_list(list<int>& act_list, vector< pair<pair<int, int>, int> >& candi_list, Circuit& dgraph)
{
    //candi_list is SWAP candidates (physical qubit)
    for(auto& gateid : act_list)
    {
        int control = dgraph.nodeset[gateid].control;
        int target  = dgraph.nodeset[gateid].target;
        int Q_control = layout_L[control];
        int Q_target  = layout_L[target];
        for(int i = 0; i < coupling_graph.node_size; i++)
        {
            if(coupling_graph.dist[Q_control][i] == 1)
            {
                if(cal_SWAP_effect(control, target, i, Q_control) <= 0) continue;
                //if(cal_SWAP_effect(control, target, i, Q_control) < 0) continue;
#if DEBUG_cost
                cout << "SWAP(" << setw(2) << Q_control << "," << setw(2) << i << ")'s effect: " << cal_SWAP_effect(control, target, i, Q_control) << endl;
#endif
                (i < Q_control) ? candi_list.push_back(make_pair(make_pair(i, Q_control), gateid)) : 
                                  candi_list.push_back(make_pair(make_pair(Q_control, i), gateid));
            }
            if(coupling_graph.dist[Q_target][i] == 1)
            {
                if(cal_SWAP_effect(control, target, i, Q_target ) <= 0) continue;
                //if(cal_SWAP_effect(control, target, i, Q_target ) < 0) continue;
#if DEBUG_cost
                cout << "SWAP(" << setw(2) << Q_target << "," << setw(2) << i << ")'s effect: " << cal_SWAP_effect(control, target, i, Q_target) << endl;
#endif
                (i < Q_target)  ? candi_list.push_back(make_pair(make_pair(i, Q_target),  gateid)) : 
                                  candi_list.push_back(make_pair(make_pair(Q_target, i),  gateid));
            }
        }
    }
}

void Qcircuit::QMapper::generate_candi_list_n(list<int>& act_list, vector< pair<pair<int, int>, int> >& candi_list, Circuit& dgraph)
{
    //candi_list is SWAP candidates (physical qubit)
    for(auto& gateid : act_list)
    {
        int control = dgraph.nodeset[gateid].control;
        int target  = dgraph.nodeset[gateid].target;
        int Q_control = layout_L[control];
        int Q_target  = layout_L[target];
        for(int i = 0; i < coupling_graph.node_size; i++)
        {
            if(coupling_graph.dist[Q_control][i] == 1)
            {
                //if(cal_SWAP_effect(control, target, i, Q_control) <= 0) continue;
#if DEBUG_cost
                cout << "SWAP(" << setw(2) << Q_control << "," << setw(2) << i << ")'s effect: " << cal_SWAP_effect(control, target, i, Q_control) << endl;
#endif
                (i < Q_control) ? candi_list.push_back(make_pair(make_pair(i, Q_control), gateid)) : 
                                  candi_list.push_back(make_pair(make_pair(Q_control, i), gateid));
            }
            if(coupling_graph.dist[Q_target][i] == 1)
            {
                //if(cal_SWAP_effect(control, target, i, Q_target ) <= 0) continue;
#if DEBUG_cost
                cout << "SWAP(" << setw(2) << Q_target << "," << setw(2) << i << ")'s effect: " << cal_SWAP_effect(control, target, i, Q_target) << endl;
#endif
                (i < Q_target)  ? candi_list.push_back(make_pair(make_pair(i, Q_target),  gateid)) : 
                                  candi_list.push_back(make_pair(make_pair(Q_target, i),  gateid));
            }
        }
    }
}


