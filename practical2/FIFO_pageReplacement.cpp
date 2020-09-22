#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

using namespace std;

vector<int> referenceString = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 3};

unordered_map<int, bool> isPresentInCache;

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

int main()
{
    int numMisses = 0;

    int insertIndex = 0;

    for(int page : referenceString){
        // cout << page << endl;
        // print current situation of frames
        printFrameStats();
        if(!isPresentInCache[page]){

            if(frame[insertIndex] != -1){
                // replace the existing from cache
                isPresentInCache[frame[insertIndex]] = false;
            }

            // insert into cache
            frame[insertIndex] = page;
            
            

            isPresentInCache[page] = true;
            // remove the entry from cache
            insertIndex = (insertIndex + 1) % numFrames;

            numMisses++;


        }
    }

    
    printFinalStats(numMisses);

}