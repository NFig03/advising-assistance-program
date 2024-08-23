#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

using namespace std;

//global variable holds path to csv file
string csvPath;

//structure to store course information
struct course
{
    string courseNumber;
    string name;
    vector<string> prerequisites;

    //adds a prerequisite to the course's list of prerequisites
    void addPrereq(string preReq)
    {
        prerequisites.push_back(preReq);
    }

    course() : courseNumber(""), name(""), prerequisites() {}
};

//Node structure which holds a course object in the tree
struct Node
{
    course nodeCourse; //course associated with node
    Node* left; //pointer to left child
    Node* right; //pointer to right child

    //Default node constructor initializes pointers to null
    Node()
    {
        left = nullptr;
        right = nullptr;
    }

    //Node constructor with course input parameter
    Node(course aCourse):Node()
    {
        nodeCourse = aCourse;
    }
};

//BinarySearchTree class
class BinarySearchTree
{
    private:
        
        //adds nodes to the tree
        void addNode(Node* node, course course);
    public:
        Node* root;
        
        //default constructor
        BinarySearchTree();
        
        //Inserts a course into a node
        void Insert(course course);
        
        //finds a specific course by course number
        void searchCourse(string courseNumber);

};

//default constructor with root initialized null
BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

//Inserts a specified course into a node
void BinarySearchTree::Insert(course course)
{
    //inserts the course into the root node if null, otherwise adds it with addNode function
    if (root == nullptr)
     {
         root = new Node(course);
     }
     else
     {
         this->addNode(root, course);
     }
}

//adds a specific node to the tree with the specified course
void BinarySearchTree::addNode(Node* node, course course)
{
    //finds the proper insertion location for the node based on course number
    if (node->nodeCourse.courseNumber > course.courseNumber)
    {
        if (node -> left == nullptr)
        {
            node->left = new Node(course);
        }
        else
        {
            addNode(node->left, course);
        }
    }
    else
    {
        if (node->right == nullptr)
        {
            node->right = new Node(course);
        }
        else
        {
            addNode(node->right, course);
        }
    }
}

//finds the specified course based on course number
void BinarySearchTree::searchCourse(string courseNumber)
{
    // traverses the tree to compare course numbers with the provided course number
    Node* current = root;
    while (current != nullptr)
    {
        if (current->nodeCourse.courseNumber.compare(courseNumber) == 0)
        {
            cout << current->nodeCourse.courseNumber << ", " << current->nodeCourse.name << endl;
            if (current->nodeCourse.prerequisites.size() != 0)
            {
                for (int i = 0; i < current->nodeCourse.prerequisites.size(); i++)
                {
                    cout << "Prerequisites: " << current->nodeCourse.prerequisites[i];
                }
            }
            cout << "\n";
            return;
        }
        else if (current->nodeCourse.courseNumber.compare(courseNumber) < 0)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
}

//traverses the tree from left to right (lowest course number to greatest)
void inOrderTraversal (Node* node)
{
    //returns null if the node is empty, otherwise recursively calls itself to traverse from left to right
    //traversal left to right prints the courses from lowest to highest course number
    if (node == nullptr)
    {
        return;
    }
    inOrderTraversal(node->left);
    cout << node->nodeCourse.courseNumber << ", " << node->nodeCourse.name << endl;
    inOrderTraversal(node->right);
}

//loads the courses into the specified tree from the specified file
void loadCourses(string& csvPath, BinarySearchTree* tree)
{
    ifstream inputFile(csvPath);

    //checks to see if file is open
    if (!inputFile.is_open())
    {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    string line;

    vector<string> numbers;

    //while there are lines to read...
    while (getline(inputFile, line))
    {
        bool found = false;

        //checks to make sure every course has the proper amount of parameters
        if (count(line.begin(), line.end(), ',') < 2)
        {
            cout << "There is a missing parameter for a course." << endl;
            exit(EXIT_FAILURE);
        }

        course c;
        istringstream stream(line);
        getline(stream, c.courseNumber, ',');
        numbers.push_back(c.courseNumber);
        getline(stream, c.name, ',');
        
        //adds the prerequisites to the vector, then adds the course to the tree
        while (getline(stream, line))
        {
            found = false;
            c.addPrereq(line);
            tree->Insert(c);

        }
        
        //breaks the loop if end of file reached
        if (inputFile.eof() == true)
        {
            break;
        }
    }
    //closes input file
    inputFile.close();
}

//prompts the menu with the screen
void printMenu()
{
    cout << "\n\t1. Load Data Structure." << endl;
    cout << "\t2. Print Course List." << endl;
    cout << "\t3. Print Course." << endl;
    cout << "\t9. Exit" << endl;
    cout << "\nWhat would you like to do? ";
}

//uses a switch to allow functionality of the menu
void menuFunctionality(BinarySearchTree* tree)
{
    string searchId;
    cout << "Welcome to the course planner." << endl;
    int input = 0;
    bool loaded = false;
    while (input != 9)
    {
        printMenu();
        cin >> input;

        //switch allows for the functionality of the menu based on user input
        switch (input)
        {
            //Prompts user for file name and calls load courses function with provided file name
            case 1:
                cin.ignore();
                cout << "\nPlease enter the file name to load: " << endl;
                getline(cin, csvPath);
                loadCourses(csvPath, tree);
                cout << "\n" << csvPath << " has been loaded." << endl;
                loaded = true;
                break;

            //will not work if the file has not been read, otherwise prints the contents in order
            case 2:
                if (loaded == false)
                {
                    cout << "\nThe data structure must be loaded first." << endl;
                    break;
                }
                else
                {
                    cout << "\nHere is a sample schedule: " << endl;
                    cout << "\n" << endl;
                    inOrderTraversal(tree->root);
                    break;
                }
            case 3:

                //will not work if the file has not been read, otherwise allows user to search for a specific course with course number
                if (loaded == false)
                {
                    cout << "\nThe data structure must be loaded first." << endl;
                    break;
                }
                else
                {
                    cin.ignore();
                    cout << "\nWhat course do you want to know about? (Case sensitive) ";
                    cin >> searchId;
                    cout << "\n";
                    tree->searchCourse(searchId);
                    break;
                }
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << input << " is not a valid option." << endl;
        }
    }
}

// main function
int main()
{
    BinarySearchTree tree;
    menuFunctionality(&tree);
}

//file name is stored below for easy copy and paste debugging
//CS 300 ABCU_Advising_Program_Input.csv
