///** Thong Nguyen **
///** Searching Algorithm for Wolf-Goat-Cabbage Puzzle **

#include<iostream>
#include<vector>
#include<string>
using namespace std;

class State {
public:
	int wolf;	// 0: shore A, 1: shore B. Same for other variables
	int goat;
	int cabbage;
	int peasant;

	string action[2];	//action[0]: what is on the boat
						//action[1]: direction of the boat
	State * parent;
	vector<State*> children;

	State(int wolf, int goat, int cabbage, int peasant, string object, string action) {
		this->wolf = wolf;
		this->goat = goat;
		this->cabbage = cabbage;
		this->peasant = peasant;
		this->action[0] = object;
		this->action[1] = action;
		this->parent = nullptr;
	}

	void addChild(State* child) {
		children.push_back(child);
		child->parent = this;
	}

	State* getChild(int index) {
		if (index < children.size())
			return children[index];
		return nullptr;
	}
};

//Function Prototypes
State * generateAllStates(void);
string search(State *);
bool checkGoal(State*);
string buildPath(State*);
void cleanup(State*&);



int main() {
	cout << "** Thong Nguyen" << endl;
	cout << "** Searching Algorithm - Wolf, Goat, Cabbage Puzzle\n\n" << endl;
	State * root = generateAllStates();	// Generate all the possible states into a tree structure.
	string result = search(root);		// Call the searching algorithm.
	cout << result << endl;				// Display the result
	cleanup(root);						// Cleanup allocated data

	return 0;
}

// I'm using DFS (LIFO, Tree structure).
string search(State * root) {
	vector<State*> frontier;
	frontier.push_back(root);

	while (true) {
		if (frontier.empty()) return "The solution does not exist!";

		State* state = frontier.back();
		frontier.pop_back();

		//cout << state->action[0] + " from " + state->action[1] << endl;
		if (checkGoal(state)) {	//If the goal state is found
			cout << "Solution found!" << endl;
			return buildPath(state);
		}
		
		for (State * child : state->children)
			frontier.push_back(child);
	}
}

bool checkGoal(State* state) {
	bool check = false;
	if (state->wolf == 1 && state->goat == 1 && state->cabbage == 1 && state->peasant == 1)
		check = true;
	return check;
}

string buildPath(State * state) {
	if (state->parent == nullptr)
		return "\n";
	string action = state->action[0] + " from " + state->action[1] + ".\n\n";
	return buildPath(state->parent) + action;
}

State * generateAllStates() {
	string toB = "shore A to shore B";
	string toA = "shore B to shore A";
	State * root = new State{ 0,0,0,0,"", "" };
	//Layer 1: Peasant = 1
	root->addChild(new State{ 1,0,0,1, "wolf", toB });
	root->addChild(new State{ 0,1,0,1, "goat", toB });
	root->addChild(new State{ 0,0,1,1, "cabbage", toB });
	root->addChild(new State{ 0,0,0,1, "nothing", toB });

	//Layer 2: peasant = 0
	State * child11 = root->getChild(1);
	child11->addChild(new State{ 0,1,0,0, "nothing", toA });

	//Layer 3: peasant = 1
	State * child20 = child11->getChild(0);
	child20->addChild(new State{ 1,1,0,1,"wolf", toB });
	child20->addChild(new State{ 0,1,1,1,"cabbage", toB });

	//Layer 4: peasant = 0
	State * child30 = child20->getChild(0);
	child30->addChild(new State{ 1,1,0,0,"nothing",toA });
	child30->addChild(new State{ 1,0,0,0,"goat", toA });

	State * child31 = child20->getChild(1);
	child31->addChild(new State{ 0,1,1,0, "nothing", toA });
	child31->addChild(new State{ 0,0,1,0, "goat", toA });

	//Layer 5: peasant = 1
	State * child41 = child30->getChild(1);
	child41->addChild(new State{ 1,0,0,1,"nothing", toB });
	child41->addChild(new State{ 1,0,1,1,"cabbage", toB });

	State * child43 = child31->getChild(1);
	child43->addChild(new State{ 1,0,1,1,"wolf",toB });

	//Layer 6: peasant = 0
	State * child51 = child41->getChild(1);
	child51->addChild(new State{ 0,0,1,0,"wolf",toA });
	child51->addChild(new State{ 1,0,1,0,"nothing",toA });

	State * child52 = child43->getChild(0);
	child52->addChild(new State{ 1,0,1,0,"nothing", toA });

	//Layer 7: peasant = 1
	State * child61 = child51->getChild(1);
	child61->addChild(new State{ 1,1,1,1,"goat", toB });
	State * child62 = child52->getChild(0);
	child62->addChild(new State{ 1,1,1,1,"goat", toB });

	return root;
}

void cleanup(State*& root) {
	if (root == nullptr) {
		return;
	}
	for (State * child : root->children)
		cleanup(child);
	delete root;
	root = nullptr;
	return;
}