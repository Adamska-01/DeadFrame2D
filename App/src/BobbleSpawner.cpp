#include <Bobble.h>
#include <BobbleSpawner.h>
#include <SubSystems/AudioManager.h>
#include <Math/Vector2.h>
#include <Utility/UsefullFunctions.h>


//TIMERS
unsigned int SpawnerTimer(unsigned int interval, void* param)
{
	static_cast<BobbleSpawner*>(param)->Spawn();
	return interval; // repeat the timer
}

unsigned int SpeedTimer(unsigned int interval, void* param)
{
	static_cast<BobbleSpawner*>(param)->IncreaseSpeed();
	return interval; // repeat the timer
}


BobbleSpawner::BobbleSpawner(int startX)
	: startX(startX)
{
	speedMultiplier = 1.2f;

	//Init score and lives 
	stats = new Stats(0, 3);
	
	//Speed
	bobbleSpeed = 100.0f;
	
	//Player's Scores
	Score = 100;
	PenaltyScore = -50;

	//Timers
	timerID = SDL_AddTimer(3.0f * 1000, SpawnerTimer, this);
	timerID2 = SDL_AddTimer(120 * 1000, SpeedTimer, this);
}

BobbleSpawner::~BobbleSpawner()
{
	Clean();
}

void BobbleSpawner::Update(float dt)
{
	//Updates every bobble and checks if any can be deleted
	for (auto i = 0; i < bobbles.size(); i++)
	{
		bobbles[i]->Update(dt);

		Blow(i);
	}
}

void BobbleSpawner::Draw()
{
	for (auto i = 0; i < bobbles.size(); i++)
	{
		bobbles[i]->Draw();
	}
}

void BobbleSpawner::Clean()
{
	RemoveTimers();

	for (auto i = 0; i < bobbles.size(); i++)
	{
		delete bobbles[i];
		bobbles[i] = nullptr;
	}

	bobbles.clear();
}

void BobbleSpawner::CollisionsWall(SDL_Rect wall, bool lavaWall)
{
	for (int i = 0; i < bobbles.size(); i++)
	{
		// Baunce
		if (!bobbles[i]->CollisionsWithWall(wall))
			continue;

		if(lavaWall && bobbles[i]->IsFiring()) 
		{	// If lava
			bobbles[i]->StopFire();
			bobbles[i]->SetDestruction(true);

			//Update stats
			if (stats->GetLives() > 0)
			{
				stats->AddScore(PenaltyScore);
				stats->AddLives(-1);
			}

			AudioManager::GetInstance()->PlaySFX(0, 0, 0);
		}
		else if (!lavaWall)
		{
			AudioManager::GetInstance()->PlaySFX(2, 0, 0);
		}
	}
}

void BobbleSpawner::CollisionsBobble(Bobble* bobble)
{
	if (bobble == nullptr || !bobble->IsFiring())
		return;

	for (auto i = 0; i < bobbles.size(); i++)
	{
		// If there is a collision and the colour doesn't match
		if (bobbles[i]->CollisionsWithBall(bobble->GetCircle()) && bobbles[i]->GetColor() != bobble->GetColor() && !bobbles[i]->CanExplode())
		{
			bobble->StopFire();
			bobble->SetDestruction(true);

			if (stats->GetLives() > 0) stats->AddScore(PenaltyScore);
			{
				AudioManager::GetInstance()->PlaySFX(0, 0, 0);
			}
		}
		// Same but the colour does match
		else if (bobbles[i]->CollisionsWithBall(bobble->GetCircle()) && bobbles[i]->GetColor() == bobble->GetColor() && !bobbles[i]->CanExplode())
		{
			bobbles[i]->StopFire();
			bobbles[i]->SetDestruction(true);
			
			bobble->StopFire();
			bobble->SetDestruction(true);

			if (stats->GetLives() > 0) stats->AddScore(Score);
			{
				AudioManager::GetInstance()->PlaySFX(0, 0, 0);
			}
		}
	}
}

void BobbleSpawner::Spawn()
{
	// Instantiate bobble
	Vector2 pos(startX, 0);
	bobbles.push_back(new Bobble(pos));

	// Set Velocity
	Vector2 vel(UsefullFunctions::RandomFloat(-1, 1), UsefullFunctions::RandomFloat(0.2f, 1));
	bobbles.back()->SetVelocity(vel.Normalize());

	// Set speed
	bobbles.back()->SetSpeed(bobbleSpeed);

	//Start motion
	bobbles.back()->Fire();

	AudioManager::GetInstance()->PlaySFX(1, 0, 4);
}

void BobbleSpawner::Blow(int index)
{
	//Destroys the bobble if it can be destroyed
	if (bobbles[index] == nullptr || !bobbles[index]->CanDestroy())
		return;

	delete bobbles[index];
	bobbles[index] = nullptr;

	bobbles.erase(bobbles.begin() + index);
}

void BobbleSpawner::IncreaseSpeed()
{
	bobbleSpeed *= speedMultiplier;
}

void BobbleSpawner::RemoveTimers()
{
	SDL_RemoveTimer(timerID);
	SDL_RemoveTimer(timerID2);
}