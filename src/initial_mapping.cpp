//initial_mapping.cpp
#include "circuit.h"
#include <random>

//degree
#define DEGREE_CHANGE_v1 0
#define DEGREE_CHANGE_v2 1 //ICCAD Paper
#define DEGREE_CHANGE_v3 0

//debugging
#define DEBUG_GRAPH 1
#define DEBUG_G 1
#define DEBUG_qc 1
#define PRINT_dist_table 1
#define PRINT_new_mapping 1
#define PRINT_else 1

#define DEBUG_NEW_INITIAL 1

using namespace std;
using namespace Qcircuit;


void Qcircuit::QMapper::initial_mapping(ARCHITECTURE archi, bool prepro, bool postpro, bool BRIDGE_MODE, INITIALTYPE initial_type)
{
    ////initial mapping
    //#if PRINT_new_mapping
    //cout << endl << "*** Initial Mapping Generate  ***" << endl;
    //cout << "# Updated initial mapping" << endl;
    //print_layout(layout_L);
    //#endif

    //if(!prepro)
    //{
    //    for(int i=0; i<positions; i++)
    //    {
    //        layout_L[i] = i; //logical -> physical
    //        qubit_Q[i] = i;  //physical -> logical
    //    }
    //    return;
    //}

//    //1st mapping preparation
//    if(!postpro && !BRIDGE_MODE)
//    {
//#if PRINT_else
//        cout << endl << "*** Coupling Graph Generate ***" << endl;
//#endif
//        select_coupling_graph(archi);
//        //build distance table of coupling graph
//        coupling_graph.build_dist_table();
//#if PRINT_dist_table
//        coupling_graph.print_dist_table();
//#endif
//    }
//
//    //post-processing
//    if(postpro) 
//    {
//        layout_L = new_layout_L;
//        qubit_Q = new_qubit_Q;
//
//#if PRINT_new_mapping
//        cout << "new mapping" << endl;
//        //print_qubit(qubit_Q); 
//        print_layout(layout_L);
//#endif
//
//        return;
//    }
//    
//
//    //Initial mapping mode?
//    int q_num;
//    switch(initial_type)
//    {
//        case INITIALTYPE::IDENTITY_MAPPING:
//            identical_mapping();
//            break;
//        case INITIALTYPE::RANDOM_MAPPING:
//            random_mapping();
//            break;
//        case INITIALTYPE::GRAPH_MATCHING_MIX_UPDATE:
//            graph_matching_processing_v4(archi, INITIAL_GRAPH::MIX);
//            q_num = nqubits;
//            for(int i=0; i<positions; i++)
//            {
//                if(qubit_Q[i] == -1)
//                {
//                    qubit_Q[i] = q_num;
//                    layout_L[q_num] = i;
//                    q_num++;
//                }
//            }
//            break;
//        case INITIALTYPE::GRAPH_MATCHING_MIX:
//            graph_matching_processing_v3(archi, INITIAL_GRAPH::MIX);
//            q_num = nqubits;
//            for(int i=0; i<positions; i++)
//            {
//                if(qubit_Q[i] == -1)
//                {
//                    qubit_Q[i] = q_num;
//                    layout_L[q_num] = i;
//                    q_num++;
//                }
//            }
//            break;
//        case INITIALTYPE::GRAPH_MATCHING_NUMBER:
//            graph_matching_processing_v3(archi, INITIAL_GRAPH::NUMBER);
//            q_num = nqubits;
//            for(int i=0; i<positions; i++)
//            {
//                if(qubit_Q[i] == -1)
//                {
//                    qubit_Q[i] = q_num;
//                    layout_L[q_num] = i;
//                    q_num++;
//                }
//            }
//            break;
//        case INITIALTYPE::GRAPH_MATCHING_ORDER:
//            graph_matching_processing_v3(archi, INITIAL_GRAPH::ORDER);
//            q_num = nqubits;
//            for(int i=0; i<positions; i++)
//            {
//                if(qubit_Q[i] == -1)
//                {
//                    qubit_Q[i] = q_num;
//                    layout_L[q_num] = i;
//                    q_num++;
//                }
//            }
//            break;
//        case INITIALTYPE::NEW_GRAPH_MATCHING:
//            new_graph_matching_processing(archi, INITIAL_GRAPH::MIX);
//            q_num = nqubits;
//            for(int i=0; i<positions; i++)
//            {
//                if(qubit_Q[i] == -1)
//                {
//                    qubit_Q[i] = q_num;
//                    layout_L[q_num] = i;
//                    q_num++;
//                }
//            }
//            break;
//        case INITIALTYPE::TCAD_PROCESSING:
//            pre_processing(archi);
//            break;
//            
//    }
//#if PRINT_new_mapping
//    cout << endl << "# new mapping" << endl;
//    print_qubit(qubit_Q); 
//    print_layout(layout_L);
//    cout << "=======================" << endl;
//#endif
}

void Qcircuit::QMapper::identical_mapping()
{
    for(int i=0; i<positions; i++)
    {
        qubit_Q[i] = i;
        layout_L[i] = i;
    }
}

void Qcircuit::QMapper::random_mapping()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0,19);
    
    int rand_num;
    int i=0;
    
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical
    
    while(i<20)
    {
        rand_num = dis(gen);
        if(qubit_Q[rand_num]!=-1) continue;
        else
        {
            qubit_Q[rand_num] = i;
            layout_L[i] = rand_num;
            cout << "qubit: " << i << ", layout: " << rand_num << endl;
            i++;
        }
    }
}


