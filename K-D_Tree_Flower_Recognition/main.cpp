#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
using namespace std; 
  
const int k = 4;
const int samples = 150;

class Node { 
  public:
    double point[k];
    string species;
    Node *left, *right; 
}; 

// Adds a new node to the tree
Node* newNode(double arr[], string specStr) { 
  Node* temp = new Node; 
  for (int i = 0; i < k; i++) {
    temp -> point[i] = arr[i];
  }
  temp -> species = specStr;
  temp -> left = NULL; 
  temp -> right = NULL;
  return temp; 
} 

// Recursive function for inserting a new node in the tree
Node* insertRecursion(Node *root, double point[], int depth, string specStr) { 
  if (root == NULL){
    return newNode(point, specStr);
  }    
  
  int dimension = depth % k; 

  if (point[dimension] < (root -> point[dimension])){
    root -> left  = insertRecursion(root -> left, point, depth + 1, specStr);
  }   
  else{
    root -> right = insertRecursion(root -> right, point, depth + 1, specStr);
  }

  return root; 
}

string printDotRecursion(Node* root, string &str, string &uniqueID){
  if (root != NULL) { 
    printDotRecursion(root->left, str, uniqueID); 
    if (root->left != NULL && root->right != NULL) {
      str = str + "\t" + root->species + " -> " + root->left->species + ", " + root->right->species + "\n";
    }
    else if(root->left != NULL) {
      str = str + "\t" + root->species + " -> " + root->left->species + ", "+ "test"+ uniqueID + "\n";
      str = str + "\t" + "test" + uniqueID + "[style=invis]\n";
      uniqueID = uniqueID + "a";
    }
    else if(root->right != NULL) {
      str = str + "\t" + root->species + " -> " "test"+ uniqueID + ", " + root->right->species  + "\n";
      str = str + "\t" + "test" + uniqueID + "[style=invis]\n";
      uniqueID = uniqueID + "a";
    }
    printDotRecursion(root->right, str, uniqueID); 
  }
  return str;
}

void print_dot(Node* root){
  ofstream os("output.txt");
  os << "digraph G {" << endl;
  string str = "";
  string uniqueID = "";
  os << printDotRecursion(root, str, uniqueID);
  os << "}";
}

// Kickstarts the recursive insert function
Node* insert(Node *root, double point[], string specStr) { 
  return insertRecursion(root, point, 0, specStr); 
} 

// Recursive function for searching for a node
Node* searchRecursion(Node* root, double point[], int depth) { 
  if (root == NULL) {
    return root;
  }
  bool found = true;
  for (int i = 0; i < k; ++i){
    if (root -> point[i] != point[i]) {
      found = false; 
    } 
  }
  if (found){ return root; }
  int dimension = depth % k; 
  if (point[dimension] < root -> point[dimension]) { 
    return searchRecursion(root -> left, point, depth + 1); 
  }
  return searchRecursion(root -> right, point, depth + 1); 
} 

// Kickstarts the recurssive search function
Node* search(Node* root, double point[]) {  
  return searchRecursion(root, point, 0); 
}

// Finds the distance between two points
double dist(double point1[], double point2[]) {
  double distance = 0;
  for(int i = 0; i < k; i++) {
    distance = distance + (point1[i] - point2[i]) * (point1[i] - point2[i]);
  }
  distance = sqrt(distance);
  return distance;
}

// Compare two nodes to a target and return the closer node
Node closest(Node* root, double point[], Node& temp) {
  if(dist(root -> point, point) < dist(temp.point, point)){
    return *root;
  }
  return temp;
}

int runtime = 0;
// Finds the nearest node to a given input
Node* nearestNeighborRecursion(Node* root, double point[], int depth, Node& best) {
  if (root == NULL) {
    return root;
  }
  bool found = true;
  for (int i = 0; i < k; ++i){
    if (root -> point[i] != point[i]) {
      found = false; 
      runtime++;
    } 
  }
  best = closest(root, point, best);
  if (found){ return root; }
  int dimension = depth % k; 
  if (point[dimension] < root -> point[dimension]) {
    return nearestNeighborRecursion(root -> left, point, depth + 1, best); 
  }
  return nearestNeighborRecursion(root -> right, point, depth + 1, best); 
}

// Kickstarts the recursive NN function
Node* nearestNeighbor(Node* root, double point[], int depth) {
  Node* best = root;
  nearestNeighborRecursion(root, point, depth, *best);
  return best;
}

int main() {
  ifstream infile;
  infile.open("Flower_Classification/iris.txt");

  // sepal_length,sepal_width,petal_length,petal_width,species
  string stringPoints[samples][k+1];
  double points[samples][k];
  string species[samples];

  for(int j = 0; j < samples; j++) {
    string temp;
    getline(infile, temp);
    stringstream stream(temp);
    for (int i = 0; i < k+1; i++) {
      getline(stream, stringPoints[j][i],',');
    }
  }

  for(int j = 0; j < samples; j++) {
    for (int i = 0; i < k; i++) {
      points[j][i] = stod(stringPoints[j][i]);
    }
    species[j] = stringPoints[j][4];
  }

  Node* root = NULL;

  int n = sizeof(points)/sizeof(points[0]); 
  for (int i=0; i<n; i++) {
    root = insert(root, points[i],species[i]);
  }
  string str = "";
  print_dot(root);

  double point1[4];
  string input;
  cout << "Enter sepal length: ";
  cin >> input;
  point1[0] = stod(input);
  
  cout << "Enter sepal width: ";
  cin >> input;
  point1[1] = stod(input);

  cout << "Enter petal length: ";
  cin >> input;
  point1[2] = stod(input);

  cout << "Enter petal width: ";
  cin >> input;
  point1[3] = stod(input);

  // Nearest Neighbor Classification Test #1
  Node* nn = nearestNeighbor(root, point1, 0);
  cout << "Nearest neighbor to point (" << point1[0] << ", " << point1[1] << ", " << point1[2] << ", " << point1[3] << ") is point ("
  << nn -> point[0] << ", " << nn -> point[1] << ", " << nn -> point[2] << ", " << nn -> point[3] << ")" << endl;

  cout << "This flower is probably a " << nn -> species << endl;
}