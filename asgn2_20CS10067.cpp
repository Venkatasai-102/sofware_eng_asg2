// NAME: VENKATA SAI SUVVARI
// ROLL: 20CS10067

/*
    1. The user searches for the name of a places instead of user-name in the first case.
    2. Every map will start with root tag "osm" and their attributes will be same as given sample map.
    3. The user searches only for the name of nodes in the first case not for the names of ways.
*/

#include <bits/stdc++.h>
#include "rapidxml.hpp" // the header for traversing the xml file

using namespace std;
using namespace rapidxml;

typedef long long ll;

// for parsing the osm/xml file
xml_document<> doc; // to point to the document
xml_node<> *root_node = NULL; // to point to the start of the xml root node

// Function for converting degrees to radians
inline long double toRadians(const long double degree){return (((long double)((M_PI) / 180)) * degree);}

// to convert "char *" to string
string ctos(char *str)
{
    string ans = "";
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        ans += str[i];
    }

    return ans;
}

// returns a string with all letters in lowercase
string to_small(string str)
{
    int n = str.size();

    string ans = "";

    for (int i = 0; i < n; i++)
    {
        if ('A' <= str[i] && 'Z' >= str[i])
        {
            ans += str[i] - 'A' + 'a';
        }
        else
        {
            ans += str[i];
        }
    }
    return ans;
}

// checks whether name1 is substring of name2
bool is_substring(string name1, string name2)
{
    // converting both the string to the lower case to check for the case-insensitive
    string name = to_small(name1);
    string or_name = to_small(name2);

    int len_ss = name.size();
    int len_nm = or_name.size();
    
    if (len_nm < len_ss)
    {
        return false;
    }
    else
    {
        for (int i = 0; i <= len_nm - len_ss; i++)
        {
            if (or_name.substr(i, len_ss) == name)
            {
                return true;
            }
        }
        return false;
    }
}

// for comparing 2 pair elements t1 and t2, which is later used in the sort function as function pointer
bool comp_pair(pair<ll, long double> t1, pair<ll, long double> t2)
{
    return (t1.second < t2.second);
}

class node
{
    public:
    ll id, uid;
    long double lat, lon;
    string usr_nm, plc_nm;

    node()
    {
        this->id = 0;
        this->lat = this->lon = 0.0;
        this->usr_nm = "name";
        this->plc_nm = "name";
    }
    node(ll id, ll uid, double lt, double ln, string nm)
    {
        this->id = id;
        this->uid = uid;
        this->lat = lt;
        this->lon = ln;
        this->usr_nm = nm;
        this->plc_nm = "name";
    }

    // finds distance from one node to other using the geo-coordinates
    long double distance_from_othernode(node other)
    {
        long double lat1 = toRadians(this->lat);
        long double long1 = toRadians(this->lon);
        long double lat2 = toRadians(other.lat);
        long double long2 = toRadians(other.lon);
        
        // Haversine Formula
        long double diff_lon = long2 - long1, diff_lat = lat2 - lat1;
        long double ans = pow(sin(diff_lat / 2), 2) + cos(lat1)*cos(lat2)*pow(sin(diff_lon / 2), 2);
        ans = 2*asin(sqrt(ans));

        long double R = 6371.0; // the radius of earth

        return (ans * R);
    }

    // returns an array of distances from given node to all other nodes
    vector<pair<ll, long double>> distance_from_allnodes(map<ll, node> list)
    {
        vector<pair<ll, long double>> one_node_dist;
        int n = list.size();
        for(auto itr : list)
        {
            if (this->id == itr.first)
            {
                continue;
            }

            pair<ll, long double> temp;
            temp.first = itr.first;
            temp.second = this->distance_from_othernode(itr.second);
            one_node_dist.push_back(temp);
        }

        sort(one_node_dist.begin(), one_node_dist.end(), comp_pair);
        return one_node_dist;
    }
};

class way
{
    public:
    ll id, uid;
    string usr_nm;
    vector<node> nodes_in_way;

    way()
    {
        this->id = 0;
        this->uid = 0;
        this->usr_nm = "name";
    }
    way(ll id, ll uid, string name)
    {
        this->id = id;
        this->uid = uid;
        this->usr_nm = name;
    }
};

class edge
{
    public:
    ll id1, id2;
    long double cost;
    edge(ll id1, ll id2, long double cst)
    {
        this->id1 = id1;
        this->id2 = id2;
        this->cost = cst;
    }
};

class graph
{
    public:
    vector<edge> list_edge;