//// 2021.02.25~
void Qcircuit::QMapper::graph_matching_processing_v4(ARCHITECTURE archi, INITIAL_GRAPH graphtype)
{
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical

    //CNOT set
    make_CNOT(true);
    
    //Make interaction graph
    int CNOTNodesetSize = Dgraph_cnot.nodeset.size();
    int NodesetSize     = Dgraph.nodeset.size();
    //int n = param_beta;
    int n = CNOTNodesetSize/param_beta;
    if (nqubits<=10 || nqubits==20) 
        n = 1;
    if ( (nqubits==15 && NodesetSize<=5000) || nqubits==11)
        n = param_beta;
    //cout<< "nqubits: " << nqubits << ", NodesetSize: " << NodesetSize << ", n: " << n << endl;

    Graph interactionGraph;
    switch(graphtype)
    {
        case INITIAL_GRAPH::MIX:
            interactionGraph = make_interactionMixgraph(true, n);
            break;
        case INITIAL_GRAPH::NUMBER:
            interactionGraph = make_interactionNumberGraph(true);
            break;
        case INITIAL_GRAPH::ORDER:
            interactionGraph = make_interactionGraph(true);
            break;
    }
    interactionGraph.build_dist_table();
#if DEBUG_NEW_INITIAL
    cout << interactionGraph << endl;
#endif

    // Make interaction graph
    Graph interaction_graph_order     = make_interactionGraph(true);
    Graph interaction_graph_number    = make_interactionNumberGraph(true);
    interaction_graph_order.build_dist_table();
    interaction_graph_number.build_dist_table();
    
    int idx_order, idx_num, idx_sub_order, idx_sub_num;
    bool equal_order, equal_num, equal_sub_order, equal_sub_num;
   
    //cout << endl <<"* Interaction Order Graph" << endl;
    pair<int, bool> graph_charact = interaction_graph_order.graph_characteristics(true);
    idx_order   = graph_charact.first;
    equal_order = graph_charact.second;
    //cout << interaction_graph_order << endl;
    //cout << "=> idx_order: " << idx_order << ", equal_order: " << equal_order << endl;
    
    //cout << endl << "* Interaction Number Graph" << endl;
    graph_charact = interaction_graph_number.graph_characteristics(false);
    idx_num   = graph_charact.first;
    equal_num = graph_charact.second;
    //cout << interaction_graph_number << endl;
    //cout << "==> idx_number: " << idx_num << ", equal_num: " << equal_num << endl;
    
    //// Make interaction graph's queue
    //cout << "------------------------------------------" << endl;
    //int q_in_order = interaction_graph_order.cal_graph_center();
    //queue<int> igraph_order_queue;
    //bfs_queue_gen(igraph_order_queue, interaction_graph_order, q_in_order, true);
    //cout << "igraph_order_queue: " << igraph_order_queue << endl;
    //
    //int q_in_number = interaction_graph_number.cal_graph_center();
    //queue<int> igraph_number_queue;
    //bfs_queue_gen(igraph_number_queue, interaction_graph_number, q_in_number, false);
    //cout << "igraph_number_queue: " << igraph_number_queue << endl;



    ///////////////////////////////////////////////////////////////////////////
#if DEBUG_GRAPH
    cout << endl << " *** GRAPH Matching Processing Start  ***" << endl;
#endif
    
    int qc = interaction_graph_number.cal_graph_center();
    //int qc = idx_order;
    //int qc = idx_num;
    int Qc = coupling_graph.cal_graph_center();
    
    queue<int> igraph_qc_queue, igraph_bfs_queue;
    int graph_dist, graph_node;
    //bfs_queue_gen(igraph_qc_queue, interaction_graph_number, qc, false);
    //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, qc, true);
    bfs_queue_gen(igraph_qc_queue, interactionGraph, qc, false);
    bfs_queue_gen(igraph_bfs_queue, interactionGraph, qc, false);

#if DEBUG_GRAPH
    cout << endl; 
    cout << "igraph_qc_queue: " << igraph_qc_queue << endl;
    cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif

    vector<int> candi_loc1, candi_loc2;
    int location_degree;
    bool first_loop = true;

    while(!igraph_qc_queue.empty())
    {
        int current_qc = igraph_qc_queue.front();
        bool dist_1_mapping = false;
        cout << " ";
#if DEBUG_qc
        cout << endl << "---------------------   qc: "  << current_qc << "    -----------------------" << endl;
#endif      
        // (1) qc mapping
        if(first_loop)
        {
#if DEBUG_G
            cout << "- qc: " << qc << "-> Qc: " << Qc << endl;
#endif
            qubit_Q[Qc] = qc;
            layout_L[qc] = Qc;

            dist_1_mapping = true;
        }
        else
        {
            if(layout_L[current_qc]!=-1 && equal_num!=true)
            {
                dist_1_mapping = true;
            }
            else if(layout_L[current_qc]!=-1 && equal_num== true)
            {
                dist_1_mapping = false;
            }
            //new center --> mapping
            else
            {
                //find physical qubit!!!!!!!!!!!!
                vector<int> ref_loc;
                make_ref_loc(ref_loc, interaction_graph_number, current_qc, false);
#if DEBUG_qc
                cout << "ref_loc: " << ref_loc << endl;
#endif
                
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_number, false);
                //make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_order, true);
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
                sort_degree(candi_loc1, coupling_graph);
                Qc = candi_loc1[0];
                qubit_Q[Qc] = current_qc;
                layout_L[current_qc] = Qc;

                dist_1_mapping = true;
#if DEBUG_G
                cout << "candi_loc1: " << candi_loc1 << endl;
                cout << "-> Qc: " << Qc << endl;
#endif                
            }

            // generate new bfs_queue
            while(!igraph_bfs_queue.empty())
                igraph_bfs_queue.pop();
            bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, current_qc, equal_num);
            //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, current_qc, equal_num);
#if DEBUG_GRAPH
            cout << endl; 
            cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // (2) dist_1 set
        cout << " ";
        
        if(dist_1_mapping)
        {
            //Location to center --> temp_set1, temp_set2
            vector<int> temp_set1, temp_set2;
            vector<pair<int, int>> dist_temp_set;
            bool first_bfs = true; 
            
            //candi_location update (disjoint set) --> candi_loc1, candi_loc2
            candi_loc1.clear();
            candi_loc2.clear();
            make_candi_loc_3(layout_L[current_qc], coupling_graph, candi_loc1, candi_loc2, location_degree);
#if DEBUG_G
            cout << "candi_loc1: " << candi_loc1 << endl;
            cout << "candi_loc2: " << candi_loc2 << endl;
            cout << "location_degree: " << location_degree << endl;
            cout << endl;
#endif            
            
            while(!igraph_bfs_queue.empty())
            {
                int current_bfs = igraph_bfs_queue.front();
                int dist_from_qc = interaction_graph_number.dist[current_qc][igraph_bfs_queue.front()];
                //int dist_from_qc = interaction_graph_order.dist[current_qc][igraph_bfs_queue.front()];
#if DEBUG_G
                cout << "current_bfs: " << current_bfs;
#endif
                if(current_qc == current_bfs)
                {   
#if DEBUG_G
                    cout << "\t-> skip (current_qc)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //has physical qubit
                else if(layout_L[current_bfs]!=-1)
                {   
#if DEBUG_G
                    cout << "\t-> skip (has physical qubit)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //not dist_1
                else if(dist_from_qc != 1)
                {
#if DEBUG_G
                    cout << "\t-> skip (not dist_1)" << endl;
#endif
                    dist_temp_set.push_back(make_pair(dist_from_qc, current_bfs));
                    igraph_bfs_queue.pop();
                }
                //only dist_1_set
                else
                {
                    if(!first_bfs)
                    {
                        if(interaction_graph_number.dist[temp_set1[0]][current_bfs] == 1)
                        //if(interaction_graph_order.dist[temp_set1[0]][current_bfs] == 1)
                        {
#if DEBUG_G
                            cout << "\t-> temp_set1" << endl;
#endif
                            temp_set1.push_back(current_bfs);
                        }
                        else
                        {
#if DEBUG_G
                            cout << "\t-> temp_set2" << endl;
#endif
                            temp_set2.push_back(current_bfs);
                        }
                        igraph_bfs_queue.pop();
                    }
                    else
                    {
#if DEBUG_G
                        cout << "\t-> temp_set1" << endl;
#endif
                        temp_set1.push_back(igraph_bfs_queue.front());
                        igraph_bfs_queue.pop();

                        first_bfs = false;
                    }
                }
            }

            graph_dist = interaction_graph_order.dist[current_qc][igraph_bfs_queue.back()];
            //graph_dist = interaction_graph_number.dist[current_qc][igraph_bfs_queue.back()];
            graph_node = temp_set1.size() + temp_set2.size();
#if DEBUG_G
            cout << " ## Graph shape? " << endl;
            cout << "graph_dist: " << graph_dist << endl;
            cout << "graph_node: " << graph_node << endl;
#endif
            
            //dist_1_set is empty
            if(temp_set1.size()==0 && temp_set2.size()==0 && graph_node >graph_dist)
            {
                //cout << "dist_temp_set: " << dist_temp_set << endl;
                for(auto dist_temp : dist_temp_set)
                {
                    if(dist_temp.first == dist_temp_set[0].first)
                        temp_set1.push_back(dist_temp.second);
                    else if(dist_temp.first == dist_temp_set[0].first+1)
                        temp_set2.push_back(dist_temp.second);
                    else
                        continue;
                }
            }

            //mapping temp_set-->candi_loc
            if(temp_set1.size() < temp_set2.size() )
                swap(temp_set1, temp_set2);
#if DEBUG_G
            cout << endl;
            cout << "temp_set1: " << temp_set1 << endl;
            cout << "temp_set2: " << temp_set2 << endl;
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            int Q_location;
#if DEBUG_G
            cout << endl << " (3-1) temp_set -> candi_loc mapping " << endl;
            cout << "temp_set1.size: " << temp_set1.size() << ", candi_loc1.size: " << candi_loc1.size() << endl;
            cout << "temp_set2.size: " << temp_set2.size() << ", candi_loc2.size: " << candi_loc2.size() << endl;
#endif
/*
            /////////////////////////////////////////////////////////
            queue<int> candiQubit1, candiQubit2;
            queue<int> candiLocQueue1, candiLocQueue2;
            for(auto i : temp_set1)
                candiQubit1.push(i);
            for(auto i : temp_set2)
                candiQubit2.push(i);
            for(auto i : candi_loc1)
                candiLocQueue1.push(i);
            for(auto i : candi_loc2)
                candiLocQueue2.push(i);
            
            int mappedQubit1=0;
            int mappedQubit2=0;
            int mappedLoc1=0;
            int mappedLoc2=0;

            if(!candiQubit1.empty() || !candiQubit2.empty())
            {
                int queueIter = 0;
                while(!candiLocQueue1.empty())
                {
                    int QQ = candiLocQueue1.front();
                    int qq = candiQubit1.front();
                    if(candiQubit1.empty()) break;
                    layout_L[qq] = QQ;
                    qubit_Q[QQ] = qq;
#if DEBUG_G
                    cout << "- q: " << qq << "-> Q" << QQ << endl;
#endif                    
                    candiLocQueue1.pop();
                    candiQubit1.pop();
                    queueIter++;

                    mappedQubit1++;
                    mappedLoc1++;
                }

                queueIter = 0;
                while(!candiLocQueue2.empty())
                {
                    int QQ = candiLocQueue2.front();
                    int qq = candiQubit2.front();
                    if(candiQubit2.empty()) break;
                    layout_L[qq] = QQ;
                    qubit_Q[QQ] = qq;
#if DEBUG_G
                    cout << "- q: " << qq << "-> Q" << QQ << endl;
#endif                    
                    candiLocQueue2.pop();
                    candiQubit2.pop();
                    queueIter++;
                    
                    mappedQubit1++;
                    mappedLoc1++;
                }
#if DEBUG_G
                print_layout(layout_L);
#endif
            }
*/            
///*
            //for candi_loc1    //////////////////////
            if(candi_loc1.size() >= temp_set1.size())
            {
                int Q_index = 0;
                for(auto q : temp_set1)
                {
                    int Q_location = candi_loc1[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc1.erase(candi_loc1.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set1.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc1)
                {
                    int qq = temp_set1[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc1.clear();
            }
            
            //for candi_loc2    //////////////////////
            if(candi_loc2.size() >= temp_set2.size())
            {
                int Q_index=0;
                for(auto q : temp_set2)
                {
                    int Q_location = candi_loc2[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc2.erase(candi_loc2.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set2.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc2)
                {
                    int qq = temp_set2[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set2.erase(temp_set2.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc2.clear();
            }
//*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //remained node??
            /*
            temp_set1.clear();
            temp_set2.clear();
            candi_loc1.clear();
            candi_loc2.clear();
            for(int i=0; i<candiQubit1.size(); i++)
            {
                temp_set1.push_back(candiQubit1.front());
                candiQubit1.pop();
            }
            for(int i=0; i<candiQubit2.size(); i++)
            {
                temp_set2.push_back(candiQubit2.front());
                candiQubit2.pop();
            }
            for(int i=0; i<candiLocQueue1.size(); i++)
            {
                candi_loc1.push_back(candiLocQueue1.front());
                candiLocQueue1.pop();
            }
            for(int i=0; i<candiLocQueue2.size(); i++)
            {
                candi_loc2.push_back(candiLocQueue2.front());
                candiLocQueue2.pop();
            }
            */


            if(temp_set1.size() + temp_set2.size() >= 0 && Dgraph.nodeset.size()>=3500)
            {
                //candi & temp_set update!
                temp_set1.insert(temp_set1.end(), temp_set2.begin(), temp_set2.end());
                candi_loc1.insert(candi_loc1.end(), candi_loc2.begin(), candi_loc2.end());
#if DEBUG_G
                cout << endl << " (3-2) Remained node! " << endl;
                cout << "temp_set1: " << temp_set1 << endl;
                cout << "candi_loc1: " << candi_loc1 << endl;
#endif
                if(candi_loc1.size() >= temp_set1.size())
                {
                    int Q_index = 0;
                    for(auto q : temp_set1)
                    {
                        int Q_location = candi_loc1[Q_index];
                        qubit_Q[Q_location] = q;
                        layout_L[q] = Q_location;
                        
                        candi_loc1.erase( remove(candi_loc1.begin(), candi_loc1.end(), Q_location) );
#if DEBUG_G
                        cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                    }
                    temp_set1.clear();

                }
                else
                {
                    int q_index = 0;
                    for(auto Q : candi_loc1)
                    {
                        int qq = temp_set1[q_index];
                        qubit_Q[Q] = qq;
                        layout_L[qq] = Q;
                        temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                        cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                    }
                    candi_loc1.clear();
                }
            }
            queue<int> temp_bfs_queue2;

            for(auto v : temp_set1)
            {
                temp_bfs_queue2.push(v);
            }
            while(!igraph_bfs_queue.empty())
            {
                temp_bfs_queue2.push(igraph_bfs_queue.front());
                igraph_bfs_queue.pop();
            }
            igraph_bfs_queue = temp_bfs_queue2;
#if DEBUG_G
            cout << "-> remained igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }
        // (3) ready to next loop
        first_loop = false;
        igraph_qc_queue.pop();
        dist_1_mapping = false;

    }




}

void Qcircuit::QMapper::new_graph_matching_processing(ARCHITECTURE archi, INITIAL_GRAPH graphtype)
{
    //initiallize layout -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; 
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  
  
    //CNOT set
    make_CNOT(true);
    
    int n = param_beta;
    if (nqubits<=10) 
        n = 2;
   
    // Make interaction graph
    Graph interaction_graph;
    switch(graphtype)
    {
        case INITIAL_GRAPH::MIX:
            interaction_graph = make_interactionMixgraph(true, n);
            break;
        case INITIAL_GRAPH::NUMBER:
            interaction_graph = make_interactionNumberGraph(true);
            break;
        case INITIAL_GRAPH::ORDER:
            interaction_graph = make_interactionGraph(true);
            break;
    }
    interaction_graph.build_dist_table();
#if DEBUG_NEW_INITIAL
    cout << interaction_graph << endl;
#endif

    ////////////////
    Graph interaction_graph_order  = make_interactionGraph(true);
    Graph interaction_graph_number = make_interactionNumberGraph(true);
    interaction_graph_order.build_dist_table();
    interaction_graph_number.build_dist_table();
    ////////////////

#if DEBUG_NEW_INITIAL
    cout << endl << " *** GRAPH Matching Processing Start  ***" << endl;
#endif
    int qc = interaction_graph.cal_graph_center_weight();
    int Qc = coupling_graph.cal_graph_center();

    //build degree_table
    coupling_graph.build_degree_table();
    coupling_graph.build_degree_n();
    coupling_graph.build_doubledegree_n();
    
    //cout << "exampleDegree: ";
    //for(int i=0; i<positions; i++)
    //    cout << setw(3) << coupling_graph.degreeN[i] << " ";
    //cout << "exampleDouble: ";
    //for(int i=0; i<positions; i++)
    //    cout << setw(3) << coupling_graph.doubleDegreeN[i] << " ";

    //coupling_graph.remove_degree_n(7);
    
    //cout << "exampleDegree: ";
    //for(int i=0; i<positions; i++)
    //    cout << setw(3) << coupling_graph.degreeN[i] << " ";
    //cout << "exampleDouble: ";
    //for(int i=0; i<positions; i++)
    //    cout << setw(3) << coupling_graph.doubleDegreeN[i] << " ";

    queue<int> igraph_qc_queue;
    bfs_queue_gen(igraph_qc_queue, interaction_graph, qc, false);
#if DEBUG_NEW_INITIAL
    cout << "igraph_qc_queue: " << igraph_qc_queue << endl;
#endif
   
    /////
    queue<int> igraph_order_queue, igraph_number_queue;
    bfs_queue_gen(igraph_order_queue, interaction_graph_order, qc, true);
    bfs_queue_gen(igraph_number_queue, interaction_graph_number, qc, false);
#if DEBUG_NEW_INITIAL
    cout << "igraph_order_queue: " << igraph_order_queue << endl;
    cout << "igraph_number_queue: " << igraph_number_queue << endl;
#endif
    /////
    
    vector<int> candiLoc1, candiLoc2;

    // mapping
    bool firstLoop = true;
    while(!igraph_qc_queue.empty())
    {
        int currentQc = igraph_qc_queue.front();
        bool dist_1_mapping = false;
#if DEBUG_NEW_INITIAL
        cout << endl << "---------------------   qc: "  << currentQc << "    -----------------------" << endl;
#endif      
        // (1) qc mapping
        if(firstLoop)
        {
#if DEBUG_NEW_INITIAL
            cout << "- qc: " << qc << "-> Qc: " << Qc << endl;
#endif
            layout_L[qc] = Qc;
            qubit_Q[Qc] = qc;
            igraph_qc_queue.pop();
        }
        else
        {
            if(layout_L[currentQc]!=-1)
            {
#if DEBUG_NEW_INITIAL
                cout << "mapping is not required" << endl;
#endif
            }
            else
            {
#if DEBUG_NEW_INITIAL
                cout << "mapping is required" << endl;
#endif
                ////////////////////////////////////////////////////////////////////
                vector<int> refLoc, candiLoc;
                make_ref_loc(refLoc, interaction_graph, currentQc, false);
#if DEBUG_NEW_INITIAL
                cout << "ref_loc: " << refLoc << endl;
#endif
                make_candi_loc_dist(currentQc, candiLoc, refLoc, coupling_graph, interaction_graph, false);
                sort_degree(candiLoc, coupling_graph);
#if DEBUG_NEW_INITIAL
                cout << "candi_loc: " << candiLoc << endl;
#endif
                layout_L[currentQc] = candiLoc[0];
                qubit_Q[candiLoc[0]] = currentQc;
#if DEBUG_NEW_INITIAL
                cout << "- qc: " << currentQc << "-> Qc: " << candiLoc[0] << endl;
#endif
                ////////////////////////////////////////////////////////////////////
            }
            igraph_qc_queue.pop();
        }
       
        // (2) dist_1 set
        int location_degree;
        candiLoc1.clear();
        candiLoc2.clear();
        make_candi_loc_degree(layout_L[currentQc], coupling_graph, candiLoc1, candiLoc2);
#if DEBUG_NEW_INITIAL
        //cout << "candiLoc1: " << candiLoc1 << endl;  
        //cout << "candiLoc2: " << candiLoc2 << endl;  
#endif
        
        queue<int> candiLocQueue1, candiLocQueue2;
        for(auto i : candiLoc1)
            candiLocQueue1.push(i);
        for(auto i : candiLoc2)
            candiLocQueue2.push(i);
#if DEBUG_NEW_INITIAL
        cout << "candiLocQueue1: " << candiLocQueue1 << endl;  
        cout << "candiLocQueue2: " << candiLocQueue2 << endl;  
#endif        

        queue<int> igraph_bfs_queue;
        bfs_queue_gen(igraph_bfs_queue, interaction_graph, currentQc, false);
        //bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, currentQc, false);
        //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, currentQc, true);
#if DEBUG_NEW_INITIAL
        cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        queue<int> candiQubit1, candiQubit2;
        vector<pair<int, int>> distTemp;
        int firstBfs;
        while(!igraph_bfs_queue.empty())
        {
            int currentBfs = igraph_bfs_queue.front();
            int distFromQc = interaction_graph.dist[currentQc][currentBfs];
            //cout << "currentBfs: " << currentBfs << endl;
            if(currentQc == currentBfs)
            {
                igraph_bfs_queue.pop();
                firstBfs = igraph_bfs_queue.front();
            }
            else if(layout_L[currentBfs]!=-1)
                igraph_bfs_queue.pop();
            else if(distFromQc!=1)
            {
                distTemp.push_back(make_pair(distFromQc, currentBfs) );
                igraph_bfs_queue.pop();
            }
            else    //only dist-1 set
            {
                if(currentBfs == firstBfs)
                {
                    candiQubit1.push(currentBfs);
                    igraph_bfs_queue.pop();
                }
                else if(interaction_graph.dist[currentBfs][firstBfs]==1)
                {
                    candiQubit1.push(currentBfs);
                    igraph_bfs_queue.pop();
                }
                else
                {
                    candiQubit2.push(currentBfs);
                    igraph_bfs_queue.pop();
                }

            }
        }
#if DEBUG_NEW_INITIAL
        cout << "candiQubit1: " << candiQubit1 << endl;  
        cout << "candiQubit2: " << candiQubit2 << endl;  
#endif
        if(!candiQubit1.empty() || !candiQubit2.empty())
        {
            int queueIter = 0;
            while(!candiLocQueue1.empty())
            {
                int QQ = candiLocQueue1.front();
                int qq = candiQubit1.front();
                if(candiQubit1.empty()) break;
                layout_L[qq] = QQ;
                qubit_Q[QQ] = qq;
#if DEBUG_NEW_INITIAL
        cout << "- q" << qq << "-> Q" << QQ << endl;  
#endif
                candiLocQueue1.pop();
                candiQubit1.pop();
                queueIter++;
                coupling_graph.remove_degree_n(QQ);
            }
            queueIter = 0;
            while(!candiLocQueue2.empty())
            {
                int QQ = candiLocQueue2.front();
                int qq = candiQubit2.front();
                if(candiQubit2.empty()) break;
                layout_L[qq] = QQ;
                qubit_Q[QQ] = qq;
#if DEBUG_NEW_INITIAL
        cout << "- q" << qq << "-> Q" << QQ << endl;  
#endif
                candiLocQueue2.pop();
                candiQubit2.pop();
                queueIter++;
                coupling_graph.remove_degree_n(QQ);
            }
#if DEBUG_NEW_INITIAL
            print_layout(layout_L);
#endif
        }

  
        // (3) ready to next loop
        firstLoop = false;
        coupling_graph.remove_degree_n(Qc);
    }

}











void Qcircuit::QMapper::graph_matching_processing_v3(ARCHITECTURE archi, INITIAL_GRAPH graphtype)
{
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical
  

    //CNOT set
    make_CNOT(true);
    
    //int n = Dgraph_cnot.nodeset.size()/param_beta;
    int n = param_beta;
    //if (nqubits<=10 || nqubits==20) 
    if (nqubits<=10) 
        n = 1;
    


    //Make interaction graph
    Graph interactionGraph;
    switch(graphtype)
    {
        case INITIAL_GRAPH::MIX:
            interactionGraph = make_interactionMixgraph(true, n);
            break;
        case INITIAL_GRAPH::NUMBER:
            interactionGraph = make_interactionNumberGraph(true);
            break;
        case INITIAL_GRAPH::ORDER:
            interactionGraph = make_interactionGraph(true);
            break;
    }
    interactionGraph.build_dist_table();
#if DEBUG_NEW_INITIAL
    cout << interactionGraph << endl;
#endif


    // Make interaction graph
    Graph interaction_graph_order     = make_interactionGraph(true);
    Graph interaction_graph_number    = make_interactionNumberGraph(true);
    interaction_graph_order.build_dist_table();
    interaction_graph_number.build_dist_table();
    
    int idx_order, idx_num, idx_sub_order, idx_sub_num;
    bool equal_order, equal_num, equal_sub_order, equal_sub_num;
   
    //cout << endl <<"* Interaction Order Graph" << endl;
    pair<int, bool> graph_charact = interaction_graph_order.graph_characteristics(true);
    idx_order   = graph_charact.first;
    equal_order = graph_charact.second;
    //cout << interaction_graph_order << endl;
    //cout << "=> idx_order: " << idx_order << ", equal_order: " << equal_order << endl;
    
    //cout << endl << "* Interaction Number Graph" << endl;
    graph_charact = interaction_graph_number.graph_characteristics(false);
    idx_num   = graph_charact.first;
    equal_num = graph_charact.second;
    //cout << interaction_graph_number << endl;
    //cout << "==> idx_number: " << idx_num << ", equal_num: " << equal_num << endl;
    
//    // Make interaction graph's queue
//    cout << "------------------------------------------" << endl;
//    int q_in_order = interaction_graph_order.cal_graph_center();
//    queue<int> igraph_order_queue;
//    bfs_queue_gen(igraph_order_queue, interaction_graph_order, q_in_order, true);
//    cout << "igraph_order_queue: " << igraph_order_queue << endl;
//    
//    int q_in_number = interaction_graph_number.cal_graph_center();
//    queue<int> igraph_number_queue;
//    bfs_queue_gen(igraph_number_queue, interaction_graph_number, q_in_number, false);
//    cout << "igraph_number_queue: " << igraph_number_queue << endl;



    ///////////////////////////////////////////////////////////////////////////
#if DEBUG_GRAPH
    cout << endl << " *** GRAPH Matching Processing Start  ***" << endl;
#endif
    
    int qc = interaction_graph_number.cal_graph_center();
    //int qc = idx_order;
    //int qc = idx_num;
    int Qc = coupling_graph.cal_graph_center();
    
    queue<int> igraph_qc_queue, igraph_bfs_queue;
    int graph_dist, graph_node;
    //bfs_queue_gen(igraph_qc_queue, interaction_graph_number, qc, false);
    //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, qc, true);
    bfs_queue_gen(igraph_qc_queue, interactionGraph, qc, false);
    bfs_queue_gen(igraph_bfs_queue, interactionGraph, qc, false);

#if DEBUG_GRAPH
    cout << endl; 
    cout << "igraph_qc_queue: " << igraph_qc_queue << endl;
    cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif

    vector<int> candi_loc1, candi_loc2;
    int location_degree;
    bool first_loop = true;

    while(!igraph_qc_queue.empty())
    {
        int current_qc = igraph_qc_queue.front();
        bool dist_1_mapping = false;
        cout << " ";
#if DEBUG_qc
        cout << endl << "---------------------   qc: "  << current_qc << "    -----------------------" << endl;
#endif      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // (1) qc mapping
        if(first_loop)
        {
#if DEBUG_G
            cout << " (1) circuit center -> physical center " << endl;
            cout << "qc: " << qc << endl;
            cout << "-> Qc: " << Qc << endl;
#endif
            qubit_Q[Qc] = qc;
            layout_L[qc] = Qc;

            dist_1_mapping = true;
        }
        else
        {
//            //center mapping is not required
            if(layout_L[current_qc]!=-1 && equal_num!=true)
            {
                cout << " ";
#if DEBUG_qc
                cout << " (1) circuit center -> physical center " << endl;
                cout << "Current_qc has a physical qubit! & dist_1_mapping start!" << endl;
#endif
                dist_1_mapping = true;
            }
            //center mapping is not required
            else if(layout_L[current_qc]!=-1 && equal_num== true)
            {
                cout << " ";
#if DEBUG_qc
                cout << " (1) circuit center -> physical center " << endl;
                cout << "Current_qc has a physical qubit! & no dist_1_mapping" << endl;
#endif
                dist_1_mapping = false;
            }
            //new center --> mapping
            else
            {
                //find physical qubit!!!!!!!!!!!!
#if DEBUG_qc
                cout << "(1) Find physical qubit for current qc" << endl;
#endif
                vector<int> ref_loc;
                make_ref_loc(ref_loc, interaction_graph_number, current_qc, false);
#if DEBUG_qc
                cout << "ref_loc: " << ref_loc << endl;
#endif
                
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_number, false);
                //make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_order, true);
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
                sort_degree(candi_loc1, coupling_graph);
                Qc = candi_loc1[0];
                qubit_Q[Qc] = current_qc;
                layout_L[current_qc] = Qc;

                dist_1_mapping = true;
#if DEBUG_G
                cout << "candi_loc1: " << candi_loc1 << endl;
                cout << "-> Qc: " << Qc << endl;
#endif                
            }

            // generate new bfs_queue
            while(!igraph_bfs_queue.empty())
                igraph_bfs_queue.pop();
            bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, current_qc, equal_num);
            //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, current_qc, equal_num);
#if DEBUG_GRAPH
            cout << endl; 
            cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // (2) dist_1 set
        cout << " ";
#if DEBUG_qc
        cout    << " (2) candi_loc (physical) & temp_set (logical) update " << endl;
#endif
        if(dist_1_mapping)
        {
            //Location to center --> temp_set1, temp_set2
            vector<int> temp_set1, temp_set2;
            vector<pair<int, int>> dist_temp_set;
            bool first_bfs = true; 
            
            //candi_location update (disjoint set) --> candi_loc1, candi_loc2
            candi_loc1.clear();
            candi_loc2.clear();
            make_candi_loc_3(layout_L[current_qc], coupling_graph, candi_loc1, candi_loc2, location_degree);
#if DEBUG_G
            cout << "candi_loc1: " << candi_loc1 << endl;
            cout << "candi_loc2: " << candi_loc2 << endl;
            cout << "location_degree: " << location_degree << endl;
            cout << endl;
#endif            
            
            while(!igraph_bfs_queue.empty())
            {
                int current_bfs = igraph_bfs_queue.front();
                int dist_from_qc = interaction_graph_number.dist[current_qc][igraph_bfs_queue.front()];
                //int dist_from_qc = interaction_graph_order.dist[current_qc][igraph_bfs_queue.front()];
#if DEBUG_G
                cout << "current_bfs: " << current_bfs;
#endif
                if(current_qc == current_bfs)
                {   
#if DEBUG_G
                    cout << "\t-> skip (current_qc)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //has physical qubit
                else if(layout_L[current_bfs]!=-1)
                {   
#if DEBUG_G
                    cout << "\t-> skip (has physical qubit)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //not dist_1
                else if(dist_from_qc != 1)
                {
#if DEBUG_G
                    cout << "\t-> skip (not dist_1)" << endl;
#endif
                    dist_temp_set.push_back(make_pair(dist_from_qc, current_bfs));
                    igraph_bfs_queue.pop();
                }
                //only dist_1_set
                else
                {
                    if(!first_bfs)
                    {
                        if(interaction_graph_number.dist[temp_set1[0]][current_bfs] == 1)
                        //if(interaction_graph_order.dist[temp_set1[0]][current_bfs] == 1)
                        {
#if DEBUG_G
                            cout << "\t-> temp_set1" << endl;
#endif
                            temp_set1.push_back(current_bfs);
                        }
                        else
                        {
#if DEBUG_G
                            cout << "\t-> temp_set2" << endl;
#endif
                            temp_set2.push_back(current_bfs);
                        }
                        igraph_bfs_queue.pop();
                    }
                    else
                    {
#if DEBUG_G
                        cout << "\t-> temp_set1" << endl;
#endif
                        temp_set1.push_back(igraph_bfs_queue.front());
                        igraph_bfs_queue.pop();

                        first_bfs = false;
                    }
                }
            }

            graph_dist = interaction_graph_order.dist[current_qc][igraph_bfs_queue.back()];
            //graph_dist = interaction_graph_number.dist[current_qc][igraph_bfs_queue.back()];
            graph_node = temp_set1.size() + temp_set2.size();
#if DEBUG_G
            cout << " ## Graph shape? " << endl;
            cout << "graph_dist: " << graph_dist << endl;
            cout << "graph_node: " << graph_node << endl;
#endif
            
            //dist_1_set is empty
            if(temp_set1.size()==0 && temp_set2.size()==0 && graph_node >graph_dist)
            {
                //cout << "dist_temp_set: " << dist_temp_set << endl;
                for(auto dist_temp : dist_temp_set)
                {
                    if(dist_temp.first == dist_temp_set[0].first)
                        temp_set1.push_back(dist_temp.second);
                    else if(dist_temp.first == dist_temp_set[0].first+1)
                        temp_set2.push_back(dist_temp.second);
                    else
                        continue;
                }
            }

            //mapping temp_set-->candi_loc
            if(temp_set1.size() < temp_set2.size() )
                swap(temp_set1, temp_set2);
#if DEBUG_G
            cout << endl;
            cout << "temp_set1: " << temp_set1 << endl;
            cout << "temp_set2: " << temp_set2 << endl;
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            int Q_location;
#if DEBUG_G
            cout << endl << " (3-1) temp_set -> candi_loc mapping " << endl;
            cout << "temp_set1.size: " << temp_set1.size() << ", candi_loc1.size: " << candi_loc1.size() << endl;
            cout << "temp_set2.size: " << temp_set2.size() << ", candi_loc2.size: " << candi_loc2.size() << endl;
#endif

            //for candi_loc1    //////////////////////
            if(candi_loc1.size() >= temp_set1.size())
            {
                int Q_index = 0;
                for(auto q : temp_set1)
                {
                    int Q_location = candi_loc1[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc1.erase(candi_loc1.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set1.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc1)
                {
                    int qq = temp_set1[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc1.clear();
            }
            
            //for candi_loc2    //////////////////////
            if(candi_loc2.size() >= temp_set2.size())
            {
                int Q_index=0;
                for(auto q : temp_set2)
                {
                    int Q_location = candi_loc2[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc2.erase(candi_loc2.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set2.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc2)
                {
                    int qq = temp_set2[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set2.erase(temp_set2.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc2.clear();
            }




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //remained node??
            if(temp_set1.size() + temp_set2.size() >= 0 && Dgraph.nodeset.size()>=3500)
            {
                //candi & temp_set update!
                temp_set1.insert(temp_set1.end(), temp_set2.begin(), temp_set2.end());
                candi_loc1.insert(candi_loc1.end(), candi_loc2.begin(), candi_loc2.end());
#if DEBUG_G
                cout << endl << " (3-2) Remained node! " << endl;
                cout << "temp_set1: " << temp_set1 << endl;
                cout << "candi_loc1: " << candi_loc1 << endl;
#endif
                if(candi_loc1.size() >= temp_set1.size())
                {
                    int Q_index = 0;
                    for(auto q : temp_set1)
                    {
                        int Q_location = candi_loc1[Q_index];
                        qubit_Q[Q_location] = q;
                        layout_L[q] = Q_location;
                        
                        candi_loc1.erase( remove(candi_loc1.begin(), candi_loc1.end(), Q_location) );
#if DEBUG_G
                        cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                    }
                    temp_set1.clear();

                }
                else
                {
                    int q_index = 0;
                    for(auto Q : candi_loc1)
                    {
                        int qq = temp_set1[q_index];
                        qubit_Q[Q] = qq;
                        layout_L[qq] = Q;
                        temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                        cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                    }
                    candi_loc1.clear();
                }
            }
            
            queue<int> temp_bfs_queue2;

            for(auto v : temp_set1)
            {
                temp_bfs_queue2.push(v);
            }
            while(!igraph_bfs_queue.empty())
            {
                temp_bfs_queue2.push(igraph_bfs_queue.front());
                igraph_bfs_queue.pop();
            }
            igraph_bfs_queue = temp_bfs_queue2;
#if DEBUG_G
            cout << "-> remained igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }
        // (3) ready to next loop
        first_loop = false;
        igraph_qc_queue.pop();
        dist_1_mapping = false;

    }




}


void Qcircuit::QMapper::graph_matching_processing_v2(ARCHITECTURE archi)
{
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical
  

    //CNOT set
    make_CNOT(true);
    
    int n = Dgraph_cnot.nodeset.size()/param_beta;
    if (nqubits<=10 || nqubits==20) 
        n = Dgraph_cnot.nodeset.size();
    

    // Make interaction graph
    Graph interaction_graph_order     = make_interactionGraph(true);
    Graph interaction_graph_number    = make_interactionNumberGraph(true);
    interaction_subgraph_order        = make_interactionSubgraph_order(true, n);
    interaction_subgraph_number       = make_interactionSubgraph_num(true, n);

    interaction_graph_order.build_dist_table();
    interaction_graph_number.build_dist_table();
    interaction_subgraph_order.build_dist_table();
    interaction_subgraph_number.build_dist_table();


    ////
    int idx_order, idx_num, idx_sub_order, idx_sub_num;
    bool equal_order, equal_num, equal_sub_order, equal_sub_num;
   
    cout << endl <<"* Interaction Order Graph" << endl;
    pair<int, bool> graph_charact = interaction_graph_order.graph_characteristics(true);
    idx_order   = graph_charact.first;
    equal_order = graph_charact.second;
    cout << "=> idx_order: " << idx_order << ", equal_order: " << equal_order << endl;

    cout << endl << "* Interaction Number Graph" << endl;
    graph_charact = interaction_graph_number.graph_characteristics(false);
    idx_num   = graph_charact.first;
    equal_num = graph_charact.second;
    cout << "==> idx_number: " << idx_num << ", equal_num: " << equal_num << endl;
    
    cout << endl << "* Interaction Subgraph_order" << endl;
    graph_charact = interaction_subgraph_order.graph_characteristics(true);
    idx_sub_order   = graph_charact.first;
    equal_sub_order = graph_charact.second;
    cout << "==> idx_sub_order: " << idx_sub_order << ", equal_sub_order: " << equal_sub_order << endl;
    
    cout << endl << "* Interaction Subgraph_num" << endl;
    graph_charact = interaction_subgraph_number.graph_characteristics(false);  
    idx_sub_num   = graph_charact.first;
    equal_sub_num = graph_charact.second;
    cout << "==> idx_sub_number: " << idx_sub_num << ", equal_sub_num: " << equal_sub_num << endl;
    cout << endl;


//    // Make interaction graph's queue
//    cout << "------------------------------------------" << endl;
//    int q_in_order = interaction_graph_order.cal_graph_center();
//    queue<int> igraph_order_queue;
//    bfs_queue_gen(igraph_order_queue, interaction_graph_order, q_in_order, true);
//    cout << "igraph_order_queue: " << igraph_order_queue << endl;
//    
//    int q_in_number = interaction_graph_number.cal_graph_center();
//    queue<int> igraph_number_queue;
//    bfs_queue_gen(igraph_number_queue, interaction_graph_number, q_in_number, false);
//    cout << "igraph_number_queue: " << igraph_number_queue << endl;
//    
//    int q_in_sub_order = interaction_subgraph_order.cal_graph_center();
//    queue<int> igraph_sub_order_queue;
//    bfs_queue_gen(igraph_sub_order_queue, interaction_subgraph_order, q_in_sub_order, true);
//    cout << "igraph_sub_order_queue: " << igraph_sub_order_queue << endl;
//    
//    int q_in_sub_number = interaction_subgraph_number.cal_graph_center();
//    queue<int> igraph_sub_number_queue;
//    bfs_queue_gen(igraph_sub_number_queue, interaction_subgraph_number, q_in_sub_number, false);
//    cout << "igraph_sub_number_queue: " << igraph_sub_number_queue << endl;
//    cout << "------------------------------------------" << endl << endl;
//



    ///////////////////////////////////////////////////////////////////////////
#if DEBUG_GRAPH
    cout << endl << " *** GRAPH Matching Processing Start  ***" << endl;
#endif
    
    int qc = interaction_graph_number.cal_graph_center();
    //int qc = idx_order;
    //int qc = idx_num;
    int Qc = coupling_graph.cal_graph_center();
    
    queue<int> igraph_qc_queue, igraph_bfs_queue;
    int graph_dist, graph_node;
    bfs_queue_gen(igraph_qc_queue, interaction_graph_number, qc, false);
    bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, qc, true);
    //bfs_queue_gen(igraph_qc_queue, interaction_graph_order, qc, true);
    //bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, qc, false);

#if DEBUG_GRAPH
    cout << endl; 
    cout << "igraph_qc_queue: " << igraph_qc_queue << endl;
    cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif

    vector<int> candi_loc1, candi_loc2;
    int location_degree;
    bool first_loop = true;

    while(!igraph_qc_queue.empty())
    {
        int current_qc = igraph_qc_queue.front();
        bool dist_1_mapping = false;
        cout << " ";
#if DEBUG_qc
        cout << endl << "---------------------   qc: "  << current_qc << "    -----------------------" << endl;
#endif      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // (1) qc mapping
        if(first_loop)
        {
#if DEBUG_G
            cout << " (1) circuit center -> physical center " << endl;
            cout << "qc: " << qc << endl;
            cout << "-> Qc: " << Qc << endl;
#endif
            qubit_Q[Qc] = qc;
            layout_L[qc] = Qc;

            dist_1_mapping = true;
        }
        else
        {
//            //center mapping is not required
            if(layout_L[current_qc]!=-1 && equal_num!=true)
            {
                cout << " ";
#if DEBUG_qc
                cout << " (1) circuit center -> physical center " << endl;
                cout << "Current_qc has a physical qubit! & dist_1_mapping start!" << endl;
#endif
                dist_1_mapping = true;
            }
            //center mapping is not required
            else if(layout_L[current_qc]!=-1 && equal_num== true)
            {
                cout << " ";
#if DEBUG_qc
                cout << " (1) circuit center -> physical center " << endl;
                cout << "Current_qc has a physical qubit! & no dist_1_mapping" << endl;
#endif
                dist_1_mapping = false;
            }
            //new center --> mapping
            else
            {
                //find physical qubit!!!!!!!!!!!!
#if DEBUG_qc
                cout << "(1) Find physical qubit for current qc" << endl;
#endif
                vector<int> ref_loc;
                make_ref_loc(ref_loc, interaction_graph_number, current_qc, false);
#if DEBUG_qc
                cout << "ref_loc: " << ref_loc << endl;
#endif
                
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_number, false);
                //make_candi_loc_dist(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_order, true);
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
                sort_degree(candi_loc1, coupling_graph);
                Qc = candi_loc1[0];
                qubit_Q[Qc] = current_qc;
                layout_L[current_qc] = Qc;

                dist_1_mapping = true;
#if DEBUG_G
                cout << "candi_loc1: " << candi_loc1 << endl;
                cout << "-> Qc: " << Qc << endl;
#endif                
            }

            // generate new bfs_queue
            while(!igraph_bfs_queue.empty())
                igraph_bfs_queue.pop();
            bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, current_qc, equal_num);
            //bfs_queue_gen(igraph_bfs_queue, interaction_graph_order, current_qc, equal_num);
#if DEBUG_GRAPH
            cout << endl; 
            cout << "igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // (2) dist_1 set
        cout << " ";
#if DEBUG_qc
        cout    << " (2) candi_loc (physical) & temp_set (logical) update " << endl;
#endif
        if(dist_1_mapping)
        {
            //Location to center --> temp_set1, temp_set2
            vector<int> temp_set1, temp_set2;
            vector<pair<int, int>> dist_temp_set;
            bool first_bfs = true; 
            
            //candi_location update (disjoint set) --> candi_loc1, candi_loc2
            candi_loc1.clear();
            candi_loc2.clear();
            make_candi_loc_3(layout_L[current_qc], coupling_graph, candi_loc1, candi_loc2, location_degree);
#if DEBUG_G
            cout << "candi_loc1: " << candi_loc1 << endl;
            cout << "candi_loc2: " << candi_loc2 << endl;
            cout << "location_degree: " << location_degree << endl;
            cout << endl;
#endif            
            
            while(!igraph_bfs_queue.empty())
            {
                int current_bfs = igraph_bfs_queue.front();
                int dist_from_qc = interaction_graph_number.dist[current_qc][igraph_bfs_queue.front()];
                //int dist_from_qc = interaction_graph_order.dist[current_qc][igraph_bfs_queue.front()];
#if DEBUG_G
                cout << "current_bfs: " << current_bfs;
#endif
                if(current_qc == current_bfs)
                {   
#if DEBUG_G
                    cout << "\t-> skip (current_qc)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //has physical qubit
                else if(layout_L[current_bfs]!=-1)
                {   
#if DEBUG_G
                    cout << "\t-> skip (has physical qubit)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //not dist_1
                else if(dist_from_qc != 1)
                {
#if DEBUG_G
                    cout << "\t-> skip (not dist_1)" << endl;
#endif
                    dist_temp_set.push_back(make_pair(dist_from_qc, current_bfs));
                    igraph_bfs_queue.pop();
                }
                //only dist_1_set
                else
                {
                    if(!first_bfs)
                    {
                        if(interaction_graph_number.dist[temp_set1[0]][current_bfs] == 1)
                        //if(interaction_graph_order.dist[temp_set1[0]][current_bfs] == 1)
                        {
#if DEBUG_G
                            cout << "\t-> temp_set1" << endl;
#endif
                            temp_set1.push_back(current_bfs);
                        }
                        else
                        {
#if DEBUG_G
                            cout << "\t-> temp_set2" << endl;
#endif
                            temp_set2.push_back(current_bfs);
                        }
                        igraph_bfs_queue.pop();
                    }
                    else
                    {
#if DEBUG_G
                        cout << "\t-> temp_set1" << endl;
#endif
                        temp_set1.push_back(igraph_bfs_queue.front());
                        igraph_bfs_queue.pop();

                        first_bfs = false;
                    }
                }
            }

            graph_dist = interaction_graph_order.dist[current_qc][igraph_bfs_queue.back()];
            //graph_dist = interaction_graph_number.dist[current_qc][igraph_bfs_queue.back()];
            graph_node = temp_set1.size() + temp_set2.size();
#if DEBUG_G
            cout << " ## Graph shape? " << endl;
            cout << "graph_dist: " << graph_dist << endl;
            cout << "graph_node: " << graph_node << endl;
#endif
            
            //dist_1_set is empty
            if(temp_set1.size()==0 && temp_set2.size()==0 && graph_node >graph_dist)
            {
                //cout << "dist_temp_set: " << dist_temp_set << endl;
                for(auto dist_temp : dist_temp_set)
                {
                    if(dist_temp.first == dist_temp_set[0].first)
                        temp_set1.push_back(dist_temp.second);
                    else if(dist_temp.first == dist_temp_set[0].first+1)
                        temp_set2.push_back(dist_temp.second);
                    else
                        continue;
                }
            }

            //mapping temp_set-->candi_loc
            if(temp_set1.size() < temp_set2.size() )
                swap(temp_set1, temp_set2);
#if DEBUG_G
            cout << endl;
            cout << "temp_set1: " << temp_set1 << endl;
            cout << "temp_set2: " << temp_set2 << endl;
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            int Q_location;
#if DEBUG_G
            cout << endl << " (3-1) temp_set -> candi_loc mapping " << endl;
            cout << "temp_set1.size: " << temp_set1.size() << ", candi_loc1.size: " << candi_loc1.size() << endl;
            cout << "temp_set2.size: " << temp_set2.size() << ", candi_loc2.size: " << candi_loc2.size() << endl;
#endif

            //for candi_loc1    //////////////////////
            if(candi_loc1.size() >= temp_set1.size())
            {
                int Q_index = 0;
                for(auto q : temp_set1)
                {
                    int Q_location = candi_loc1[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc1.erase(candi_loc1.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set1.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc1)
                {
                    int qq = temp_set1[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc1.clear();
            }
            
            //for candi_loc2    //////////////////////
            if(candi_loc2.size() >= temp_set2.size())
            {
                int Q_index=0;
                for(auto q : temp_set2)
                {
                    int Q_location = candi_loc2[Q_index];
                    qubit_Q[Q_location] = q;
                    layout_L[q] = Q_location;
                    candi_loc2.erase(candi_loc2.begin());
#if DEBUG_G
                    cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                }
                temp_set2.clear();
            }
            else
            {
                int q_index = 0;
                for(auto Q : candi_loc2)
                {
                    int qq = temp_set2[q_index];
                    qubit_Q[Q] = qq;
                    layout_L[qq] = Q;
                    temp_set2.erase(temp_set2.begin());
#if DEBUG_G
                    cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                }
                candi_loc2.clear();
            }




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //remained node??
            if(temp_set1.size() + temp_set2.size() >= 0 && Dgraph.nodeset.size()>=3500)
            {
                //candi & temp_set update!
                temp_set1.insert(temp_set1.end(), temp_set2.begin(), temp_set2.end());
                candi_loc1.insert(candi_loc1.end(), candi_loc2.begin(), candi_loc2.end());
#if DEBUG_G
                cout << endl << " (3-2) Remained node! " << endl;
                cout << "temp_set1: " << temp_set1 << endl;
                cout << "candi_loc1: " << candi_loc1 << endl;
#endif
                if(candi_loc1.size() >= temp_set1.size())
                {
                    int Q_index = 0;
                    for(auto q : temp_set1)
                    {
                        int Q_location = candi_loc1[Q_index];
                        qubit_Q[Q_location] = q;
                        layout_L[q] = Q_location;
                        
                        candi_loc1.erase( remove(candi_loc1.begin(), candi_loc1.end(), Q_location) );
#if DEBUG_G
                        cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                    }
                    temp_set1.clear();

                }
                else
                {
                    int q_index = 0;
                    for(auto Q : candi_loc1)
                    {
                        int qq = temp_set1[q_index];
                        qubit_Q[Q] = qq;
                        layout_L[qq] = Q;
                        temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                        cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                    }
                    candi_loc1.clear();
                }
            }
            
            queue<int> temp_bfs_queue2;

            for(auto v : temp_set1)
            {
                temp_bfs_queue2.push(v);
            }
            while(!igraph_bfs_queue.empty())
            {
                temp_bfs_queue2.push(igraph_bfs_queue.front());
                igraph_bfs_queue.pop();
            }
            igraph_bfs_queue = temp_bfs_queue2;
#if DEBUG_G
            cout << "-> remained igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif
        }
        // (3) ready to next loop
        first_loop = false;
        igraph_qc_queue.pop();
        dist_1_mapping = false;

    }




}

void Qcircuit::QMapper::graph_matching_processing(ARCHITECTURE archi)
{
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical
   

    //CNOT set
    Dgraph_cnot.nodeset.clear();
    //cout << " ** CNOT set ** " << endl;
    for(const auto& gate : Dgraph.nodeset)
    {
        if(gate.type != GATETYPE::CNOT) continue;
        const int target  = gate.target;
        const int control = gate.control;
        
        add_cnot(control, target, Dgraph_cnot);
        add_cnot_num--;
    }
    //cout << Dgraph_cnot.nodeset << endl;






    int n = Dgraph_cnot.nodeset.size()/param_beta;
    if (nqubits<=10) 
        n = Dgraph_cnot.nodeset.size();
    
    // Make interaction graph
    Graph interaction_graph_order         = make_interactionGraph(true);
    Graph interaction_graph_number         = make_interactionNumberGraph(true);
    interaction_subgraph_order        = make_interactionSubgraph_order(true, n);
    interaction_subgraph_number       = make_interactionSubgraph_num(true, n);

    interaction_graph_order.build_dist_table();
    interaction_graph_number.build_dist_table();
    interaction_subgraph_order.build_dist_table();
    interaction_subgraph_number.build_dist_table();

#if DEBUG_GRAPH
    int idx_order, idx_num, idx_sub_order, idx_sub_num;
    bool equal_order, equal_num, equal_sub_order, equal_sub_num;
   
    cout << endl <<"* Interaction Order Graph" << endl;
    pair<int, bool> graph_charact = interaction_graph_order.graph_characteristics(true);
    idx_order   = graph_charact.first;
    equal_order = graph_charact.second;
    cout << "=> idx_order: " << idx_order << ", equal_order: " << equal_order << endl;

    cout << endl << "* Interaction Number Graph" << endl;
    graph_charact = interaction_graph_number.graph_characteristics(false);
    idx_num   = graph_charact.first;
    equal_num = graph_charact.second;
    cout << "==> idx_number: " << idx_num << ", equal_num: " << equal_num << endl;
    
    cout << endl << "* Interaction Subgraph_order" << endl;
    graph_charact = interaction_subgraph_order.graph_characteristics(true);
    idx_sub_order   = graph_charact.first;
    equal_sub_order = graph_charact.second;
    cout << "==> idx_sub_order: " << idx_sub_order << ", equal_sub_order: " << equal_sub_order << endl;
    
    cout << endl << "* Interaction Subgraph_num" << endl;
    graph_charact = interaction_subgraph_number.graph_characteristics(false);  
    idx_sub_num   = graph_charact.first;
    equal_sub_num = graph_charact.second;
    cout << "==> idx_sub_number: " << idx_sub_num << ", equal_sub_num: " << equal_sub_num << endl;
    cout << endl;
#endif

#if DEBUG_GRAPH
    cout << endl << " *** GRAPH Matching Processing Start  ***" << endl;
#endif

    //Find center (using interaction_graph)
    
#if DEBUG_GRAPH
    //1. interaction order graph
    int q_in_order = interaction_graph_order.cal_graph_center();
    queue<int> igraph_order_queue;
    bfs_queue_gen(igraph_order_queue, interaction_graph_order, q_in_order, true);
    cout << "igraph_order_queue: " << igraph_order_queue << endl;
    
    //2. interaction number graph
    int q_in_number = interaction_graph_number.cal_graph_center();
    queue<int> igraph_number_queue;
    bfs_queue_gen(igraph_number_queue, interaction_graph_number, q_in_number, false);
    cout << "igraph_number_queue: " << igraph_number_queue << endl;
    
    //3. interaction sub_order graph
    int q_in_sub_order = interaction_subgraph_order.cal_graph_center();
    queue<int> igraph_sub_order_queue;
    bfs_queue_gen(igraph_sub_order_queue, interaction_subgraph_order, q_in_sub_order, true);
    cout << "igraph_sub_order_queue: " << igraph_sub_order_queue << endl;
    
    //4. interaction sub_number graph
    int q_in_sub_number = interaction_subgraph_number.cal_graph_center();
    queue<int> igraph_sub_number_queue;
    bfs_queue_gen(igraph_sub_number_queue, interaction_subgraph_number, q_in_sub_number, false);
    cout << "igraph_sub_number_queue: " << igraph_sub_number_queue << endl;
    
    cout << "------------------------------------------" << endl << endl;
#endif




    //int qc = interaction_number_graph.cal_graph_center();
    //int qc = interaction_order_graph.cal_graph_center();
    int qc = interaction_subgraph_order.cal_graph_center();
    //int qc = idx_sub_num;
    
    //int qc = interaction_subgraph_number.cal_graph_center();
    int Qc = coupling_graph.cal_graph_center();
    
    //bfs_queue
    queue<int> igraph_qc_queue, igraph_bfs_queue;
    int graph_dist, graph_node;
    //bfs_queue_gen(igraph_qc_queue, interaction_number_graph, qc, false);
    //bfs_queue_gen(igraph_qc_queue, interaction_order_graph, qc, true);
    //bfs_queue_gen(igraph_qc_queue, interaction_subgraph_order, qc, true);
    //bfs_queue_gen(igraph_qc_queue, interaction_subgraph_number, qc, false);
    
    bfs_queue_gen_2(igraph_qc_queue, interaction_subgraph_order, interaction_subgraph_number, qc);
    igraph_bfs_queue = igraph_qc_queue;
#if DEBUG_GRAPH
    cout << endl << "igraph_qc_queue: " << igraph_qc_queue << endl;
#endif
   
    





    vector<int> candi_loc1, candi_loc2;
    int location_degree;
    bool first_loop = true;

    //Sequentially logical->physical mapping
    while(!igraph_qc_queue.empty())
    {
        int current_qc = igraph_qc_queue.front();
        bool dist_1_mapping = false;
        cout << " ";
#if DEBUG_qc
        cout << endl << "---------------------   qc: "  << current_qc << "    -----------------------" << endl;
#endif        
        // (1) circuit center --> coupling graph center
        if(first_loop)
        {
#if DEBUG_G
            cout << " (1) circuit center -> physical center " << endl;
            cout << "qc: " << qc << endl;
            cout << "-> Qc: " << Qc << endl;
#endif
            qubit_Q[Qc] = qc;
            layout_L[qc] = Qc;

            dist_1_mapping = true;
        }
        else
        {
            //center mapping is not required
            if(layout_L[current_qc]!=-1)
            {
        cout << " ";
#if DEBUG_qc
                cout << " (1) circuit center -> physical center " << endl;
                cout << "Current_qc has a physical qubit!" << endl;
#endif
                dist_1_mapping = false;
            }
            //new center --> mapping
            else
            {
                //find physical qubit!!!!!!!!!!!!
                vector<int> ref_loc;
                make_ref_loc(ref_loc, interaction_graph_number, current_qc, false);
                
                sort_degree(ref_loc, coupling_graph);
                
                candi_loc1.clear();
                candi_loc2.clear();
                make_candi_loc_2(ref_loc[0], coupling_graph, candi_loc1, candi_loc2, location_degree);
                candi_loc1.insert(candi_loc1.end(), candi_loc2.begin(), candi_loc2.end());
                cout << " ";
#if DEBUG_qc
                cout << "(1) Find physical qubit for current qc" << endl;
                cout << "ref_loc: " << ref_loc << endl;
                cout << "candi_loc1: " << candi_loc1 << endl;
#endif
                
                //sort_degree(candi_loc1, coupling_graph);
                sort_distance(current_qc, candi_loc1, ref_loc, coupling_graph, interaction_graph_number);

                Qc = candi_loc1[0];
                qubit_Q[Qc] = current_qc;
                layout_L[current_qc] = Qc;

                dist_1_mapping = true;
#if DEBUG_G
                cout << "candi_loc1: " << candi_loc1 << endl;
                cout << "-> Qc: " << Qc << endl;
#endif                
            }
        }


        // (2) dist_1 set
        cout << " ";
#if DEBUG_qc
        cout    << " (2) candi_loc (physical) & temp_set (logical) update " << endl;
#endif
        if(dist_1_mapping)
        {
            //candi_location update (disjoint set) --> candi_loc1, candi_loc2
            candi_loc1.clear();
            candi_loc2.clear();
            make_candi_loc_2(layout_L[current_qc], coupling_graph, candi_loc1, candi_loc2, location_degree);
          
            //Location to center --> temp_set1, temp_set2
            vector<int> temp_set1, temp_set2;
            queue<int> temp_bfs_queue;
            bool first_bfs = true; 
            
#if DEBUG_G
            cout << "candi_loc1: " << candi_loc1 << endl;
            cout << "candi_loc2: " << candi_loc2 << endl;
            cout << "location_degree: " << location_degree << endl;
            cout << endl;

        /*
            //while(!igraph_bfs_queue.empty())
            //    igraph_bfs_queue.pop();
            //
            //bfs_queue_gen(igraph_bfs_queue, interaction_graph_number, current_qc, false);
            //cout << "<igraph_bfs_queue>: " <<  igraph_bfs_queue <<  endl;
        */
#endif            
            
            while(!igraph_bfs_queue.empty())
            {
                int current_bfs = igraph_bfs_queue.front();
#if DEBUG_G
                cout << "current_bfs: " << current_bfs;
#endif
                //current_qc = current_bfs -> pop()
                if(current_qc == current_bfs)
                {   
#if DEBUG_G
                    cout << "\t-> skip (current_qc)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //has physical qubit
                else if(layout_L[current_bfs]!=-1)
                {   
#if DEBUG_G
                    cout << "\t-> skip (has physical qubit)" << endl;
#endif
                    igraph_bfs_queue.pop();
                }
                //not dist_1
                else if(interaction_graph_number.dist[current_qc][igraph_bfs_queue.front()] != 1)
                {
#if DEBUG_G
                    cout << "\t-> skip (not dist_1)" << endl;
#endif
                    temp_bfs_queue.push(current_bfs);
                    igraph_bfs_queue.pop();
                }
                //only dist_1_set
                else
                {
                    if(!first_bfs)
                    {
                        if(interaction_graph_number.dist[temp_set1[0]][current_bfs] == 1)
                        {
#if DEBUG_G
                            cout << "\t-> temp_set1" << endl;
#endif
                            temp_set1.push_back(current_bfs);
                        }
                        else
                        {
#if DEBUG_G
                            cout << "\t-> temp_set2" << endl;
#endif
                            temp_set2.push_back(current_bfs);
                        }
                        igraph_bfs_queue.pop();
                    }
                    else
                    {
#if DEBUG_G
                        cout << "\t-> temp_set1" << endl;
#endif
                        temp_set1.push_back(igraph_bfs_queue.front());
                        igraph_bfs_queue.pop();

                        first_bfs = false;
                    }
                }
            }
            
            igraph_bfs_queue = temp_bfs_queue;


            //mapping temp_set-->candi_loc
            if(temp_set1.size() < temp_set2.size())
                swap(temp_set1, temp_set2);

            //sort candi_loc
            sort_degree(candi_loc1, coupling_graph);
            sort_degree(candi_loc2, coupling_graph);
#if DEBUG_G
            cout << endl;
            cout << "temp_set1: " << temp_set1 << endl;
            cout << "temp_set2: " << temp_set2 << endl;
            cout << "-> candi_loc1: " << candi_loc1 << endl;
            cout << "-> candi_loc2: " << candi_loc2 << endl;
#endif



            //graph_dist = interaction_graph.dist[igraph_bfs_queue.front()][igraph_bfs_queue.back()];
            graph_dist = interaction_graph_order.dist[current_qc][igraph_bfs_queue.back()];
            graph_node = temp_set1.size() + temp_set2.size();
#if DEBUG_G
            cout << " ## Graph shape? " << endl;
            cout << "graph_dist: " << graph_dist << endl;
            cout << "graph_node: " << graph_node << endl;
#endif






            int Q_location;
            //////////////////////////////////////////////////////////////////////////////////////////////////
#if DEBUG_G
            cout << endl << " (3-1) temp_set -> candi_loc mapping " << endl;
            cout << "temp_set1.size: " << temp_set1.size() << ", candi_loc1.size: " << candi_loc1.size() << endl;
            cout << "temp_set2.size: " << temp_set2.size() << ", candi_loc2.size: " << candi_loc2.size() << endl;
#endif



            ////////// graph shape?????????????
//            if(graph_dist > graph_node)
//            {
//                //line shape
//#if DEBUG_G
//                cout << "**** line shape! ****" << endl;
//#endif
//            }
//            else
            if(graph_dist <= graph_node)
            {
                //circle shape
#if DEBUG_G
                cout << "**** circle shape! ****" << endl;
#endif
                //for candi_loc1    //////////////////////
                if(candi_loc1.size() >= temp_set1.size())
                {
                    int Q_index = 0;
                    for(auto q : temp_set1)
                    {
                        int Q_location = candi_loc1[Q_index];
                        qubit_Q[Q_location] = q;
                        layout_L[q] = Q_location;
                        
                        candi_loc1.erase(candi_loc1.begin());
#if DEBUG_G
                        cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                    }
                    temp_set1.clear();
                }
                else
                {
                    int q_index = 0;
                    for(auto Q : candi_loc1)
                    {
                        int qq = temp_set1[q_index];
                        qubit_Q[Q] = qq;
                        layout_L[qq] = Q;
                        //q_index++;
                        temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                        cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                    }
                    candi_loc1.clear();
                }
                
                //for candi_loc2    //////////////////////
                if(candi_loc2.size() >= temp_set2.size())
                {
                    int Q_index=0;
                    for(auto q : temp_set2)
                    {
                        int Q_location = candi_loc2[Q_index];
                        qubit_Q[Q_location] = q;
                        layout_L[q] = Q_location;
                        
                        candi_loc2.erase(candi_loc2.begin());
#if DEBUG_G
                        cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                    }
                    temp_set2.clear();
                }
                else
                {
                    int q_index = 0;
                    for(auto Q : candi_loc2)
                    {
                        int qq = temp_set2[q_index];
                        qubit_Q[Q] = qq;
                        layout_L[qq] = Q;
                        //q_index++;
                        temp_set2.erase(temp_set2.begin());
#if DEBUG_G
                        cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                    }
                    candi_loc2.clear();
                }

            }



            //
//            if(candi_loc1.size() + candi_loc2.size() != 0)
//            {

                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //if temp_set1 + temp_set2 >= 0
                if(temp_set1.size() + temp_set2.size() >= 0)
                {
                    //candi & temp_set update!
                    temp_set1.insert(temp_set1.end(), temp_set2.begin(), temp_set2.end());
                    candi_loc1.insert(candi_loc1.end(), candi_loc2.begin(), candi_loc2.end());
#if DEBUG_G
                    cout << endl << " (3-2) Remained node! " << endl;
                    cout << "temp_set1: " << temp_set1 << endl;
                    cout << "candi_loc1: " << candi_loc1 << endl;
#endif
                    if(candi_loc1.size() >= temp_set1.size())
                    {
                        int Q_index = 0;
                        for(auto q : temp_set1)
                        {
                            int Q_location = candi_loc1[Q_index];
                            qubit_Q[Q_location] = q;
                            layout_L[q] = Q_location;
                            
                            candi_loc1.erase( remove(candi_loc1.begin(), candi_loc1.end(), Q_location) );
#if DEBUG_G
                            cout << "--> q: " << q << ", Q: " << Q_location << endl;
#endif
                        }
                        temp_set1.clear();

                    }
                    else
                    {
                        int q_index = 0;
                        for(auto Q : candi_loc1)
                        {
                            int qq = temp_set1[q_index];
                            qubit_Q[Q] = qq;
                            layout_L[qq] = Q;
                            //q_index++;
                            temp_set1.erase(temp_set1.begin());
#if DEBUG_G
                            cout << "--> qq: " << qq << ", Q: " << Q << endl;
#endif
                        }
                        candi_loc1.clear();
                    }
                }
                
                //////////////// remained temp_set
                queue<int> temp_bfs_queue2;

                for(auto v : temp_set1)
                {
                    temp_bfs_queue2.push(v);
                }
                while(!igraph_bfs_queue.empty())
                {
                    temp_bfs_queue2.push(igraph_bfs_queue.front());
                    igraph_bfs_queue.pop();
                }
                igraph_bfs_queue = temp_bfs_queue2;
#if DEBUG_G
                cout << "-> remained igraph_bfs_queue: " << igraph_bfs_queue << endl;
#endif


                
//            }
//            //////////////////////////////////////////////////////////////////////////////////////////////////////
//            else
//            {
//#if DEBUG_G
//                cout << "** candi_loc update is required!" << endl;
//#endif
//            }
            
            

        }
        
        // (3) ready to next loop
        first_loop = false;
        igraph_qc_queue.pop();
        dist_1_mapping = false;

    }

}


void Qcircuit::QMapper::pre_processing(ARCHITECTURE archi)
{
    //initiallize layout -> -1
    for(int i=0; i<nqubits; i++)
        layout_L[i] = -1; //logical -> physical
    for(int i=0; i<positions; i++)
        qubit_Q[i] = -1;  //physical -> logical
   
    make_CNOT(true);

    Graph interaction_graph = make_interactionGraph(true);
    cout << interaction_graph << endl;
    
    
    interaction_graph.build_dist_table();
    interaction_graph.print_dist_table();
    
    int qc = interaction_graph.cal_graph_center();
    int Qc = coupling_graph.cal_graph_center();
    cout << qc << endl;
    cout << Qc << endl;
    
    //interaction graph bfs queue
    queue<int> igraph_bfs_queue;
    int graph_dist;
    bfs_queue_gen(igraph_bfs_queue, interaction_graph, qc, true);
    igraph_bfs_queue.pop();

    //mapping update
    layout_L[qc] = Qc;
    qubit_Q[Qc] = qc;

    int prev_p = qc;

    while(!igraph_bfs_queue.empty())
    {
        int p = igraph_bfs_queue.front();
        igraph_bfs_queue.pop();

        vector<int> ref_loc;
        vector<int> candi_loc;
        make_ref_loc(ref_loc, interaction_graph, p, true);
        cout << "ref_loc: " << ref_loc << endl;
        if(!ref_loc.empty())
        make_candi_loc(candi_loc, ref_loc, coupling_graph);
   
#if DEGREE_CHANGE_v1
        //bool relation_p_prev = false;
        if(interaction_graph.dist[prev_p][p]!=1) //not relationship between p and prev_p
        {
            cout << "not have relationship between p and prev_p" << endl;
            std::reverse(candi_loc.begin(), candi_loc.end());
        }
        prev_p = p;
#endif

        int Q = candi_loc[0];
        //int Q = return_minimum_degree1();
#if DEGREE_CHANGE_v2
        if(candi_loc.size()>1)
        {
            int temp_dist = abs(ref_loc[0] - candi_loc[0]);
            for(auto candi_Q : candi_loc)
            {
                int dist = abs(ref_loc[0] - candi_Q);
                if(temp_dist >= dist)
                {
                    temp_dist = dist;
                    Q = candi_Q;
                }
            }
        }
#endif
#if DEGREE_CHANGE_v3
        if(candi_loc.size()>1)
        {
            int temp_dist = abs(p - candi_loc[0]);
            for(auto candi_Q : candi_loc)
            {
                int dist = abs(p - candi_Q);
                if(temp_dist >= dist)
                {
                    temp_dist = dist;
                    Q = candi_Q;
                }
            }
        }
#endif
        //mapping update(q->Q)
        layout_L[p] = Q;
        qubit_Q[Q] = p;

    }

    int idx = nqubits;
    for(auto& kv : qubit_Q)
    {
        if(kv.second == -1)
        {
            layout_L[idx] = kv.first;
            kv.second = idx++;
        }
    }
}


void Qcircuit::QMapper::print_layout(const map<int, int> &layout)
{
    cout.setf(ios::left);
    for(auto& it : layout)
        cout << " q" << setw(2) << it.first << "\t->    Q" << setw(2) << it.second << endl;
        //cout << " q" << setw(2) << it.first << " is mapped to Q" << setw(2) << it.second << endl;
    cout << endl;
}

void Qcircuit::QMapper::print_qubit(const map<int, int> &qubit)
{
    cout.setf(ios::left);
    for(auto& it : qubit)
        cout << " Q" << setw(2) << it.first << "\t<-    q" << setw(2) << it.second << endl; 
        //cout << " Q" << setw(2) << it.first << " has logical qubit q" << setw(2) << it.second << endl; 
    cout << endl;
}

void Qcircuit::QMapper::print_layout()
{
    const map<int, int> &layout = layout_L;
    cout.setf(ios::left);
    for(auto& it : layout)
        cout << " q" << setw(2) << it.first << "\t->    Q" << setw(2) << it.second << endl;
        //cout << " q" << setw(2) << it.first << " is mapped to Q" << setw(2) << it.second << endl;
    cout << endl;
}

void Qcircuit::QMapper::print_qubit()
{
    const map<int, int> &qubit = qubit_Q;
    cout.setf(ios::left);
    for(auto& it : qubit)
        cout << " Q" << setw(2) << it.first << "\t<-    q" << setw(2) << it.second << endl; 
        //cout << " Q" << setw(2) << it.first << " has logical qubit q" << setw(2) << it.second << endl; 
    cout << endl;
}

