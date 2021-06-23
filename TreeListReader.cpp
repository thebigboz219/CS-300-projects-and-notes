// TreeListReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Course {     //initialize course object
    string courseID;        //unique identifier
    string courseName;
    string coursePreReq1;
    string coursePreReq2;
    Course() {
        courseName = "none";
    }
};

struct Node {       //initialization of node structure
    Course course;

    Node* leftPtr;
    Node* rightPtr;

    Node() {
        leftPtr = nullptr;
        rightPtr = nullptr;
    }

    Node(Course startCourse) : Node() { //structure for creating a node with course info
        this->course = startCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseID);
};

BinarySearchTree::BinarySearchTree() {  //initialization of root node
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree() {

}

void BinarySearchTree::InOrder() {  //begins recursive call to print in alphanumeric order
    inOrder(root);
}

void BinarySearchTree::Insert(Course course) {  //inserts course into tree 
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

Course BinarySearchTree::Search(string courseID) {
    string formatID = courseID;         //Formats user input to match any potential canidates
    for (int i = 0; i < 4; i++) {
        formatID[i] = toupper(formatID[i]);
    }

    Node* current = root;
    while (current != nullptr) {        //Loops through tree searching for given course ID
        if (current->course.courseID == formatID) {
            return current->course;
        }
        if (current->course.courseID < formatID) {
            current = current->rightPtr;
        }
        else {
            current = current->leftPtr;
        }
    }

    Course course;
    return course;
}

void BinarySearchTree::addNode(Node* node, Course course) { //Creates new node with given course information
    if (node->course.courseID > course.courseID) {
        if (node->leftPtr == nullptr) {
            node->leftPtr = new Node(course);
        }
        else {
            this->addNode(node->leftPtr, course);
        }
    }
    else {
        if (node->rightPtr == nullptr) {
            node->rightPtr = new Node(course);
        }
        else {
            this->addNode(node->rightPtr, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {        //Recursive call to move through and print course information
    if (node != nullptr) {
        inOrder(node->leftPtr);
        cout << node->course.courseID << ", " << node->course.courseName << endl;
        inOrder(node->rightPtr);
    }
}

void loadCourses(string fileName, BinarySearchTree* bst) {
    ifstream inFS;
    string currentLine;
    string substr;
    int numCommas;
    vector<string> result;

    cout << "loading file " << fileName << endl;        //Attempts to open file
    inFS.open(fileName);

    if (!inFS.is_open()) {
        cout << "Could not open file" << fileName << endl;      //Checks to see if file successfully opened
        return;
    }

    while (!inFS.eof()) {
        numCommas = 0;
        getline(inFS, currentLine);

        if (!inFS.fail()) {

            for (int i = 0; i < currentLine.size(); i++) {         //Reads for commas in the line read from the file
                if (currentLine.at(i) == ',') {
                    numCommas++;
                }

            }

            if (numCommas < 1) {        //Error checking incase of missing information
                cout << "Missing core data in course line, moving to next course" << endl;
                continue;
            }
            else if (numCommas == 1) {
                Course currentCourse;
                stringstream s_stream(currentLine);

                while (s_stream.good()) {           //Seperates out the information for data storage
                    getline(s_stream, substr, ',');
                    result.push_back(substr);
                }

                currentCourse.courseID = result.at(0);
                currentCourse.courseName = result.at(1);        //Stores information in a course object and inserts it into the tree
                bst->Insert(currentCourse);
                result.clear();
            }
            else if (numCommas == 2) {
                Course currentCourse;
                stringstream s_stream(currentLine);

                while (s_stream.good()) {
                    getline(s_stream, substr, ',');
                    result.push_back(substr);
                }

                currentCourse.courseID = result.at(0);
                currentCourse.courseName = result.at(1);
                currentCourse.coursePreReq1 = result.at(2);
                currentCourse.coursePreReq2 = "";
                bst->Insert(currentCourse);
                result.clear();
            }
            else if (numCommas == 3) {
                Course currentCourse;
                stringstream s_stream(currentLine);

                while (s_stream.good()) {
                    getline(s_stream, substr, ',');
                    result.push_back(substr);
                }

                currentCourse.courseID = result.at(0);
                currentCourse.courseName = result.at(1);
                currentCourse.coursePreReq1 = result.at(2);
                currentCourse.coursePreReq2 = result.at(3);
                bst->Insert(currentCourse);
                result.clear();
            }
        }
    }

    inFS.close();
}

void DisplayMenu() {        //Constant call to display menu when needed
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "4. Exit" << endl;
    cout << endl;
}

int main()
{
    bool endProgram = false;
    char userChoice;
    BinarySearchTree* bst = new BinarySearchTree();
    Course course;
    string userSearch;
    string userFile;

    cout << "Welcome to the course planner." << endl;

    while (endProgram != true) {

        DisplayMenu();
        cout << "What would you like to do?" << endl;
        cin >> userChoice;

        switch (userChoice) {
        case '1':
            cout << "What is the name of the file to read?" << endl;
            cin >> userFile;
            loadCourses(userFile, bst);
            break;

        case '2':
            cout << "Here is a sample schedule:" << endl;
            cout << endl;
            bst->InOrder();
            break;

        case '3':
            cout << "What course do you want to know about?" << endl;
            cin >> userSearch;
            course = bst->Search(userSearch);

            if (course.courseName == "none") {
                cout << "Course not found" << endl;
                break;
            }
            else {
                cout << course.courseID << ", " << course.courseName << endl;
                cout << "Prerequisites: " << course.coursePreReq1 << ", " << course.coursePreReq2 << endl;
            }
            break;

        case '4':
            cout << "Thank you for using the course planner!" << endl;
            endProgram = true;
            break;

        default:
            cout << userChoice << " is not a valid option." << endl;
            cin.clear();
            break;
        }
    }
}

