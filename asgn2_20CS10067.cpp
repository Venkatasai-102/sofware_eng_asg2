// NAME: VENKATA SAI SUVVARI
// ROLL: 20CS10067

#include <bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

typedef long long ll;

xml_document<> doc;
xml_node<> *root_node = NULL;

// Utility function for
// converting degrees to radians
inline long double toRadians(const long double degree){return (((long double)((M_PI) / 180)) * degree);}

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

bool is_substring(string name, string or_name)
{
    int len_ss = name.size();
    int len_nm = or_name.size();

    if (len_nm < len_ss)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < len_nm - len_ss; i++)
        {
            if (or_name.substr(i, len_ss) == name)
            {
                return true;
            }
        }
        return false;
    }
}

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

    long double distance_from_othernode(node other)
    {
        long double lat1 = toRadians(this->lat);
        long double long1 = toRadians(this->lon);
        long double lat2 = toRadians(other.lat);
        long double long2 = toRadians(other.lon);
        
        // Haversine Formula
        long double diff_lon = long2 - long1;
        long double diff_lat = lat2 - lat1;
        long double ans = pow(sin(diff_lat / 2), 2) + cos(lat1)*cos(lat2)*pow(sin(diff_lon / 2), 2);

        ans = 2*asin(sqrt(ans));

        // Radius of Earth in Kilometers is R = 6371Km
        long double R = 6371;

        ans = ans * R;

        return ans;
    }

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

int main(void)
{
    // Read the map.osm file
    ifstream input("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);

    // Find out the root node
    root_node = doc.first_node("osm");

    int count_node = 0,  count_ways = 0;
    // int count_tags = 0;

    map<ll, node> list_nodes;
    vector<way> lsit_ways;

    // Iterate over the nodes
    xml_node<> *way_strt = root_node->first_node("way"); // to start way and stop node
    xml_node<> *way_end = root_node->first_node("relation"); // to stop way

    for (xml_node<> *parse_node = root_node->first_node("node"); parse_node != way_strt; parse_node = parse_node->next_sibling())
    {
        count_node ++; // counting the number of nodes

        node temp_nd;
        temp_nd = node(stoll(parse_node->first_attribute("id")->value()), stoll(parse_node->first_attribute("uid")->value()), stod(parse_node->first_attribute("lat")->value()), stod(parse_node->first_attribute("lon")->value()), ctos(parse_node->first_attribute("user")->value()));

        bool is_nm = false;
        string pl_name;

        for (xml_node<> *parse_tag = parse_node->first_node("tag"); parse_tag; parse_tag = parse_tag->next_sibling())
        {
            if (parse_tag->first_attribute("k")->value() == "name")
            {
                pl_name = parse_tag->first_attribute("v")->value();
            }
        }

        temp_nd.plc_nm = pl_name;
        list_nodes.insert(pair<ll, node>(temp_nd.id, temp_nd));
    }

    cout << "Number of nodes present in the map are: " << count_node << "\n";

    // Iterate over the way
    for (xml_node<> *parse_way = way_strt; parse_way != way_end; parse_way = parse_way->next_sibling())
    {
        count_ways ++;
        way temp_way;
        temp_way = way(stoll(parse_way->first_attribute("id")->value()), stoll(parse_way->first_attribute("uid")->value()), ctos(parse_way->first_attribute("user")->value()));
        lsit_ways.push_back(temp_way);
    }
    
    cout << "Number of ways present in the map are: " << count_ways << "\n";

    node temp_node;

    int choice;
    while (true)
    {
        cout << "Enter 1 to search the id's enter by a user.\n";
        cout << "Enter 2 to find the k-closest nodes of a particular node.\n";
        cout << "Enter 3 to see the the length of the shortest path between 2 particular nodes.\n";
        cout << "Enter 0 to exit the loop.\n";
        cin >> choice;
        if (choice == 0)
        {
            break;
        }
        
        switch (choice)
        {
        case 1:
            break;
        case 2:
        {
            ll id;
            int k;
            cout << "Enter the id of node from which you want to find distance of k-closest nodes: ";
            cin >> id;
            cout << "Enter the value of k: ";
            cin >> k;
            int ind_id = 0;
            node temp_nd;
            for(auto itr : list_nodes)
            {
                if (itr.first == id)
                {
                    temp_nd = itr.second;
                    break;
                }
                ind_id ++;
            }

            vector<pair<ll, long double>> dist_arr = temp_nd.distance_from_allnodes(list_nodes);
            cout << "The first " << k << " closest nodes with id " << id << " are:\n";
            for (int i = 0; i < k; i++)
            {
                cout << dist_arr[i].first << " " << dist_arr[i].second << "\n";
            }
        }
            break;
        default:
            cout << "Invalid Input!!\n";
            break;
        }
    }

    return 0;
}