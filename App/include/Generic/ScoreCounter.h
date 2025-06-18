#pragma once


class TextMesh;


class ScoreCounter
{
private:
	float currentTime;

	int currentScore;

	TextMesh* scoreText;


public:
	ScoreCounter();


	void Init();

	void Update(float deltaTime);


	void SetTextMesh(TextMesh* textMesh);

	void AddScore(int scoreToAdd);

	void ResetScore();

	void ResetTimer();

	float GetCurrentTime();
};