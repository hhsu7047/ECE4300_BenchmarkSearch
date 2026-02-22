#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
#include <chrono>

using namespace std;

/**
 * @brief returns the first index of elem if it exists, otherwise returns -1
 * iterativeSearch starts at the first index and iterates sequentially to the next until it either
 * finds the element or until it reaches the the end (i.e. element does not exist)
 *
 * @param v : vector of elements
 * @param elem : integer to look for
 * @return int
 */
int iterativeSearch(vector<int>v, int elem){
   // use a for loop where the index, i goes from 0 to the size of v
    for (int i = 0; i < v.size(); i++) {
        
        // inside the for loop, use an if statement to check whether the element at i (e.g. v[i]) equals elem
        if (v[i] == elem) {
            // inside the if statement return i   
            return i;
        }
    }

    // outside of the for loop return -1
    return -1;

}

/**
 * @brief returns the index of elem, if it exists in v. Otherwise it returns -1.
 * binarySearch is recursive (i.e. function calls itself).
 * It utilizes the fact that v is increasing order (e.g. values go up and 
 * duplicates are not allowed).
 *
 * It calculates the mid from the start and end index. It compares v[mid] (i.e. value
 * at mid) with elem and decides whether to search the left half (lower values)
 * or right half (upper values).
 *
 * 
 * @param v : vector of elements
 * @param start : leftmost index (starting value is 0)
 * @param end  :rightmost index (starting value is v.size())
 * @param elem : integer to look for
 * @return int 
 */
int binarySearch(vector<int> & v, int start, int end, int elem){
    // write an if statement that checks the terminating case 
    // inside the if statement return -1
    if (start > end) {
        return -1;
    }
    int mid = start + (end - start) / 2;

    if (v[mid] == elem) {
       
        return mid;
    } else if (v[mid] > elem) {

        end = mid - 1;
    } else {
    
        start = mid + 1;
    }

    // return a recursive call to binarySearch
    return binarySearch(v, start, end, elem);
}

/**
 * @brief updates v to contain the values from filename (leave as is)
 * 
 * It is expected that the files contain values ranging from one to 
 * one hundred million in ascending order (no duplicates).
 * 
 * @param filename : string
 * @param v :vector
 */
void vecGen(string filename, vector<int> & v){
    ifstream file(filename);
    int num;
    v.clear();
    while (file.is_open() && file >> num){
        v.push_back(num);
    }
    file.close();
}

/**
 * @brief writes to file the time it took to search with respect to the
 *  size of the vector, n
 *  Number of Elements (n)	    Time (sec) 
    XXXX	                    X.XXXXX
    XXXX	                    X.XXXXX
 * @param filename (string) : filename (e.g. output_10000_numbers.csv)
 * @param times (vector<double>) : average times 
 * @param n (vector<int>) : sizes of vectors
 */
void writeTimes(string filename, const vector<double> times, const vector<int> n){
    ofstream myFile(filename);

    myFile << "Number of Elements (n)\t Time (sec) " << endl;
    for(int i = 0; i < times.size(); i++){

        myFile << n[i] << "\t" << times[i] << "\n";
    }
    myFile.close();
    cout << "Wrote to " << filename << endl;
}

/**
 * @brief computes the average of the elements in vector, a
 * 
 * @param a  vector of double
 * @return double 
 */
double average(const vector<double> a){
    //if vector is empty..
    if (a.empty()) {
        return 0.0; 
    }

    double sum = 0.0;
    for (double element : a) {
        sum += element;
    }
    return sum / a.size();
}

int main(){
  vector<int> elem_to_find;
    vecGen("test_elem.csv", elem_to_find);
    
    vector<int> file_sizes;
    vecGen("sizes.csv", file_sizes);
    
    vector<int> v; 
    vector<double> times;
    vector<double> avg;
    //linear search
    for(int i = 0; i < file_sizes.size(); i++) {
        string filename = to_string(file_sizes[i]) + "_numbers.csv";
        
        vecGen(filename, v); 
        
        cout << "Running iterative search on: " << filename << endl;
        times.clear(); 
        
        for (int j = 0; j < elem_to_find.size(); j++) {
            auto start_time = chrono::high_resolution_clock::now();
            
            iterativeSearch(v, elem_to_find[j]);
            
            auto end_time = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_time_in_sec = end_time - start_time;
            
            times.push_back(elapsed_time_in_sec.count());
        }
  
        double avg_time = average(times);
        avg.push_back(avg_time);
    }

    writeTimes("iterativeSearch_times.csv", avg, file_sizes); 

    //binary search start
    avg.clear(); //clear for binary search

    for(int i = 0; i < file_sizes.size(); i++) {
        string filename = to_string(file_sizes[i]) + "_numbers.csv";
        
        vecGen(filename, v); 
        
        cout << "Running binary search on: " << filename << endl;
        times.clear(); 
        
        for (int j = 0; j < elem_to_find.size(); j++) {
            auto start_time = chrono::high_resolution_clock::now();
            
            binarySearch(v, 0, v.size() - 1, elem_to_find[j]);
            
            auto end_time = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_time_in_sec = end_time - start_time;
            
            times.push_back(elapsed_time_in_sec.count());
        }
  
        double avg_time = average(times);
        avg.push_back(avg_time);
    }

    writeTimes("binarySearch_times.csv", avg, file_sizes);

    return 0;
}