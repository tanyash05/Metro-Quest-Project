#include<iostream>
#include<unordered_map>
#include<vector>
#include<cstring>
#include<algorithm>
#include<queue>
#include<cmath>
#include<climits>
#include<sstream>
using namespace std;

// Class to represent a Graph
class Graph {

    public:

    // Inner class to represent a Vertex in the graph
    class Vertex {
    public:
        unordered_map<string, int> neighbours; // Neighbours and edge weights
    };

    static unordered_map<string, Vertex> vertices;

    Graph() {
        if (vertices.empty()) {
            vertices = unordered_map<string, Vertex>();
        }
    }

    // Function to get the number of vertices in the graph
    int numVertex() {
        return vertices.size();
    }
     
    // Function to check if a vertex exists in the graph or not
    bool containsVertex(string Vname) {
        return vertices.find(Vname) != vertices.end();
    }

    // Function to add a vertex to the graph
    void addVertex(string Vname) {
        Vertex vtx;
        vertices[Vname] = vtx;
    }

    // Function to remove a vertex from the graph
    void removeVertex(string Vname) {
        Vertex vtx = vertices[Vname];
        vector<string> keys;

        // Collect all neighbour keys
        for (auto const& it : vtx.neighbours) {
            keys.push_back(it.first);
        }
        // Remove the vertex from its neighbours
        for (string key : keys) {
            vertices[key].neighbours.erase(Vname);
        }
        // Remove the vertex
        vertices.erase(Vname);
    }

    // Function to get the number of edges in the graph
    int numEdges() {
        int count = 0;
        for (const auto& element : vertices) {
            count += element.second.neighbours.size();
        }

        return count / 2;
    }

    // Function to check if an edge exists between two vertices or not
    bool containsEdge(string Vname1, string Vname2) {
        if (!containsVertex(Vname1) || !containsVertex(Vname2)) {
            return false;
        }

        return vertices[Vname1].neighbours.find(Vname2) != vertices[Vname1].neighbours.end();
    }

    // Function to add an edge between two vertices
    void addEdge(string Vname1, string Vname2, int value) {
        if (!containsVertex(Vname1) || !containsVertex(Vname2) || containsEdge(Vname1, Vname2)) {
            return;
        }

        vertices[Vname1].neighbours[Vname2] = value;
        vertices[Vname2].neighbours[Vname1] = value;
    }

    // Function to remove an edge between two vertices
    void removeEdge(string Vname1, string Vname2) {
        if (!containsVertex(Vname1) || !containsVertex(Vname2) || !containsEdge(Vname1, Vname2)) {
            return;
        }

        vertices[Vname1].neighbours.erase(Vname2);
        vertices[Vname2].neighbours.erase(Vname1);
    }

    // Function to display the graph
    void display_Map() {
        cout << "\t\t\t DELHI METRO MAP" << endl;
        cout << "\t\t\t---------------------" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;

        vector<string> keys;
        for (auto const& element : Graph::vertices) {
            keys.push_back(element.first);
        }

        // Display each vertex and its neighbours
        for (string key : keys) {
            string str = key + " =>\n";
            Vertex vtx = vertices[key];
            vector<string> VtxNeighbours;
            for (auto const& element : vtx.neighbours) {
                VtxNeighbours.push_back(element.first);
            }

            for (string nbr : VtxNeighbours) {
                str = str + "\t" + nbr + "\t";
                if (nbr.length() < 16)
                    str = str + "\t";
                if (nbr.length() < 8)
                    str = str + "\t";
                str = str + to_string(vtx.neighbours[nbr]) + "\n";
            }
            cout << str << endl;
        }

        cout << "\t\t----------------------------------------------------" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
        cout<<endl;
    }

