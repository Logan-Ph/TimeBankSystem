#include "RatingScore.h"
#include "Functions.h"

RatingScore::~RatingScore() = default;

RatingScore::RatingScore(float ratingScore, int numberOfRating) : ratingScore(ratingScore), numberOfRating(numberOfRating){};

float RatingScore::getRatingScore()
{
    return ratingScore;
}

int RatingScore::getNumberOfRating()
{
    return numberOfRating;
}

void RatingScore::setRatingScore(float _ratingScore)
{
    ratingScore = _ratingScore;
}

void RatingScore::setNumberOfRating(int _numberOfRating)
{
    numberOfRating = _numberOfRating;
}

void RatingScore::addRatingScore(int ratingScore)
{
    this->ratingScore = ratingScore / (numberOfRating + 1) + this->ratingScore * numberOfRating / (numberOfRating + 1);
    numberOfRating++;
}

void RatingScore::read(std::ifstream &ifs)
{
    setRatingScore(readData<float>(ifs));  // read rating score from file
    setNumberOfRating(readData<int>(ifs)); // read number of rating score from file
};

void RatingScore::write(std::ofstream &ofs)
{
    writeData<float>(ofs, ratingScore);  // write rating score into file
    writeData<int>(ofs, numberOfRating); // write number of rating score into file
};
