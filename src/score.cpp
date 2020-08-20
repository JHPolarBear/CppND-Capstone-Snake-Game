#include "score.h"

unique_ptr<Score> Score::mInstance;
once_flag Score::mOnceFlag;
int Score::_score;

// Add Score
void Score::AddScore()
{
    _score++;
}

// Get Score
int Score::GetScore()
{
    return _score;
}