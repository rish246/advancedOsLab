#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

using namespace std;

vector<int> referenceString = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 1, 2, 0};

unordered_map<int, bool> isPresentInCache;

int numFrames = 3;

vector<int> frame(numFrames, -1);

void printFrameStats(){
    for(auto fr : frame){
        cout << setw(5) << fr << ' ';
    }
    cout << endl;
}

int main()
{
    int numMisses = 0;

    int insertIndex = 0;

    for(int page : referenceString){

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

    cout << "Total misses in cache = " <<  numMisses << endl;
    cout << "Total hits  in  cache = " << (referenceString.size() - numMisses) << endl; 


}