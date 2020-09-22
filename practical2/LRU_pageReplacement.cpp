#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <climits>

using namespace std;

vector<int> referenceString = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };

unordered_map<int, int> indexOf, lastOccurence;

int numFrames = 4;

vector<int> frame(numFrames, -1);

void printFrameStats(){
    for(auto fr : frame){
        cout << setw(5) << fr << ' ';
    }
    cout << endl;
}

void printFinalStats(int numMisses){
    int numHits = referenceString.size() - numMisses;

    cout << "Total misses in cache = " <<  numMisses << endl;
    cout << "Total hits  in  cache = " << numHits << endl; 
    cout << "Hit ratio of the system  = " << (1.0 * numHits / referenceString.size()) << endl;
}

/// producing wrong output
int getAppropriateIndex(){
    for(int i=0; i<numFrames; i++){
        if(frame[i] == -1) return i;
    }

    // replace the frame
    int locationLeastUsedPage = INT_MAX;
    int leastUsedPage;
    for(auto occurence : lastOccurence){
        if(occurence.second < locationLeastUsedPage) {
            leastUsedPage = occurence.first;
            locationLeastUsedPage = occurence.second;
        } 
    }

    return indexOf[leastUsedPage] - 1;



}

// leastUsed => 2 =>

int main()
{
    int numMisses = 0;
    for(int i = 0; i < referenceString.size(); i++){
        
        int currPage = referenceString[i];
        printFrameStats();

        // cout << currPage <<  ' ' << lastOccurence[currPage] << endl;

        if(indexOf[currPage] > 0){ // currentPage is present in cache 
            lastOccurence[currPage] = i;
            continue;
        }

        // 2 =>

        // replacement needs to carry out
        int insertIndex = getAppropriateIndex();

        // 

        // galat ho ra hai
        indexOf[frame[insertIndex]] = -1;
        lastOccurence[frame[insertIndex]] = INT_MAX;

        // insert new page
        frame[insertIndex] = currPage;
        // insertIndex ==> 0
        indexOf[currPage] = (insertIndex + 1);
        lastOccurence[currPage] = i;
        numMisses++;
    }

    printFinalStats(numMisses);
}


















































