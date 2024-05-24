#include<iostream>
#include<unordered_map>
#include<vector>
#include<sstream>
#include "graph.cpp"
using namespace std;

unordered_map<string, Graph::Vertex> Graph::vertices;

// Function to print a list of station names along with their generated codes
vector<string> printCodelist(unordered_map<string, Graph::Vertex>& vertices) {
    cout << "List of station along with their codes :\n" << endl;

    vector<string> keys;
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        keys.push_back(it->first);
    }

    int i = 1;
    int j = 0;
    int m = 1;
    vector<string> codes(keys.size());
    string temp;

    // Generate codes for each station name
    for (string key : keys) {
        istringstream stname(key);
        codes[i - 1] = "";
        j = 0;

        while (stname >> temp) {
            char c = temp[0];
            while (isdigit(c)) {
                codes[i - 1] += c;
                j++;
                c = temp[j];
            }
            if ((c < '0' || c > '9') && c < 'z') {
                codes[i - 1] += c;
            }
        }

        if (codes[i - 1].length() < 2) {
            codes[i - 1] += toupper(temp[1]);
        }

        cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            cout << "\t";
        if (key.length() < (14 - m))
            cout << "\t";
        if (key.length() < (6 - m))
            cout << "\t";
        cout << codes[i - 1] << endl;

        i++;
        if (i == static_cast<int>(pow(10, m)))
            m++;
    }

    return codes;
}


int main() {
    Graph g;
    g.Create_Metro_Map(g);

    cout << "\n\t\t\t\t\t******* WELCOME TO METROQUEST - YOUR JOURNEY PARTNER *******" << endl;

    while (true) {

        // ---------*********** AVOID SPACES IN INPUT **********------------ //
        
        cout << "\t\t\t\t\t\t\t-----EXPLORE JOURNEY OPTIONS-----\n\n";
        cout << "1. NAME ALL THE METRO STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE FROM 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME TO REACH FROM 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "7. EXIT THE OPTIONS" << endl;
        cout << "\n ENTER YOUR CHOICE FROM THE ABOVE OPTIONS (1-7) :" ;

        int choice = -1;
        cin >> choice;
        cin.ignore();

        cout << "\n*********************************************************************************\n";

        if (choice == 7) {
            break;
        }

        switch (choice) {


            case 1:
                g.display_Stations();
                break;


            case 2:
                g.display_Map();
                break;


            case 3: {
                vector<string> keys;
                for (auto station : Graph::vertices) {
                    keys.push_back(station.first);
                }
                vector<string> codes = printCodelist(g.vertices);
                cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
                cout << "ENTER YOUR CHOICE:";
                int choice;
                cin >> choice;
                int j;

                string st1 = "", st2 = "";
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                if (choice == 1) {
                    int serial1, serial2;
                    cin >> serial1 >> serial2;
                    st1 = keys[serial1 - 1];
                    st2 = keys[serial2 - 1];
                } else if (choice == 2) {
                    string a, b;
                    cin >> a >> b;
                    transform(a.begin(), a.end(), a.begin(), ::toupper);
                    transform(b.begin(), b.end(), b.begin(), ::toupper);
                    for (j = 0; j < keys.size(); j++) {
                        if (a == codes[j]) {
                            break;
                        }
                    }
                    st1 = keys[j];
                    for (j = 0; j < keys.size(); j++) {
                        if (b == codes[j]) {
                            break;
                        }
                    }
                    st2 = keys[j];
                } 
                else if (choice==3) {
                    cin.ignore();
                    getline(cin, st1);
                    getline(cin, st2);
                }
                 else {
                    cout << "Invalid choice" << endl;
                    exit(0);
                }

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM" << endl;
                }

                break;
            }


            case 4:{
                cout << "ENTER THE SOURCE STATION: ";
                string sat1;
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                string sat2;
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                cout << "SHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES\n\n";
                break;
            }


            case 5:{
                cout << "ENTER THE SOURCE STATION: ";
                string s1;
                getline(cin, s1);
                cout << "ENTER THE DESTINATION STATION: ";
                string s2;
                getline(cin, s2);

                unordered_map<string, bool> processed2;

                if(!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2)){
                    cout << "THE INPUTS ARE INVALID" << endl;
                }
                else 
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout << "-------------------------------------" << endl;
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len-1] << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len-2] << endl;
                    cout << "----------------" << endl;
                    cout << "START  ==>  " << str[0] << endl;
                    for(int i=1; i<len-3; i++)
                    {
                        cout << str[i] << endl;
                    }
                    cout << str[len-3] << "   ==>    END" << endl;
                    cout << "----------------" << endl;
                }

                break;
            }

	
            case 6: {
                cout << "ENTER THE SOURCE STATION: ";
                string sat11;
                getline(cin, sat11);
                cout << "ENTER THE DESTINATION STATION: ";
                string sat22;
                getline(cin, sat22);
				
					unordered_map<string, bool> processed3;
					if(!g.containsVertex(sat11) || !g.containsVertex(sat22) || !g.hasPath(sat11, sat22, processed3)){
                        cout << "THE INPUTS ARE INVALID" << endl;
                    }
					else
					{
						vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(sat11, sat22));
						int len = str.size();
                        cout << "-------------------------------------" << endl;
						cout << "SOURCE STATION : " << sat11 << endl;
						cout << "DESTINATION STATION : " << sat22 << endl;
						cout << "TIME : " << str[len-1] << " MINUTES" << endl;
						cout << "NUMBER OF INTERCHANGES : " << str[len-2] << endl;
                        cout << "----------------" << endl;
						cout << "START  ==>  " << str[0] << " ==>  ";
						for(int i=1; i<len-3; i++)
						{
							cout << str[i] << endl;
						}
						cout << str[len-3] << "   ==>    END" << endl;
                        cout << "----------------" << endl;
					}

                    break;
            }
            

            default:
                cout << "Please enter a valid option!" << endl;
                break;
        }
    }

    return 0;
}