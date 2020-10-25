#include <iostream>
#include <vector>
#include <thread>

using namespace std;

vector<int> nums;

int main()
{
    int try_catch_test = 5;

    try
    {
        /* code */
        if (nums.size() < 5)
            throw "Array too small";
    }
    catch (const char *err)
    {
        std::cerr << err << endl;
    }
}