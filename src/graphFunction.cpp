//graphFunction.cpp
#include "circuit.h"

#define PRINT_graph_function 0

using namespace std;
using namespace Qcircuit;

void Qcircuit::Graph::build_degree_table()
{
    int idx = 0;
    for(auto& kv : nodeset)
        nodeid[idx++] = kv.first;
    node_size = nodeset.size();

    degree = new int*[node_size];
    for(int i = 0; i < node_size; i++)
        degree[i] = new int[node_size];
    
    //initialize
    for(int i = 0; i < node_size; i++)
        for(int j = 0; j < node_size; j++)
            degree[i][j] = 0;

    //set value
    for(int i = 0; i < node_size; i++)
        for(int j = i+1; j < node_size; j++)
            if(dist[i][j]==1)
            {
                degree[i][j] = degree[i][j] + 1;
                degree[j][i] = degree[i][j];
            }
            else continue;
}

void Qcircuit::Graph::print_degree_table()
{
    cout << endl << " # Degree table " << endl;
    cout << "i↓  j→";
    for(int i=0; i<node_size; i++)
        cout << setw(4) << i;
    cout << endl;
    for(int i=0; i<node_size; i++)
    {
        cout << setw(6) << i;
        for(int j=0; j<node_size; j++)
        {
            cout << setw(4) << degree[i][j];
        }
        cout << endl;
    }
    cout << endl;            
}

void Qcircuit::Graph::delete_degree_table()
{
    for(int i=0; i<node_size; i++)
        delete[] degree[i];
    delete[] degree;
}


void Qcircuit::Graph::build_degree_n()
{
    degreeN = new int[node_size];
    for(int i=0; i<node_size; i++)
        degreeN[i] = 0;

    for(int i=0; i<node_size; i++)
        for(int j=0; j<node_size; j++)
            if(degree[i][j]==1)
                degreeN[i]++;
}

void Qcircuit::Graph::build_doubledegree_n()
{
    doubleDegreeN = new int[node_size];
    for(int i=0; i<node_size; i++)
        doubleDegreeN[i] = 0;
    
    for(int i=0; i<node_size; i++)
        for(int j=0; j<node_size; j++)
            if(degree[i][j]==1)
                doubleDegreeN[i] += degreeN[j];
}

void Qcircuit::Graph::remove_degree_n(int n)
{
    for(int i=0; i<node_size; i++)
        if(degree[n][i]==1)
        {
            degree[n][i]=0;
            doubleDegreeN[i] -= degreeN[n];
            degreeN[i]--;
        }
    degreeN[n]=0;
    doubleDegreeN[n]=0;
}



/////////////////////////////////
void Qcircuit::Graph::build_dist_table()
{
    int idx = 0;
    for(auto& kv : nodeset)
        nodeid[idx++] = kv.first;
    node_size = nodeset.size();

    dist = new int*[node_size];
    for(int i = 0; i < node_size; i++)
        dist[i] = new int[node_size];

    for(int i = 0; i < node_size; i++)
        for(int j = 0; j < node_size; j++)
            if(i < j)
                dist[i][j] = bfs(i, j);
            else if(i == j)
                dist[i][j] = 0;
            else
                dist[i][j] = dist[j][i];
}

void Qcircuit::Graph::print_dist_table()
{
    cout << endl << " # Distance table " << endl;
    cout << "i↓  j→";
    for(int i=0; i<node_size; i++)
        cout << setw(4) << i;
    cout << endl;
    for(int i=0; i<node_size; i++)
    {
        cout << setw(6) << i;
        for(int j=0; j<node_size; j++)
        {
            cout << setw(4) << dist[i][j];
        }
        cout << endl;
    }
    cout << endl;            
}

void Qcircuit::Graph::delete_dist_table()
{
    for(int i=0; i<node_size; i++)
        delete[] dist[i];
    delete[] dist;
}

int Qcircuit::Graph::bfs(int start, int end)
{
    int return_val = -1;
    
    // table index
    vector<bool> isvisited(node_size, false);
    vector<int>  dist(node_size, 0);
    queue<int> queue;
    queue.push(start);
    isvisited[start] = true;

    while(!queue.empty())
    {
        int table_idx = queue.front();
        queue.pop();
        
        if(table_idx == end && isvisited[table_idx])
        {
            return_val = dist[table_idx];
            break;
        }

        //find adjacent node of table_idx node
        int g_id = nodeid[table_idx];
        Node& n = nodeset[g_id];
        for(auto& i : n.edges)
        {
            Edge& e = edgeset[i];
            int s = e.source->getid();
            int t = e.target->getid();
            int id = (s == g_id) ? t : s;         
            
            //adjacent node id (table_idx)
            int t_id = nodeid.find(id)->first;
            
            if(isvisited[t_id]) continue;

            isvisited[t_id] = true;
            dist[t_id] = dist[table_idx] + 1;
            queue.push(t_id);
        }
    }
    return return_val;
}

