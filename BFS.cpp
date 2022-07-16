/*Implementation of the BFS search algorithm*/

#include <iostream>
#include <string>

using namespace std;

// A simple node is a node with only 1 "next" pointer
class Simple_Node {
private:
	int Value;
	Simple_Node* Simple_Next;
public:
	// constructor
	Simple_Node(int value, Simple_Node* simple_next) {
		Value = value;
		Simple_Next = simple_next;
	}
	// getters
	int getValue() {
		return Value;
	}
	Simple_Node* getSimpleNext() {
		return Simple_Next;
	}
	// setters
	void setValue(int value) {
		Value = value;
	}
	void setSimpleNext(Simple_Node* simple_next) {
		Simple_Next = simple_next;
	}
};

// A double node is a node with 2 "next" pointers
class Double_Node : public Simple_Node {
private:
	Double_Node* Double_Next;
public:
	// constructor
	Double_Node(int value, Simple_Node* simple_next, Double_Node* double_next) :
		Simple_Node(value, simple_next) {
		Double_Next = double_next;
	};
	// getters
	Double_Node* getDoubleNext() {
		return Double_Next;
	}
	// setters
	void setDoubleNext(Double_Node* double_next) {
		Double_Next = double_next;
	}
};

// A Queue with the operations: FRONT(), EMPTY(), ENQUEUE(), DEQUEUE()
class Queue {
private:
	Simple_Node* Head;
public:
	// constructor
	Queue(Simple_Node* head) {
		Head = head;
	}
	// setters
	void setHead(Simple_Node* head) {
		Head = head;
	}

	// Returns first element of queue
	Simple_Node* Front() {
		return Head;
	}
	// Returns TRUE if queue is empty, else FALSE
	bool Empty() {
		if (Head == NULL)
			return true;
		else
			return false;
	}
	// Enqueue
	void Enqueue(Simple_Node* simple_node) {
		if (Head == NULL) {
			Head = simple_node;
		}
		else {
			Simple_Node* iter = Head;
			while (iter->getSimpleNext() != NULL)
				iter = iter->getSimpleNext();
			iter->setSimpleNext(simple_node);
		}
	}
	// Dequeue
	Simple_Node* Dequeue() {
		Simple_Node* temp = Head;
		Head = Head->getSimpleNext();
		return temp;
	}
};

// An indirected graph: a set of vertices V, and a set of edges E consisted of disordered pairs (v,w)
class Graph {
private:
	Double_Node* neighbors_list_head;
	int num_of_vertices;
	bool* visited_array;
	Queue Q = Queue(NULL);
public:
	// constructor
	Graph(string s) {
		int n = 0; // vertices counter
		int s_size = s.size(); // input string size
		size_t find_start = 0; // start position for the 'find' function
		size_t substr_start = 0; // start position for the 'substr' function
		size_t substr_end = 0; // end position for the 'substr' function, translated to <len> afterwards
		string node_str; // node value as string
		int node; // node value as integer
		string neighbor_str; // neighbor node value as string
		int neighbor; // neighbor node value as integer
		Double_Node* curr_double_node; // current node of primary list
		Double_Node* iter_double_node; // previous node of primary list
		Simple_Node* curr_simple_node; // current node of secondary list
		Simple_Node* iter_simple_node; // previous node of secondary list
		Simple_Node* secondary_list_head; // head of secondary list
		neighbors_list_head = NULL; /*head of neighbors list - a 2 dimensional linked list*/
		
		/*parsing the input string*/
		do {
			/*find positions of '{' and ':' that surround a primary node*/
			size_t open_curl_bracket = s.find('{', find_start);
			size_t semicolon = s.find(':', open_curl_bracket+1);
			
			/*capture node value and turn to integer*/
			node_str = s.substr(open_curl_bracket + 1, semicolon - open_curl_bracket - 1);
			node = stoi(node_str);
			
			/*allocation for new primary node*/
			curr_double_node = (Double_Node*)malloc(sizeof(Double_Node));
			if (curr_double_node == NULL) {
				cout << "ERROR: curr_double_node malloc in Graph constructor failed." << endl;
				exit(1);
			}
			
			/*initialize node*/
			*curr_double_node = Double_Node(node, NULL, NULL);
			
			/*first node, list is empty*/
			if (neighbors_list_head == NULL) {
				neighbors_list_head = curr_double_node;
				
			}

			/*add primary node to list in the double nodes direction*/
			else {
				iter_double_node = neighbors_list_head;
				while (iter_double_node->getDoubleNext() != NULL)
					iter_double_node = iter_double_node->getDoubleNext();
				iter_double_node->setDoubleNext(curr_double_node);
			}
			
			/*building the secondary list which is a list of neighbors for a specific primary node.
			this is a list of simple nodes.*/
			secondary_list_head = curr_double_node; // head of secondary list
			iter_simple_node = secondary_list_head; // iterator
			n++; // count vertices
			bool another_neighbor_exists = true; // flag indicating there're more neighbors to come
			find_start = semicolon + 1; // start position for the "find" function
			while (another_neighbor_exists) {
				size_t comma = s.find(',', find_start);
				size_t curl = s.find('}', find_start);
				
				if (curl < comma) { // no more neighbors
					another_neighbor_exists = false;
					substr_end = curl;
				}
				else {
					substr_end = comma;
				}

				/*capture node value and turn to integer*/
				substr_start = find_start;
				neighbor_str = s.substr(substr_start, substr_end - substr_start);
				neighbor = stoi(neighbor_str);
				
				/*allocation for new secondary node*/
				curr_simple_node = (Simple_Node*)malloc(sizeof(Simple_Node));
				if (curr_simple_node == NULL) {
					cout << "ERROR: curr_simple_node malloc in Graph constructor failed." << endl;
					exit(1);
				}
				
				/*initialize node*/
				*curr_simple_node = Simple_Node(neighbor, NULL);

				/*add secondary node to secondary list, in the simple nodes direction*/
				while (iter_simple_node->getSimpleNext() != NULL)
					iter_simple_node = iter_simple_node->getSimpleNext();
				iter_simple_node->setSimpleNext(curr_simple_node);

				/*start position for the "find" function in the next iteration*/
				find_start = substr_end + 1;
			}
			
		} while (find_start < s_size);

		/*allocate an array indicating if a node was visited before - used in BFS search*/
		num_of_vertices = n;
		visited_array = (bool*)malloc(sizeof(bool) * num_of_vertices);
	}

