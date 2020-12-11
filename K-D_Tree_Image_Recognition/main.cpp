/*
@author Peter Finch
*/
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
using namespace std; 
  
const int k = 2;
const int samples = 12;

class Node { 
  public:
    int point[k];
    string name;
    Node *left, *right; 
}; 

/*
Adds a new node to the tree.
@param arr Array holding the data in the point to be added.
@param specStr A string to be associated with the new node created AKA name.
@return The created node.
*/
Node* newNode(int arr[], string specStr) { 
  Node* temp = new Node; 
  for (int i = 0; i < k; i++) {
    temp -> point[i] = arr[i];
  }
  temp -> name = specStr;
  temp -> left = NULL; 
  temp -> right = NULL;
  return temp; 
} 

/*
Recursive function for inserting a new node in the tree.
@param root The root of our tree.
@param point The point being inserted.
@param depth The current depth that the recursive function is at in the tree.
@param specStr A string associated with the point. We use this to store names.
@return The new node after the point is inserted.
*/
Node* insertRecursion(Node *root, int point[], int depth, string specStr) { 
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

/*
Used to kickstart the insertRecursion() function.
@param root The root of our tree.
@param point The point being inserted.
@param specStr A string associated with the point. We use this to store names.
@return The new node after the point is inserted.
*/
Node* insert(Node *root, int point[], string specStr) { 
  return insertRecursion(root, point, 0, specStr);
}
/*
Simple in-order traversal of tree.
@param root The root node of the tree.
*/
void inOrder(Node *root) {
  if (root != NULL) {
    inOrder(root->left);
    cout << root->name << " ";
    inOrder(root->right);
  }
}

/*
Recursive function that searches the tree.
@param root The root node of the tree.
@param point The point we are looking for the nearest neighbor of.
@return Returns the node that matches the input if it exists, or returns NULL.
*/
Node* searchRecursion(Node* root, int point[], int depth) { 
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

/*
Used to kickstart the searchRecursion() function.
@param root The root node of our tree.
@param point The point that we are searching for.
@return The node that has been found to match, or NULL
*/
Node* search(Node* root, int point[]) {  
  return searchRecursion(root, point, 0); 
}

/*
Finds the distance between two points.
@param point1 First point to find distance between.
@param point2 Second point to find distance between.
@return the distance between the two points passed as parameters.
*/
int dist(int point1[], int point2[]) {
  long distance = 0;
  for(int i = 0; i < k; i++) {
    long temp1 = point1[i];
    long temp2 = point2[i];
    distance = distance + (temp1 - temp2) * (temp1 - temp2);
  }
  int distanceInt = sqrt(distance);
  return distanceInt;
}

/*
Compares two nodes to the input and returns the node that is closer to the input.
@param root The first node to be compared.
@param point The point that both nodes are to be compared to.
@param temp The second node to be compared.
@return The node that is closer to the input.
*/
Node closest(Node* root, int point[], Node& temp) {
  if(dist(root -> point, point) < dist(temp.point, point)){
    return *root;
  }
  return temp;
}

/*
Recursive function that searches the tree and finds the node that is closest to the input.
@param root The root node of the tree.
@param point The point we are looking for the nearest neighbor of.
@param best The closest node that we have found so far.
@return Returns the node that matches the input if it exists, or returns NULL.
*/
Node* nearestNeighborRecursion(Node* root, int point[], int depth, Node& best) {
  // If we've reached the bottom of the tree
  if (root == NULL) {
    return root;
  }

  // Variable to keep track of whether we found an exact match of the node we're looking for
  bool found = true;

  // If the current node being examined is not a perfect match, found = false
  for (int i = 0; i < k; ++i){
    if (root -> point[i] != point[i]) {
      found = false; 
    } 
  }
  // Compare our best node so far with the current node being examined to determine which is closer
  best = closest(root, point, best);

  // If we found an exact match, end the search there
  if (found){ return root; }

  // Calculate the dimension that is to be partitioned on this iteration
  int dimension = depth % k;

  // Search through the tree
  if (point[dimension] < root -> point[dimension]) {
    return nearestNeighborRecursion(root -> left, point, depth + 1, best); 
  }
  return nearestNeighborRecursion(root -> right, point, depth + 1, best); 
}

/*
Used to kickstart the nearestNeighborRecursion function.
@param root The root node of our tree.
@param point The point we are looking for the nearest neighbor of.
@return The node that is closest to the point parameter
*/
Node* nearestNeighbor(Node* root, int point[]) {
  Node* best = root;
  int depth = 0;
  nearestNeighborRecursion(root, point, depth, *best);
  return best;
}

/*
Traverses the tree in order to print out every node in DOT file format.
@param root The root node of the K-D Tree.
*/
string printDotRecursion(Node* root, string &str, string &uniqueID){
  if (root != NULL) { 
    printDotRecursion(root->left, str, uniqueID); 
    if (root->left != NULL && root->right != NULL) {
      str = str + "\t" + root->name + " -> " + root->left->name + ", " + root->right->name + "\n";
    }
    else if(root->left != NULL) {
      str = str + "\t" + root->name + " -> " + root->left->name + ", "+ "test"+ uniqueID + "\n";
      str = str + "\t" + "test" + uniqueID + "[style=invis]\n";
      uniqueID = uniqueID + "a";
    }
    else if(root->right != NULL) {
      str = str + "\t" + root->name + " -> " "test"+ uniqueID + ", " + root->right->name  + "\n";
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

/*
Adds together the contents of a vector.
@param vect Input vector to be summed together.
@return The result.
*/
int sum(vector<int> vect) {
  int total = 0;
  for (int i = 0; i < vect.size(); i++) {
    total = total + vect[i];
  }
  return total;
}
//6312605 6324790
//6222954 6596893
/*
Designed to turn a 256x256 vector collected from a .img file into an (x, y) hash.
@param vect 256x256 vector containing only 0's or 255's. See getPoint() below.
@return An array containing the x hash and y hash.
*/
int * hash2d(vector<vector<int>> vect) {
  // Used to store the hashes from each column
  vector<int> y{};
  // Used to store the hashes from each row
  vector<int> x{};

  // Populating y vector 
  for(int row = 0; row < vect.size(); row++){
    int hash = 0;
    for(int column = 0; column < vect[row].size(); column++){
      // Modified checkSum that multiplies based on position. Uses ternary operator to scale down
      hash = hash + ((vect[row][column] == 255)? 1:0) * (column+1);
    }
    y.push_back(hash);
  }
  
  // Populating x vector
  for(int column = 0; column < vect[0].size(); column++){
    int hash = 0;
    for(int row = 0; row < vect.size(); row++){
      // Modified checkSum that multiplies based on position. Uses ternary operator to scale down
      hash = hash + ((vect[row][column] == 255)? 1:0) * (row+1);
    }
    x.push_back(hash);
  }
  
  int hash[] = {sum(x), sum(y)};
  return hash;
}

/*
Used to get data on one specific point in our dataset.
@param fname is the path of the file to be opened.
@return a vector containing the values for each pixel in the .img file.
*/
vector<vector<int>> getPoint(string fname){
  // Open the input stream
  ifstream fs(fname);
  string file_row;
  vector<vector<int>> vect{};
  // Collect all the elements of the .img file and store them in a 2d vector
  while(getline(fs, file_row)){
    istringstream file_row_stream(file_row);
    int file_element;

    vector<int> tempVect{};
    while(file_row_stream >> file_element){
      tempVect.push_back(file_element);
    }
    vect.push_back(tempVect);
  }
  return vect;
}

/*
Gets the data and stores it in the array that is passed by reference.
@param points is how the result is returned since you can't return an array in C++.
*/
void getData(int (&points)[samples][k]){

  // A list of paths to pass to the getPoint() function
  string paths[] = {
    "dataset/Luke/Luke_dataset1.img",
    "dataset/Luke/Luke_dataset2.img",
    "dataset/Luke/Luke_dataset3.img",
    "dataset/Alec/Alec_dataset1.img",
    "dataset/Alec/Alec_dataset2.img",
    "dataset/Alec/Alec_dataset3.img",
    "dataset/Peter/Peter_dataset1.img",
    "dataset/Peter/Peter_dataset2.img",
    "dataset/Peter/Peter_dataset3.img",
    "dataset/Ty/Ty_dataset1.img",
    "dataset/Ty/Ty_dataset2.img",
    "dataset/Ty/Ty_dataset3.img",
  };

  // Iterate through i elements of the dataset and collect the 2d hash
  for(int i = 0; i < samples; i++) {
    vector<vector<int>> vector = getPoint(paths[i]);
    for(int j = 0; j < k; j++){
      points[i][j] = hash2d(vector)[j];
    }
  }
}

/*
Binarizes an img file using local method.
@param vect The 2d input vector of the img file.
@param neighbourhood_size The size of the neighbourhood to use with local.
@return The binarized vector
*/
vector<vector<double>> local(vector<vector<double>> vect, int neighbourhood_size){
  cout << "Binarizing, please wait ..." << endl;
  vector<vector<double>> vect_local{};
  int displacement = neighbourhood_size / 2;
  double min = 255;
  double max = 0;
  for(int x=0; x < vect.size(); x++){
    for(int y=0; y < vect[x].size(); y++){
      if(vect[x][y] < min){
        min = vect[x][y];
      }
      if(vect[x][y] > max){
        max = vect[x][y];
      }
    }
  }
  for(int x=0; x < vect.size(); x++){
    vector<double> vect_temp{};
    for(int y=0; y < vect[x].size(); y++){
      double sum = 0;
      double bad_pixels = 0;
      double standardDeviation = 0;
      vector<double> list{};
      for(int i=-displacement; i<=displacement; i++){
        for(int j=-displacement; j<=displacement; j++){
          if((x+i) < 0){
            bad_pixels ++;
          }
          else if((x+i) > vect.size()-1){
            bad_pixels++;
          }
          else if((y+j) < 0){
            bad_pixels++;
          }
          else if((y+j) > vect[x].size()-1){
            bad_pixels++;
          }
          else{
            sum = sum + vect[x+i][y+j];
            list.push_back(vect[x+i][y+j]);
          }
        }
      }

      double average = sum/(pow(neighbourhood_size,2)-bad_pixels);
      for(int i=0; i<list.size(); i++){
        standardDeviation += pow(list[i] - average, 2);
      }
      standardDeviation = sqrt(standardDeviation / list.size());
      double val;
      double orig = vect[x][y];
      double R = 0.5*(max-min);
      double K = 0.2;
      double A = (standardDeviation)/(R);
      double threshhold = (average)*(1+K*(A - 1));
      if(orig < threshhold){
        val = 0;
      }
      else{
        val = 255;
      }
      vect_temp.push_back(val);
    }
    vect_local.push_back(vect_temp);
  }
  return vect_local;
}


int main() {
  // Collect the path to be used from user
  cout << "Enter image input: ";
  string input_path;
  cin >> input_path;

  // Convert the image file to a .img using any2img.py
  string execute_python = "python any2img.py "+input_path+" input.img";
  system(execute_python.c_str());

  // Initialize the K-D tree
  int points[samples][k];
  getData(points);
  Node* root = NULL;
  string names[] = {"Luke","Luke","Luke","Alec","Alec","Alec","Peter","Peter","Peter","Ty","Ty","Ty"};
  int n = sizeof(points)/sizeof(points[0]); 
  for (int i=0; i<n; i++) {
    root = insert(root, points[i],names[i]);
  }

  // Print the tree
  print_dot(root);

  // Binarize image
  vector<vector<int>> grayscaleInt = getPoint("input.img");
  vector<vector<double>> grayscale {};

  // Convert the vector to double so it may be binarized
  for(int i = 0; i < grayscaleInt.size(); i++){
    vector<double> temp {};
    for(int j = 0; j < grayscaleInt[0].size(); j++){
      double element = grayscaleInt[i][j];
      temp.push_back(element);
    }
    grayscale.push_back(temp);
  }
  vector<vector<double>> vectDouble = local(grayscale, 51);
  vector<vector<int>> vect {};
  // Convert back to int
  for(int i = 0; i < vectDouble.size(); i++){
    vector<int> temp {};
    for(int j = 0; j < vectDouble[0].size(); j++){
      int element = grayscaleInt[i][j];
      temp.push_back(element);
    }
    vect.push_back(temp);
  }

  int point1[] = {hash2d(vect)[0], hash2d(vect)[1]};
  Node* nn = nearestNeighbor(root, point1);

  cout << "\n\nThis picture is probably of " << nn -> name << endl;
  
  return 0;
}