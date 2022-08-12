#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Node{
    /*
        Count how many outbounds this vertex has
        Will be used in page rank calculations
    */
    unsigned int outboundCount = 0;

    // Hold the index of the inbound vertices to this vertex
    vector<unsigned int> inbound;

    /*
        Hold previous rank and actual rank.
        FirstPair value in adjacency class will toggle which one to use.
        This prevents using the new calculated value during iterations.
    */
    pair<float,float> rank = {0.0,0.0};
};

class AdjacencyList {
private:
    // Track index of inserting
    unsigned int i = 0;

    // Map the vertices to an index
    map<string, unsigned int> vertices;

    // Map the index of the vertices to a Node
    unordered_map<unsigned int, Node*> graph;

    // Used to toggle which pair element to use for page rank calculations, and which value to display
    bool firstPair = true;

public:
    void insertEdge(string from, string to){
        /*
            Check if both vertices are in the vertices map
            if not create them, assign index, and assign node
        */
        if(vertices.find(from) == vertices.end()){
            vertices[from] = i;
            // Insert index in graph with new node
            graph[i] = new Node();
            // Increase index for next insert mapping
            i++;
        }
        if(vertices.find(to) == vertices.end()){
            vertices[to] = i;
            // Insert index in graph with new node
            graph[i] = new Node();
            // Increase index for next insert mapping
            i++;
        }

        // Increment outbound in from vertices
        graph[vertices[from]]->outboundCount += 1;

        // Add inbound index in to vertices
        graph[vertices[to]]->inbound.push_back(vertices[from]);
    }

    void PageRank(int n){
        /*
            Initialize page rank of all to 1 / number of vertices
            the index variable i will be equal to the number of vertices that we have
        */
        float iPgRk = 1.0 / float (i);

        // Initialize all vertices to that initial page rank
        for (auto it = vertices.begin(); it != vertices.end(); it++){
            graph[it->second]->rank.first = iPgRk;
        }

        // Perform the calculation for n number of iteration
        for(int i = 1; i < n; i++){
            // Calculate page rank
            for (auto it = vertices.begin(); it != vertices.end(); it++){
                float pageRank = 0.0;

                // get inbounds links
                vector<unsigned int> inboundLinks = graph[it->second]->inbound;

                // calculate page rank with the sum of each inbound link page rank over their outbound
                for(int inboundLink : inboundLinks){
                    float previousRank = (firstPair ? graph[inboundLink]->rank.first : graph[inboundLink]->rank.second);
                    pageRank += (previousRank / graph[inboundLink]->outboundCount);
                }

                // Set the value of page Rank to current node
                if(firstPair) graph[it->second]->rank.second = pageRank;
                else graph[it->second]->rank.first = pageRank;
            }

            // Toggle the value of firstPair for next iteration
            if(firstPair) firstPair = false;
            else firstPair = true;
        }

        // Print page rank
        for (auto it = vertices.begin(); it != vertices.end(); it++) {
            cout << it->first << " " << fixed << setprecision(2) << (firstPair ? graph[it->second]->rank.first : graph[it->second]->rank.second) << endl;
        }
    }

    // Helper method to print graph
    void printVertices(){
        for (auto it = vertices.begin(); it != vertices.end(); it++){
            // Print vertices name and index
            cout << it->second << ". " << it->first << endl;

            // Print outbound count
            cout << "     " << setw(10) << "Outbound" << "-> " << graph[it->second]->outboundCount << endl;

            // Print inbound links
            vector<unsigned int> inboundLinks = graph[it->second]->inbound;
            cout << "     " << setw(10) << "Inbound" << "-> ";
            for(int inboundLink : inboundLinks){
                cout << inboundLink << " ";
            }
            cout << endl;
            cout << endl;
        }
    }
};

int main()
{
    int no_of_lines, power_iterations;
    std::string from, to;
    std::cin >> no_of_lines;
    std::cin >> power_iterations;

    // Create Graph
    AdjacencyList graph;

    for(int i=0; i< no_of_lines; i++)
    {
        std::cin >> from;
        std::cin >> to;

        // Insert into the graph
        graph.insertEdge(from, to);
    }

    // Calculate page rank
    graph.PageRank(power_iterations);
}