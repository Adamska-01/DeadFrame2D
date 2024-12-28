#pragma once


class Stats
{
private:
	int initalScore;

	int initalLives;

	int currentScore;

	int currentLives;


public:
	Stats(int score, int lives);


	void Reset();

	void AddScore(int value);

	void AddLives(int value);

	int GetScore() const;

	int GetLives() const;
};