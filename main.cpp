#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// vector containing the proper solution matrix
const vector<int> solution{1,2,3,4,5,6,7,8,0};

class Node {
    public:
        int depth;
        int heuristic;
        int cost; // depth + heuristic
        vector<int> puzzle;

        Node(vector<int> puzzle, int depth, int heuristic) {
            this->puzzle = puzzle;
            this->depth = depth;
            this->heuristic = heuristic;
            cost = depth + heuristic;
        }
};

// Overloads for priority queue and uses heuristic total
// Referenced structure (in report)
bool operator<(Node a, Node b) {
    return a.cost > b.cost;
}

// Prints out each tile of the puzzle in matrix form
void printPuzzle(vector<int> puzzle) {
    cout << puzzle[0] << " " << puzzle[1] << " " << puzzle[2] << endl;
    cout << puzzle[3] << " " << puzzle[4] << " " << puzzle[5] << endl;
    cout << puzzle[6] << " " << puzzle[7] << " " << puzzle[8] << endl << endl;
}

// Returns misplaced tile heuristic
int misplacedTile(vector<int> puzzle) {
    int heur = 0;
    for (int i = 0; i < puzzle.size(); ++i) {
        if (puzzle.at(i) != solution.at(i)) {
            ++heur;
        }
    }
    return heur;
}

// Returns count for manhattan distance in our puzzle
// dist = abs(x2 - x1) + abs(y2-y1)
// Referenced structure (in report)
int manhattanDistance(vector<int> puzzle) {
    int count = 0;
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < solution.size(); j++) {
            if (puzzle[i] == solution[j]) {
                if (i != j) {
                    count += + abs((i % 3) - (j % 3)) + abs((i / 3) - (j / 3));
                }
            }
        }
    }
    return count;
}

// Returns the index number where 0 is located
int findZero(vector<int> puzzle) {
    for (int i = 0; i < puzzle.size(); ++i) {
        if (puzzle.at(i) == 0) {
            return i;
        }
    }
    return -1;
}

// Checks if we can move up on the matrix
// If 0 is in first row, we can't go up [0,1,2]
bool checkUp(int index) {
    if (index == 0 || index == 1 || index == 2) {
        return false;
    }
    return true;
}

// Checks if we can move down on the matrix
// If 0 is in last row, we can't go up [6,7,8]
bool checkDown(int index) {
    if (index == 6 || index == 7 || index == 8) {
        return false;
    }
    return true;
}

// Checks if we can move left on the matrix
// If 0 is in first column, we can't go left [0,3,6]
bool checkLeft(int index) {
    if (index == 0 || index == 3 || index == 6) {
        return false;
    }
    return true;
}

// Checks if we can move right on the matrix
// If 0 is in last column, we can't go right [2,5,8]
bool checkRight(int index) {
    if (index == 2 || index == 5 || index == 8) {
        return false;
    }
    return true;
}

// Expands the node and updates values based on the type of alg
// Returns vector with children and depth/heuristic values
// Referenced structure (in report)
vector<Node> expand(Node curr, int algorithm) {
    vector<int> parent = curr.puzzle; // sets parent to current puzzle
    vector<int> child; // sets child to swap with parent later
    vector<Node> children;

    int depth = 0;
    int heuristic = 0;
    int index = 0;

    depth = curr.depth + 1;
    index = findZero(parent); // returns the index of where 0 is located

    if (checkUp(index) == true) { //checks to move Up
        child = parent;
        // swap function from included library
        swap(child.at(index - 3), child.at(index)); // swaps the blank tile with the tile above 
        
        if (algorithm == 1) {
            heuristic = 0;
        }
        else if (algorithm == 2) {
            heuristic = misplacedTile(child);
        }
        else if (algorithm == 3) {
            heuristic = manhattanDistance(child);
        }
        Node result(child, depth, heuristic);
        children.push_back(result);
    }

    if (checkDown(index) == true) { // checks to move Down
        child = parent;
        swap(child.at(index + 3), child.at(index)); // swaps the blank tile with the tile below

        if (algorithm == 1) {
            heuristic = 0;
        }
        else if (algorithm == 2) {
            heuristic = misplacedTile(child);
        }
        else if (algorithm == 3) {
            heuristic = manhattanDistance(child);
        }
        Node result(child, depth, heuristic);
        children.push_back(result);
    }
    
    if (checkLeft(index) == true) { // checks to move Left
        child = parent;
        swap(child.at(index - 1), child.at(index)); // swaps the blank tile with the tile left
        
        if (algorithm == 1) {
            heuristic = 0;
        }
        else if (algorithm == 2) {
            heuristic = misplacedTile(child);
        }
        else if (algorithm == 3) {
            heuristic = manhattanDistance(child);
        }
        Node result(child, depth, heuristic);
        children.push_back(result);
    }

    if (checkRight(index) == true) { // checks to move Right
        child = parent;
        swap(child.at(index + 1), child.at(index)); // swaps the blank tile with the tile right
        
        if (algorithm == 1) {
            heuristic = 0;
        }
        else if (algorithm == 2) {
            heuristic = misplacedTile(child);
        }
        else if (algorithm == 3) {
            heuristic = manhattanDistance(child);
        }
        Node result(child, depth, heuristic);
        children.push_back(result);
    }

    return children; // nodes have been expanded and results with their depth/heuristic stored in vector
}

