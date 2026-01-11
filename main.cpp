#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

// --- SHARED DATA STRUCTURE ---
struct Employee {
    int id;
    string name;
    string position;
    double salary;
};

class Node {
public:
    Employee* emp;
    string deptName;
    bool isDept;
    Node* parent;
    vector<Node*> children;

    // Constructor for Department
    Node(string dName, Node* p = nullptr) : deptName(dName), parent(p), isDept(true), emp(nullptr) {}
    // Constructor for Employee
    Node(Employee* e, Node* p = nullptr) : emp(e), parent(p), isDept(false), deptName("") {}

    ~Node() {
        if (emp) delete emp;
    }
};

class OrgChart {
private:
    Node* root;

    // Helper for Recursive display (AMIT)
    void displayRecursive(Node* curr, int depth) {
        if (!curr) return;
        for (int i = 0; i < depth; i++) cout << "  |--";
        
        if (curr->isDept) cout << "[DEPT] " << curr->deptName << endl;
        else cout << curr->emp->name << " (" << curr->emp->position << ") - ID: " << curr->emp->id << endl;

        for (Node* child : curr->children) {
            displayRecursive(child, depth + 1);
        }
    }

    // Helper for Serialization (AMIT)
    void saveRecursive(ofstream& ofs, Node* curr) {
        if (!curr) return;
        ofs << curr->isDept << ",";
        if (curr->isDept) {
            ofs << curr->deptName << "\n";
        } else {
            ofs << curr->emp->id << "," << curr->emp->name << "," 
                << curr->emp->position << "," << curr->emp->salary << "\n";
        }
        ofs << curr->children.size() << "\n";
        for (Node* child : curr->children) {
            saveRecursive(ofs, child);
        }
    }

public:
    OrgChart(string companyName) { root = new Node(companyName); }

    // NAISHA: ENGINE & CORE OPERATIONS
    
    Node* findNode(int id) {
        if (!root) return nullptr;
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            if (!curr->isDept && curr->emp && curr->emp->id == id) return curr;
            for (Node* child : curr->children) q.push(child);
        }
        return nullptr;
    }

    void hire(int managerID, Employee* e) {
        Node* managerNode = findNode(managerID);
        // Fallback: If manager is 0 or not found, try checking if root is intended target
        if (!managerNode && managerID == 0) managerNode = root; 

        if (!managerNode) {
            cout << "Error: Manager ID " << managerID << " not found.\n";
            return;
        }
        Node* newEmpNode = new Node(e, managerNode);
        managerNode->children.push_back(newEmpNode);
        cout << "Employee " << e->name << " hired under " << (managerNode->isDept ? managerNode->deptName : managerNode->emp->name) << ".\n";
    }

    // CHAHAT: ADVANCED LOGIC & ALGORITHMS

    void fire(int id) {
        Node* empNode = findNode(id);
        if (!empNode || !empNode->parent) {
            cout << "Employee not found or cannot fire the root node.\n";
            return;
        }

        Node* boss = empNode->parent;
        // Promote children to the boss
        for (Node* child : empNode->children) {
            child->parent = boss;
            boss->children.push_back(child);
        }

        // Remove the employee from the boss's list
        auto& siblings = boss->children;
        siblings.erase(remove(siblings.begin(), siblings.end(), empNode), siblings.end());

        delete empNode; // Destructor handles Employee* deletion
        cout << "Employee ID " << id << " has been removed and subordinates promoted.\n";
    }

    double calculateBudget(Node* start) {
        if (!start) return 0.0;
        double sum = (!start->isDept) ? start->emp->salary : 0.0;
        for (Node* child : start->children) sum += calculateBudget(child);
        return sum;
    }

    void findLowestCommonBoss(int id1, int id2) {
        Node* n1 = findNode(id1);
        Node* n2 = findNode(id2);
        if (!n1 || !n2) { cout << "One or both employees not found.\n"; return; }

        vector<Node*> path1;
        while (n1) { path1.push_back(n1); n1 = n1->parent; }

        Node* temp = n2;
        while (temp) {
            for (Node* p : path1) {
                if (temp == p) {
                    cout << "Lowest Common Boss: " << (temp->isDept ? temp->deptName : temp->emp->name) << endl;
                    return;
                }
            }
            temp = temp->parent;
        }
    }

    // AMIT: DISPLAY & DATA PERSISTENCE

    void display() {
        cout << "\n~ ORGANIZATION STRUCTURE ~\n";
        displayRecursive(root, 0);
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }

    void saveToFile(string filename) {
        ofstream ofs(filename);
        if (!ofs) { cout << "Error opening file for saving.\n"; return; }
        saveRecursive(ofs, root);
        ofs.close();
        cout << "Data saved to " << filename << endl;
    }

    Node* getRoot() { return root; }
};

// --- AMIT: MAIN INTERFACE ---
void showMenu() {
    cout << "\n1. Hire Employee\n2. Fire Employee\n3. Display Org Chart\n"
         << "4. Calculate Salary Budget\n5. Find Common Boss\n6. Save to File\n0. Exit\n"
         << "Choice: ";
}

int main() {
    OrgChart myCompany("TechCorp HQ");
    int choice, id, mId;
    string name, pos;
    double sal;

    while (true) {
        showMenu();
        cin >> choice;
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                cout << "Enter Manager ID (or 0 for HQ): "; cin >> mId;
                cout << "Enter Emp ID, Name, Position, Salary: ";
                cin >> id >> name >> pos >> sal;
                myCompany.hire(mId, new Employee{id, name, pos, sal});
                break;
            }
            case 2:
                cout << "Enter ID to fire: "; cin >> id;
                myCompany.fire(id);
                break;
            case 3:
                myCompany.display();
                break;
            case 4:
                cout << "Total Salary Budget: $" << myCompany.calculateBudget(myCompany.getRoot()) << endl;
                break;
            case 5:
                int idA, idB;
                cout << "Enter two Employee IDs: "; cin >> idA >> idB;
                myCompany.findLowestCommonBoss(idA, idB);
                break;
            case 6:
                myCompany.saveToFile("org_data.txt");
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