	// getters
	Double_Node* getNeighborsList() {
		return neighbors_list_head;
	}
	int getNumOfVertices() {
		return num_of_vertices;
	}
	bool* getVisitedArray(){
		return visited_array;
	}
	Queue getQ() {
		return Q;
	}

	// setters
	void setVisitedArray(int position, bool boolean_value) {
		visited_array[position] = boolean_value;
	}

	/*The BFS search function. searches nodes by breadth, their distance from origin*/
	void BFS_Search() {
		for (int i = 1; i <= num_of_vertices; i++) { // for every vertex x in V
			visited_array[i] = false; // visited[x] = false
		}
		cout << getNeighborsList()->getValue() << endl; // print v
		setVisitedArray(getNeighborsList()->getValue(), true); // visited[v] = TRUE
		Q = getQ();
		
		/*Allocate an item to enqueue - a node without its next pointers*/
		Double_Node* item_to_enqueue = (Double_Node*)malloc(sizeof(Double_Node));
		if (item_to_enqueue == NULL)
			exit(1);
		*item_to_enqueue = Double_Node(getNeighborsList()->getValue(), NULL, NULL); // initialize item
		Q.Enqueue(item_to_enqueue); // Enqueue(v,Q)

		while (!Q.Empty()) { // while(not Empty(Q))
			Simple_Node* dequeued_x = Q.Dequeue(); // x = Dequeue(Q)
			
			/*find the primary node that matches the dequeued node*/
			Double_Node* iter = neighbors_list_head;
			while (iter->getDoubleNext() != NULL && iter->getValue() != dequeued_x->getValue())
				iter = iter->getDoubleNext();
			Double_Node* x = iter;
			
			/*iterate over neighbors of x*/
			Simple_Node* y = x->getSimpleNext(); // y is the first neighbor of x
			while (y != NULL) { // for every neighbor y of x
				if (!getVisitedArray()[y->getValue()]) { // if(not visited[y])
					cout << y->getValue() << endl; // print y
					getVisitedArray()[y->getValue()] = true; // visited[y] = true
					
					/*Allocate an item to enqueue - a node without its next pointers*/
					Double_Node* item_to_enqueue = (Double_Node*) malloc(sizeof(Double_Node)); // initialize item
					if (item_to_enqueue == NULL)
						exit(1);
					*item_to_enqueue = Double_Node(y->getValue(), NULL, NULL); // enqueue node without its nexts
					Q.Enqueue(item_to_enqueue); // Enqueue(y,Q)
				}
				y = y->getSimpleNext();
			}
		}
		cout << "BFS_Search() Finished" << endl;
	}
};

int main() {
	/* prompt user with example input string: {1:2,3} {2:1,4,5} {3:1,6,7} {4:2} {5:2} {6:3} {7:3}*/
	cout << "Insert a string that describes that graph" << endl <<
		"Example: {1:2,3} {2:1,4,5} {3:1,6,7} {4:2} {5:2} {6:3} {7:3}" << endl << endl;
	
	/*get string from user, will be parsed to build the graph*/
	string input_string;
	getline(cin, input_string);

	/*initilize a graph instance*/
	Graph graph = Graph(input_string);

	/*start BFS search*/
	graph.BFS_Search();

	return 0;
}