// General search to expand nodes and sort through the priority queue based on chosen algorithm
void searchAlgorithm(vector<int> puzzle, int algorithm) {
    Node curr(puzzle, 0, 0);
    vector<Node> children;
    vector<Node> visited;
    priority_queue<Node, vector<Node>> pq;
    int heuristic = 0;
    int maxQueued = 0;
    int numExpanded = 0;

    Node root(puzzle, 0, heuristic);
    pq.push(root); // this begins the priority queue with the very first node

    while (curr.puzzle != solution) { // continues until puzzle matches our solution
        visited.push_back(curr); // tracks nodes we have visited
        children = expand(curr, algorithm); // calls expand function
        for (int i = 0; i < children.size(); ++i) {
            pq.push(children.at(i)); // send the children into our queue
        }

        if (pq.size() > maxQueued) {
            maxQueued = pq.size(); // adjusts our max queue
        }

        // resets and clears the queue
        curr = pq.top();
        pq.pop();

        // Uniform cost with always have h hardcoded to 0
        if (algorithm == 1) {
            heuristic = 0;
            cout << "The best state to expand with a g_n(n) = " << curr.depth << " is.." << endl;
            printPuzzle(curr.puzzle);
        }
        else if (algorithm == 2) {
            heuristic = misplacedTile(puzzle);
            cout << "The best state to expand with a g_n(n) = " << curr.depth << " and h_n(n) = " << heuristic << " is.." << endl;
            printPuzzle(curr.puzzle);
        }
        else if (algorithm == 3) {
            heuristic = manhattanDistance(puzzle);
            cout << "The best state to expand with a g_n(n) = " << curr.depth  << " and h_n(n) = " << heuristic << " is.." << endl;
            printPuzzle(curr.puzzle);
        }

        ++numExpanded; // increments the number of nodes expanded

    }

    if (curr.puzzle == solution) { // checks to see puzzle matches solution
        cout << "Goal!!" << endl;
        cout << "To solve this problem, the search algorithm expanded a total of " << numExpanded << " nodes" << endl;
        cout << "The maximum number of nodes in the queue at any one time was " << maxQueued << endl;
        cout << "The depth of the goal node was " << curr.depth << endl;
    }
}

int main() {
    int input = 0;
    int count = 0;
    int puzzleVal = 0;
    vector<int> puzzle;

    do {
        cout << "Welcome to the 8-puzzle solver!" << endl;
        cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
        cin >> input;
        if (input == 1) {
            cout << "Default Puzzle: " << endl;
            puzzle.push_back(1);
            puzzle.push_back(2);
            puzzle.push_back(3);
            puzzle.push_back(4);
            puzzle.push_back(0);
            puzzle.push_back(6);
            puzzle.push_back(7);
            puzzle.push_back(5);
            puzzle.push_back(8);
            printPuzzle(puzzle);
        }
        else if (input == 2) {
            cout << "Enter your puzzle with a space in between each digit and use 0 to represent the blank." << endl;
            for (int i = 0; i < solution.size(); ++i) {
                cin >> puzzleVal;
                if (puzzleVal > 8 || puzzleVal < 0) { // only digits 0-8 accepted
                    cout << "Error: Invalid input" << endl;
                }
                puzzle.push_back(puzzleVal);
                ++count;
            }
            cout << "Custom Puzzle:" << endl;
            printPuzzle(puzzle);
        }
    } while (input != 1 && input != 2);
    
    // Choose algorithm for puzzle
    cout << "Enter your input of algorithm:" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile heuristic" << endl;
    cout << "3. A* with the Manhattan Distance heuristic" << endl;
    cin >> input;
    
    searchAlgorithm(puzzle, input);

    return 0;
}