    // Function to display all station names
    void display_Stations() {
        cout << "\n*********************************************\n" << endl;
        vector<string> keys;
        for (auto const& element : Graph::vertices) {
            keys.push_back(element.first);
        }
    
        // Display each station
        int i = 1;
        for (string key : keys) {
            cout << i << ". " << key << endl;
            i++;
        }
        
        cout << "\n*********************************************\n" << endl;
    }

    // Function to check if there's a path between two vertices
    bool hasPath(string Vname1, string Vname2, unordered_map<string, bool>& processed) {
        // Direct Edge
        if (containsEdge(Vname1, Vname2)) {
            return true;
        }

        processed[Vname1] = true;

        Vertex vtx = vertices[Vname1];
        vector<string> neighbours;
        for (auto const& element : vtx.neighbours) {
            neighbours.push_back(element.first);
        }

        for (string nbr : neighbours) {
            if (processed[nbr]) { 
                continue; 
            }
            processed[nbr] = true;
            if (hasPath(nbr, Vname2, processed)) {
                return true;
            }
        }
       
        return false;
    }

    
    // Class to represent a pair for Dijkstra's algorithm
    class Dijkstra_Pair {
    public:
        string Vname;
        string shortest_path;
        int cost;

        // Overload > operator for priority queue comparison
        bool operator>(const Dijkstra_Pair& other) const {
            return cost > other.cost;
        }
    };

    // Function to find the shortest path using Dijkstra's algorithm
    int dijkstra(string source, string destination, bool Nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, Dijkstra_Pair> map;

        // Priority queue to act as the min-heap
        priority_queue<Dijkstra_Pair, vector<Dijkstra_Pair>, greater<Dijkstra_Pair>> pq;

        for (auto const& key : vertices) {
            // np = new pair
            Dijkstra_Pair np;
            np.Vname = key.first;
            np.cost = INT_MAX;
            np.shortest_path = "";

            if (key.first == source) {
                np.cost = 0;
                np.shortest_path = key.first;
            }

            pq.push(np);
            map[key.first] = np;
        }

        while (!pq.empty()) {
            // rp = remove pair
            Dijkstra_Pair rp = pq.top();
            pq.pop();

            if (rp.Vname == destination) {
                val = rp.cost;
                break;
            }

            if (map.find(rp.Vname) == map.end()) {
                continue; 
            }

            map.erase(rp.Vname);
            ans.push_back(rp.Vname);

            Vertex v = vertices[rp.Vname];
            for (auto const& nbr : v.neighbours) {
                if (map.find(nbr.first) != map.end()) {
                    int oldCost = map[nbr.first].cost;
                    int newCost;
                    if (Nan) {
                        newCost = rp.cost + 120 + 40 * v.neighbours[nbr.first];
                    } else {
                        newCost = rp.cost + v.neighbours[nbr.first];
                    }

                    if (newCost < oldCost) {
                        Dijkstra_Pair& gp = map[nbr.first];
                        gp.shortest_path = rp.shortest_path + nbr.first;
                        gp.cost = newCost;

                        // Update priority by pushing the updated pair
                        pq.push(gp);
                    }
                }
            }
        }
        return val;
    }


    class Pair {
    public:
        string Vname;
        string shortest_path;
        int min_dis;
        int min_time;
    };

    // Function to get the path with the minimum distance
    string Get_Minimum_Distance(string source, string destination) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> dq;

        Pair sp;
        sp.Vname = source;
        sp.shortest_path = source + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        dq.push_front(sp);

        while (!dq.empty()) {
            Pair rp = dq.front();
            dq.pop_front();

            if (processed.count(rp.Vname)) {
                continue;
            }

            processed[rp.Vname] = true;

            if (rp.Vname == destination) {
                int temp = rp.min_dis;
                if (temp < min) {
                    ans = rp.shortest_path;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vertices[rp.Vname];
            vector<string> neighbours;
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++) {
                neighbours.push_back(it->first);
            }

            for (string nbr : neighbours) {
                if (!processed.count(nbr)) {
                    Pair np;
                    np.Vname = nbr;
                    np.shortest_path = rp.shortest_path + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.neighbours[nbr];
                    dq.push_front(np);
                }
            }
        }

        ans += to_string(min);
        return ans;
    }

