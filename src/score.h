#pragma once

#include <mutex>
#include <memory>

using namespace std;

class Score
{
public:
    
    // Constructor
    Score(){_score = 0;}

    // Design Score always exist alone
    static Score &GetInstance()
    {
        call_once(Score::mOnceFlag, [](){
            mInstance.reset(new Score);
        } );

        return *(mInstance.get());
    }

    // Add Score
    static void AddScore(); 

    // Get Score
    static int GetScore(); 

private:
    static unique_ptr<Score> mInstance;
    static once_flag mOnceFlag;

    static int _score;
};