int Qcircuit::Graph::bfs_weight(int start, int end)
{
    int return_val = -1;
    
    // table index
    vector<bool> isvisited(node_size, false);
    vector<int>  dist(node_size, 0);
    vector<int>  weight(node_size, 0);
    queue<int> queue;
    queue.push(start);
    isvisited[start] = true;

    while(!queue.empty())
    {
        int table_idx = queue.front();
        queue.pop();
        
        if(table_idx == end && isvisited[table_idx])
        {
            return_val = dist[table_idx];
            return_val = weight[table_idx];
            break;
        }

        //find adjacent node of table_idx node
        int g_id = nodeid[table_idx];
        Node& n = nodeset[g_id];
        for(auto& i : n.edges)
        {
            Edge& e = edgeset[i];
            int s = e.source->getid();
            int t = e.target->getid();
            int w = e.getweight();
            int id = (s == g_id) ? t : s;         
            
            //adjacent node id (table_idx)
            int t_id = nodeid.find(id)->first;
            
            if(isvisited[t_id])
            {
                if(dist[t_id] < dist[table_idx] + 1) continue;
                if(weight[t_id] < weight[table_idx] + w) continue;
                dist[t_id] = dist[table_idx] + 1;
                weight[t_id] = weight[table_idx] + w;
                continue;
            }

            isvisited[t_id] = true;
            dist[t_id] = dist[table_idx] + 1;
            weight[t_id] = weight[table_idx] + w;
            queue.push(t_id);
        }
    }
    return return_val;
}

int Qcircuit::Graph::cal_graph_center()
{
#if PRINT_graph_function
    cout << " # calculating center" << endl;
#endif
    int min_value = INT_MAX;
    int min_index = -1;
    for(int i = 0; i < node_size; i++)
    {
        int sum = 0;
        for(int j = 0; j < node_size; j++)
        {
            sum += dist[i][j];
        }
        if(min_value > sum)
        {   
            min_value = sum;
            min_index = i;
        }
    }
#if PRINT_graph_function
    cout << " the center is " << endl;
    cout << nodeid[min_index] << endl;
#endif

    return nodeid[min_index];
}


////////////////////////
bool compare_center(pair<int, int> a, pair<int, int> b)
{
    if(a.first == b.first)
        return a.second < b.second;
    else
        return a.first < b.first;
}
bool compare_center_d(pair<int, int> a, pair<int, int> b)
{
    if(a.first == b.first)
        return a.second < b.second;
    else
        return a.first > b.first;
}

int Qcircuit::Graph::cal_graph_center_weight()
{
#if PRINT_graph_function
    cout << " # calculating center" << endl;
#endif
    int min_value = INT_MAX;
    int min_index = -1;
    vector<pair<int, int>> minValueVec;
    vector<pair<int, int>> minWeightVec;
    
    int nqubits = nodeset.size();
    vector<int> edge_weight(nqubits, 0);
    for(auto edge : edgeset)
    {
        const int weight    = edge.second.getweight();
        const int sourceid  = edge.second.getsourceid();
        const int targetid  = edge.second.gettargetid();
        edge_weight[sourceid] += weight;
        edge_weight[targetid] += weight;
    }
    for(int i = 0; i < node_size; i++)
    {
        int sum = 0;
        for(int j = 0; j < node_size; j++)
        {
            sum += dist[i][j];
        }
        minValueVec.push_back(make_pair(sum, i));
    }
    sort(minValueVec.begin(), minValueVec.end(), compare_center);
    for(auto i : minValueVec)
    {
        //cout << "sum: " << i.first << ", index: " << i.second << endl;
        if(i.first == minValueVec[0].first)
        {
            int iWeight = edge_weight[i.second];
            minWeightVec.push_back(make_pair(iWeight, i.second));
        }
    }
    sort(minWeightVec.begin(), minWeightVec.end(), compare_center_d);
    //for(auto i : minWeightVec)
    //    cout << "sum: " << i.first << ", index: " << i.second << endl;

    //return nodeid[min_index];
    return minWeightVec[0].second;

}




////////////////////////

bool vector_equal(const vector<int>& v)
{
        const auto& first = v[0];
            for (const auto& c : v)
                        if (first != c)
                                        return false;
                return true;
}

pair<int, bool> Qcircuit::Graph::graph_characteristics(bool i)
{
    int nqubits = nodeset.size();
    int idx_max, idx_min;

    vector<int> edge_weight(nqubits, 0);
    vector<int> edge_degree(nqubits, 0);
    for(auto edge : edgeset)
    {
        const int weight    = edge.second.getweight();
        const int sourceid  = edge.second.getsourceid();
        const int targetid  = edge.second.gettargetid();

        edge_weight[sourceid] += weight;
        edge_weight[targetid] += weight;
        
        edge_degree[sourceid] += 1;
        edge_degree[targetid] += 1;
    }
#if PRINT_graph_function
    cout << "|| # Node ||" << endl;
    cout << nodeset.size() << endl;
    
    cout << "|| # Edge ||" << endl;
    cout << edgeset.size() << endl;
    
    cout << "|| Degree ||" << endl;
    cout << edge_degree << endl;
    
    cout << "|| Weight sum ||" << endl;
    cout << edge_weight << endl;
#endif

    bool bool_vector_equal = vector_equal(edge_weight);

    idx_max = max_element(edge_weight.begin(), edge_weight.end()) - edge_weight.begin();
    idx_min = min_element(edge_weight.begin(), edge_weight.end()) - edge_weight.begin();
    
    int idx = i==true ? idx_min : idx_max;
    return make_pair(idx, bool_vector_equal);
}