    // Forms graph from the list of ways
    graph form_graph(vector<way> &list_way)
    {
        graph g;
        node temp_nd1, temp_nd2;
        for (auto itr : list_way)
        {
            int len = itr.nodes_in_way.size();
            int prev = 0;
            for (int i = 1; i < len; i++)
            {
                temp_nd1 = itr.nodes_in_way[prev];
                temp_nd2 = itr.nodes_in_way[i];
                edge e1 = edge(temp_nd1.id, temp_nd2.id, temp_nd1.distance_from_othernode(temp_nd2));
                edge e2 = edge(temp_nd2.id, temp_nd1.id, temp_nd1.distance_from_othernode(temp_nd2));
                prev++;

                // since this is an undirected graph, push the edges which are opposite to each other in terms of ends
                g.list_edge.push_back(e1);
                g.list_edge.push_back(e2);
            }
        }
        return g;
    }

    // This prints the neighbouring nodes of each node in the graph (*** Beware this is too large ***)
    void print_graph()
    {
        int len = this->list_edge.size();
        vector<bool> visited(len, false);
        for (int i = 0; i < len; i++)
        {
            if (visited[i] == true)
            {
                continue;
            }
            
            visited[i] = true;
            ll id = this->list_edge[i].id1;
            cout << id << " -> " << this->list_edge[i].id2 << ", ";

            for (int j = i+1; j < len; j++)
            {
                if (this->list_edge[j].id1 == id)
                {
                    cout << this->list_edge[j].id2 << ", ";
                    visited[j] = true;
                }
            }
            cout << "\n";
        }
    }

    // gives the shortest path for 2 given nodes using dijkstra algorithm variation called "Uniform-cost search"
    long double traverse_graph(graph g, map<ll, node> &list_nodes, ll src_id, ll dst_id)
    {
        map<ll, bool> visited;
        for (auto itr : list_nodes)
        {
            visited.insert(pair<ll, bool> (itr.first, false));
        }
        
        priority_queue<pair<long double, ll>> pr_que;
        pr_que.push(make_pair(0.0, src_id));

        long double dist = (INT_MAX)*1.0;
        while (pr_que.size() > 0)
        {
            pair<long double, ll> temp = pr_que.top();
            temp.first *= -1;
            pr_que.pop();
            
            if (dst_id == temp.second)
            {
                if (dist > temp.first)
                {
                    dist = temp.first;
                }
                visited.clear();
                return dist;
            }
            
            if (!visited[temp.second])
            {
                for (auto itr : g.list_edge)
                {
                    if (itr.id1 == temp.second)
                    {
                        long double nw_cost = temp.first + itr.cost;
                        pr_que.push(make_pair(nw_cost*(-1), itr.id2));
                    }
                }
            }
            visited[temp.second] = true;
        }
        
        visited.clear();
        return dist;
    }
};

