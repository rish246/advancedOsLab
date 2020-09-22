#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <climits>

using namespace std;

void printFrameStats(vector<int> frame){
    for(auto fr : frame){
        cout << setw(5) << fr << ' ';
    }
    cout << endl;
}

void printFinalStats(int numMisses, vector<int> referenceString, string nameScheduler){

    int numHits = referenceString.size() - numMisses;
    string twentyDots(20, '.');

    cout << twentyDots << nameScheduler <<" Scheduler " << twentyDots << endl;
    cout <<"Total misses in cache \t = \t"  << numMisses << endl;
    cout <<"Total hits  in  cache \t = \t"  << numHits << endl; 
    cout <<"Hit ratio of the system  = \t" << (1.0 * numHits / referenceString.size()) << endl;

    cout << endl;
    cout << endl;

}

class FIFO {
    unordered_map<int, bool> isPresentInCache;
    int numMisses = 0, numFrames;
    vector<int> frame;

    public :
        int pageReplacement(vector<int> referenceString, int numFrames){

            frame.resize(numFrames, -1);
            int insertIndex = 0;

            for(int page : referenceString){
                // cout << page << endl;
                // print current situation of frames
                printFrameStats(frame);
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

            return numMisses;
        }

};

class LRU {
    unordered_map<int, int> indexOf, lastOccurenceOf;

    vector<int> frame;

    int numFrames = 0, numMisses = 0;

    int getAppropriateIndex(){
        for(int i=0; i<numFrames; i++){
            if(frame[i] == -1) return i;
        }

        // replace the frame
        int locationLeastUsedPage = INT_MAX;
        int leastUsedPage;
        for(auto occurence : lastOccurenceOf){
            if(occurence.second < locationLeastUsedPage) {
                leastUsedPage = occurence.first;
                locationLeastUsedPage = occurence.second;
            } 
        }

        return indexOf[leastUsedPage] - 1;



    }

    

    public:
        int pageReplacement(vector<int> referenceString, int numFrames){
            this->numFrames = numFrames;

            frame.resize(numFrames, -1);
            for(int i = 0; i < referenceString.size(); i++){
            
                int currPage = referenceString[i];
                // printFrameStats(frame);

                // cout << currPage <<  ' ' << lastOccurenceOf[currPage] << endl;

                if(indexOf[currPage] > 0){ // currentPage is present in cache 
                    lastOccurenceOf[currPage] = i;
                    continue;
                }

                // cout << "This part of code running ..." << endl;
                // 2 =>

                // replacement needs to carry out
                int insertIndex = getAppropriateIndex();
                // cout << insertIndex << endl;
                // 

                // galat ho ra hai
                indexOf[frame[insertIndex]] = -1;
                lastOccurenceOf[frame[insertIndex]] = INT_MAX;

                // insert new page
                frame[insertIndex] = currPage;
                // insertIndex ==> 0
                indexOf[currPage] = (insertIndex + 1);
                lastOccurenceOf[currPage] = i;
                numMisses++;
            }

            return numMisses;
        }

    
};

int main()
{
    vector<int> referenceString = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 3};

    int numFrames = 4;

    vector<int> frame(numFrames, -1);

    FIFO fifo;
    LRU lru;
    

    int numMissesFIFO = fifo.pageReplacement(referenceString, numFrames);
    int numMissesLRU = lru.pageReplacement(referenceString, numFrames);

    printFinalStats(numMissesFIFO, referenceString, "FIFO");
    printFinalStats(numMissesLRU, referenceString, "LRU");
    
}
// run one at a time