    // Function to get the path with the minimum time
    string Get_Minimum_Time(string source, string destination) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> dq;

        Pair sp;
        sp.Vname = source;
        sp.shortest_path = source + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        dq.push_front(sp);

        while (!dq.empty()) {
            Pair rp = dq.front();
            dq.pop_front();

            if (processed.count(rp.Vname)) {
                continue;
            }

            processed[rp.Vname] = true;

            if (rp.Vname == destination) {
                int temp = rp.min_time;
                if (temp < min) {
                    ans = rp.shortest_path;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vertices[rp.Vname];
            vector<string> neighbours;
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++) {
                neighbours.push_back(it->first);
            }

            for (string nbr : neighbours) {
                if (!processed.count(nbr)) {
                    Pair np;
                    np.Vname = nbr;
                    np.shortest_path = rp.shortest_path + nbr + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.neighbours[nbr];
                    dq.push_front(np);
                }
            }
        }
        
        // Convert seconds to minutes
        double minutes = ceil((double)min / 60);
        ans += to_string(minutes);
        return ans;
    }

    // Function to get the interchanges in a path
    vector<string> get_Interchanges(string str) {
        vector<string> arr;
        vector<string> res;
        int count = 0;

        // Split the input string by "  " and store the parts in res
        size_t pos = 0;
        while ((pos = str.find("  ")) != string::npos) {
            res.push_back(str.substr(0, pos));
            str.erase(0, pos + 2);
        }
        res.push_back(str); // Add the last segment
        count = res.size();

        arr.push_back(res[0]);

        // Process each segment to identify interchanges
        for (int i = 1; i < count - 1; i++) {
            int index = res[i].find("~");
            string s = res[i].substr(index + 1);

            if (s.length() == 2) {
                string prev = res[i - 1].substr(res[i - 1].find("~") + 1);
                string next = res[i + 1].substr(res[i + 1].find("~") + 1);

                if (prev == next) {
                    arr.push_back(res[i]);
                } else {
                    arr.push_back(res[i] + " ==> " + res[i + 1]);
                    i++;
                }
            } else {
                arr.push_back(res[i]);
            }
        }

        arr.push_back(to_string(count - 2)); 
        arr.push_back(res[count - 1]);

        return arr;
    }

    // Function to create the metro map
    void Create_Metro_Map(Graph& g)
    {
        g.addVertex("Noida Sector 62~B");
        g.addVertex("Botanical Garden~B");
        g.addVertex("Yamuna Bank~B");
        g.addVertex("Rajiv Chowk~BY");
        g.addVertex("Vaishali~B");
        g.addVertex("Moti Nagar~B");
        g.addVertex("Janak Puri West~BO");
        g.addVertex("Dwarka Sector 21~B");
        g.addVertex("Huda City Center~Y");
        g.addVertex("Saket~Y");
        g.addVertex("Vishwavidyalaya~Y");
        g.addVertex("Chandni Chowk~Y");
        g.addVertex("New Delhi~YO");
        g.addVertex("AIIMS~Y");
        g.addVertex("Shivaji Stadium~O");
        g.addVertex("DDS Campus~O");
        g.addVertex("IGI Airport~O");
        g.addVertex("Rajouri Garden~BP");
        g.addVertex("Netaji Subhash Place~PR");
        g.addVertex("Punjabi Bagh West~P");

        g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
        g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
        g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
        g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
        g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
        g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
        g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
        g.addEdge("Huda City Center~Y", "Saket~Y", 15);
        g.addEdge("Saket~Y", "AIIMS~Y", 6);
        g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
        g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
        g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
        g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
        g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
        g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
        g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
        g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
    }

};