int main()
{
    // Read the map.osm file
    ifstream input("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(input)), istreambuf_iterator<char>()); // putting all into the buffer
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);

    // Find out the root node
    root_node = doc.first_node("osm");

    int count_node = 0,  count_ways = 0;

    // lists to store the nodes, nodes with name tags and list of ways in the map.osm file
    map<ll, node> list_nodes;
    vector<node> list_nd_with_name;
    vector<way> list_ways;

    // Iterate over the nodes
    xml_node<> *way_strt = root_node->first_node("way"); // to start way and stop node
    xml_node<> *way_end = root_node->first_node("relation"); // to stop way

    for (xml_node<> *parse_node = root_node->first_node("node"); parse_node != way_strt; parse_node = parse_node->next_sibling())
    {
        count_node ++; // counting the number of nodes

        node temp_nd;
        // initiating with given attributes
        temp_nd = node(stoll(parse_node->first_attribute("id")->value()), stoll(parse_node->first_attribute("uid")->value()), stod(parse_node->first_attribute("lat")->value()), stod(parse_node->first_attribute("lon")->value()), ctos(parse_node->first_attribute("user")->value()));

        bool is_nm = false;
        string pl_name;

        for (xml_node<> *parse_tag = parse_node->first_node("tag"); parse_tag; parse_tag = parse_tag->next_sibling()) // parsing for the name of the node if present
        {
            if (ctos(parse_tag->first_attribute("k")->value()) == "name")
            {
                pl_name = ctos(parse_tag->first_attribute("v")->value());
                is_nm = true;
                break;
            }
        }
        
        temp_nd.plc_nm = pl_name;

        if (is_nm)
        {
            list_nd_with_name.push_back(temp_nd); // if the node contains the name tags then adding it to the list2 also
        }
        
        list_nodes.insert(pair<ll, node>(temp_nd.id, temp_nd));
    }

    cout << "Number of nodes present in the map are: " << count_node << "\n";
    // Iterate over the way
    for (xml_node<> *parse_way = way_strt; parse_way != way_end; parse_way = parse_way->next_sibling())
    {
        count_ways ++;
        way temp_way;
        temp_way = way(stoll(parse_way->first_attribute("id")->value()), stoll(parse_way->first_attribute("uid")->value()), ctos(parse_way->first_attribute("user")->value()));
        
        // parsing and storing the nodes in the way in the respective object
        for (xml_node<> *parse_nd = parse_way->first_node("nd"); parse_nd != parse_way->first_node("tag"); parse_nd = parse_nd->next_sibling())
        {
            temp_way.nodes_in_way.push_back(list_nodes.at(stoll(parse_nd->first_attribute("ref")->value())));
        }
        list_ways.push_back(temp_way);
    }
    
    cout << "Number of ways present in the map are: " << count_ways << "\n";

    node temp_node;

    graph g = g.form_graph(list_ways);

    int choice;
    while (true)
    {
        cout << "\nEnter 1 to search the id of a place by its name.\n";
        cout << "Enter 2 to find the k-closest nodes of a particular node.\n";
        cout << "Enter 3 to see the the length of the shortest path between 2 particular nodes.\n";
        cout << "Enter 0 to exit the loop.\n";
        cin >> choice;
        getchar(); // reading the "\n" character
        if (choice == 0)
        {
            break;
        }
        
        switch (choice)
        {
        case 1:
        {
            string plc_name;
            cout << "\nEnter full name or substring of name of place to find the id of that place: ";
            getline(cin, plc_name);
            
            bool is_present = false;
            for (auto itr : list_nd_with_name) // checking the list with nodes if the name matches with any
            {
                if (is_substring(plc_name, itr.plc_nm))
                {
                    cout << "The id of the place with the name " << itr.plc_nm << " is: " << itr.id << "\nlatitude: " << itr.lat << "\nlongitude: " << itr.lon << "\n";
                    is_present = true;
                }
            }
            
            if (!is_present)
            {
                cout << "There is no node with the entered name as substring!!\n";
            }
        }
            break;
        case 2:
        {
            ll id;
            int k;
            cout << "Enter the id of node from which you want to find distance of k-closest nodes: ";
            cin >> id;
            cout << "Enter the value of k: ";
            cin >> k;
            
            node temp_nd;
            bool is_prsnt = false;
            for(auto itr : list_nodes) // checking if the node with enetred id is present and if present, assigning the node to a temporary node for furthur use
            {
                if (itr.first == id)
                {
                    temp_nd = itr.second;
                    is_prsnt = true;
                    break;
                }
            }

            if (!is_prsnt)
            {
                cout << "\nThe node with entered id is not present in the map.\n\n";
                continue;
            }
            
            vector<pair<ll, long double>> dist_arr = temp_nd.distance_from_allnodes(list_nodes);
            cout << "The first " << k << " closest nodes with id " << id << " are:\n";
            for (int i = 0; i < k; i++)
            {
                cout << dist_arr[i].first << " " << dist_arr[i].second << "\n";
            }

            // removing allocated memory
            dist_arr.clear();
        }
            break;
        case 3:
        {
            cout << "\nDo you know the id of the start and destination?\nEnter:\n0 if you don't know and then enter 1 and type the name to find id's, and\n1 if you know\n";
            int ch;
            cin >> ch;
            if (!ch)
            {
                continue;
            }
            
            cout << "\nEnter the id of the start: ";
            ll id1, id2;
            cin >> id1;
            cout << "\nEnter the id of the destination: ";
            cin >> id2;
            bool is_id1_p = false, is_id2_p = false;

            for (auto itr : list_nodes) // cjecking if the nodes with the entered id's are actually present
            {
                if (itr.first == id1)
                {
                    is_id1_p = true;
                }
                if (itr.first == id2)
                {
                    is_id2_p = true;
                }
                if (is_id2_p && is_id1_p)
                {
                    break;
                }
            }
            
            if (!(is_id2_p && is_id1_p))
            {
                cout << "\nOne of the id's you have entered is not correct!!\nCheck and enter again\n";
                continue;
            }
            
            long double lst_dist = g.traverse_graph(g, list_nodes, id1, id2);
            ll temp = (ll)lst_dist;

            if (temp == INT_MAX)
            {
                cout << "There is no possible way between these nodes!!\n";
                continue;
            }
            
            cout << "\nThe least possible distance is: " << lst_dist << " Km\n";
        }
            break;
        default:
            cout << "Invalid Input!!\n";
            break;
        }
    }
    
    // clearing memory allocated
    list_nd_with_name.clear();
    list_nodes.clear();
    g.list_edge.clear();
    list_ways.clear();

    cout << "\n\t\t\t******* You have done *******\n\n";

    return 0;
}