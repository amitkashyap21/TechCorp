# Organizational Hierarchy Management System

A comprehensive C++ implementation of a corporate management structure utilizing an **N-ary Tree (General Tree)** data structure. This project serves as a semester evaluation for Data Structures and Algorithms (DSA), demonstrating tree manipulation, recursive processing, and data persistence.

---

## 🏗️ System Architecture

The system models a real-world corporate hierarchy where a "Root" node represents the Company Headquarters, and subsequent nodes represent departments or employees.



### Core Data Structures
* **N-ary Tree:** Unlike binary trees, each node can have an arbitrary number of children, stored in a `std::vector<Node*>`.
* **Employee Struct:** Encapsulates business data ($ID$, $Name$, $Position$, $Salary$).
* **Parent Pointers:** Each node maintains a pointer to its superior, allowing for efficient upward traversal (Bottom-up).

---

## 🚀 Key Features & Logic

### 1. Engine & Search (Naisha)
* **BFS Search:** Uses a `std::queue` to perform a Breadth-First Search to find any employee by their unique ID.
* **Dynamic Insertion:** The `hire` function allows adding nodes at any depth, dynamically expanding the organization.

### 2. Advanced Algorithms (Chahat)
* **Structural Integrity (Fire):** When a node is deleted, the system prevents a "broken tree" by automatically promoting all subordinates to report to the deleted node's supervisor.
* **LCA Algorithm:** The `findLowestCommonBoss` function identifies the closest shared superior by tracing ancestral paths from two different nodes.
* **Recursive Budgeting:** A depth-first traversal that calculates the total payroll.
    $$Total = Salary_{current} + \sum_{i=1}^{n} Total(child_i)$$

### 3. Display & Persistence (Amit)
* **Recursive Visualization:** Prints the tree with hierarchical indentation (`|--`) for clear visual reporting.
* **Data Serialization:** Implements a custom file format to save the tree structure to `org_data.txt`, preserving the parent-child relationships for future sessions.

---

## 🛠️ Technical Specifications

### Complexity Analysis
| Operation | Algorithm | Complexity |
| :--- | :--- | :--- |
| **Search by ID** | Breadth-First Search | $O(N)$ |
| **Hiring** | Node Insertion | $O(1)$* |
| **Firing** | Re-linking + Deletion | $O(N)$ |
| **Budgeting** | Recursive Traversal | $O(N)$ |
| **Common Boss** | Path Intersection | $O(H)$ |
*\*After finding the target node.*

---

## 💻 Implementation Details

### Compilation
The project is written in standard C++ and can be compiled using any modern compiler:
```bash
g++ -o OrgSystem main.cpp
./OrgSystem
