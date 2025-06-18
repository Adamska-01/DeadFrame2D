#include "Generic/ScoreCounter.h"
#include <Components/UI/TextMesh.h>
#include <Tools/Helpers/Guards.h>


ScoreCounter::ScoreCounter()
	: currentTime(0),
	currentScore(0),
	scoreText(nullptr)
{
}

void ScoreCounter::Init()
{
	Tools::Helpers::GuardAgainstNull(scoreText, "You forgot to set the TextMesh!");
}

void ScoreCounter::Update(float deltaTime)
{
	currentTime += deltaTime;
}

void ScoreCounter::SetTextMesh(TextMesh* textMesh)
{
	this->scoreText = textMesh;
}

void ScoreCounter::AddScore(int scoreToAdd)
{
	currentScore += scoreToAdd;

	scoreText->SetText("Score: " + std::to_string(currentScore));
}

void ScoreCounter::ResetScore()
{
	currentScore = 0;

	scoreText->SetText("Score: 0");
}

void ScoreCounter::ResetTimer()
{
	currentTime = 0.0f;
}

float ScoreCounter::GetCurrentTime()
{
	return currentTime;
}
