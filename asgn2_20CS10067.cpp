// NAME: VENKATA SAI SUVVARI
// ROLL: 20CS10067

#include <bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

typedef long long ll;

xml_document<> doc;
xml_node<> *root_node = NULL;

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

class node
{
    public:
    ll id, uid;
    double lat, lon;
    string usr_nm;

    node()
    {
        this->id = 0;
        this->lat = this->lon = 0.0;
        this->usr_nm = "name";
    }
    node(ll id, ll uid, double lt, double ln, string nm)
    {
        this->id = id;
        this->uid = uid;
        this->lat = lt;
        this->lon = ln;
        this->usr_nm = nm;
    }
};

class way
{
    public:
    ll id, uid;
    string usr_nm;

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

    vector<node> list_nodes;
    vector<way> lsit_ways;

    // Iterate over the nodes
    xml_node<> *way_strt = root_node->first_node("way"); // to start way and stop node
    xml_node<> *way_end = root_node->first_node("relation"); // to stop way

    for (xml_node<> *parse_node = root_node->first_node("node"); parse_node != way_strt; parse_node = parse_node->next_sibling())
    {
        // cout << parse_node->first_attribute("id")->value();
        // cout << endl;

        count_node ++; // counting the number of nodes

        node temp_nd;
        temp_nd = node(stoll(parse_node->first_attribute("id")->value()), stoll(parse_node->first_attribute("uid")->value()), stod(parse_node->first_attribute("lat")->value()), stod(parse_node->first_attribute("lon")->value()), ctos(parse_node->first_attribute("user")->value()));
        list_nodes.push_back(temp_nd);
    }

    cout << "Number of nodes present in the map are: " << count_node << "\n";
    // cout << "Number of tags = " << count_tags << "\n";

    // Iterate over the way
    for (xml_node<> *parse_way = way_strt; parse_way != way_end; parse_way = parse_way->next_sibling())
    {
        count_ways ++;
        way temp_way;
        temp_way = way(stoll(parse_way->first_attribute("id")->value()), stoll(parse_way->first_attribute("uid")->value()), ctos(parse_way->first_attribute("user")->value()));
        lsit_ways.push_back(temp_way);
    }
    
    cout << "Number of ways present in the map are: " << count_ways << "\n";

    return 0;
}
