#ifndef _RATINGSCORE_H_
#define _RATINGSCORE_H_
#include "Serialization.h"
#include <iostream>

class RatingScore : public Serialization
{
private:
    float ratingScore;
    int numberOfRating;

public:
    RatingScore(float ratingScore = 0, int numberOfRating = 0);
    ~RatingScore();
    void read(std::ifstream &ifs) override;
    void write(std::ofstream &ofs) override;
    float getRatingScore();
    int getNumberOfRating();
    void setRatingScore(float _ratingScore);
    void setNumberOfRating(int _numberOfRating);
    void addRatingScore(int ratingScore);
};

#endif