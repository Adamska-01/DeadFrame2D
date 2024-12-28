#include "Stats.h"


Stats::Stats(int score, int lives)
{
	initalScore = currentScore = score;
	initalLives = currentLives = lives;
}

void Stats::Reset()
{
	currentScore = initalScore;
	currentLives = initalLives;
}

void Stats::AddScore(int value)
{
	currentScore += value;
}

void Stats::AddLives(int value)
{
	currentLives += value;
}

int Stats::GetScore() const 
{
	return currentScore;
}

int Stats::GetLives() const
{
	return currentLives;
}