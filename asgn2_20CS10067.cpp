// NAME: VENKATA SAI SUVVARI
// ROLL: 20CS10067

#include <bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

xml_document<> doc;
xml_node<> *root_node = NULL;

int main(void)
{
    cout << "\nParsing my students data (sample.xml)....." << endl;

    // Read the sample.xml file
    ifstream theFile("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);

    // Find out the root node
    root_node = doc.first_node("osm");

    // Iterate over the student nodes
    int count_node = 0, count_tags = 0, count_ways = 0;
    for (xml_node<> *parse_node = root_node->first_node("node"); parse_node != root_node->first_node("way"); parse_node = parse_node->next_sibling())
    {
        cout << parse_node->first_attribute("id")->value();
        cout << endl;

        // Interate over the Student Names
        count_node ++;
        for (xml_node<> *student_name_node = parse_node->first_node("tag"); student_name_node; student_name_node = student_name_node->next_sibling())
        {
            // cout << "Student Name =   " << student_name_node->value();
            // cout << endl;
            count_tags ++;
        }
        // cout << endl;
    }

    cout << "Number of nodes = " << count_node << "\n";
    cout << "Number of tags = " << count_tags << "\n";

    for (xml_node<> *parse_way = root_node->first_node("way"); parse_way; parse_way = parse_way->next_sibling())
    {
        count_ways ++;
    }
    
    cout << "Number of ways = " << count_ways << "\n";

    return 0;
}
