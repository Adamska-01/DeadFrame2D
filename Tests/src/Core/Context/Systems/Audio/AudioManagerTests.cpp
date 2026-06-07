#include <doctest.h>
#include "Core/Context/Systems/Audio/AudioManager.h"
#include "Mocks/Context/Systems/Audio/MockAudioBackend.h"


using namespace DF2D::Core;
using namespace DF2D::Models;


static std::unique_ptr<AudioManager> MakeManager(AudioConfig config, MockAudioBackend*& outMock)
{
	auto mock = std::make_unique<MockAudioBackend>();
	outMock = mock.get();

	return std::make_unique<AudioManager>(config, std::move(mock));
}


TEST_SUITE_BEGIN("AudioManager");


TEST_CASE("Constructor stores audio config values")
{
	auto config = AudioConfig
	{
		.musicVolume = 0.75f,
		.sfxVolume = 0.25f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	CHECK(manager->GetMasterVolume() == doctest::Approx(0.5f));
	CHECK(manager->GetMusicVolume() == doctest::Approx(0.75f));
	CHECK(manager->GetSFXVolume() == doctest::Approx(0.25f));
}


TEST_CASE("Volume values are clamped to [0, 1]")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->SetMasterVolume(-0.5f);
	CHECK(manager->GetMasterVolume() == doctest::Approx(0.0f));

	manager->SetMasterVolume(1.5f);
	CHECK(manager->GetMasterVolume() == doctest::Approx(1.0f));

	manager->SetMusicVolume(-1.0f);
	CHECK(manager->GetMusicVolume() == doctest::Approx(0.0f));

	manager->SetMusicVolume(2.0f);
	CHECK(manager->GetMusicVolume() == doctest::Approx(1.0f));

	manager->SetSFXVolume(-0.1f, -1);
	CHECK(manager->GetSFXVolume() == doctest::Approx(0.0f));

	manager->SetSFXVolume(1.1f, -1);
	CHECK(manager->GetSFXVolume() == doctest::Approx(1.0f));
}


TEST_CASE("SetMusicVolume scales by masterVolume when forwarding to backend")
{
	auto config = AudioConfig
	{
		.musicVolume = 1.0f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->SetMusicVolume(0.8f);

	CHECK(mock->lastMusicVolume == doctest::Approx(0.5f * 0.8f));
	CHECK(manager->GetMusicVolume() == doctest::Approx(0.8f));
}


TEST_CASE("SetSFXVolume scales by masterVolume when forwarding to backend")
{
	auto config = AudioConfig
	{
		.sfxVolume = 1.0f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->SetSFXVolume(0.6f, -1);

	CHECK(mock->lastSFXVolume == doctest::Approx(0.3f)); // 0.5 * 0.6
	CHECK(manager->GetSFXVolume() == doctest::Approx(0.6f));
}


TEST_CASE("SetMasterVolume propagates scaled volumes to backend")
{
	auto config = AudioConfig
	{
		.musicVolume = 0.8f,
		.sfxVolume = 0.5f,
		.masterVolume = 1.0f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->SetMasterVolume(0.5f);

	CHECK(mock->lastMusicVolume == doctest::Approx(0.4f)); // 0.5 * 0.8
	CHECK(mock->lastSFXVolume == doctest::Approx(0.25f));  // 0.5 * 0.5
	CHECK(manager->GetMasterVolume() == doctest::Approx(0.5f));
}


TEST_CASE("LoadMusic caches by file path")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id1 = manager->LoadMusic("song.ogg");
	auto id2 = manager->LoadMusic("song.ogg");

	CHECK(id1 == id2);
	CHECK(mock->loadMusicCount == 1);
}


TEST_CASE("LoadSFX caches by file path")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id1 = manager->LoadSFX("sfx.wav");
	auto id2 = manager->LoadSFX("sfx.wav");

	CHECK(id1 == id2);
	CHECK(mock->loadSFXCount == 1);
}


TEST_CASE("LoadMusic returns 0 when backend returns 0")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	mock->nextMusicId = 0; // Backend fails to load

	auto id = manager->LoadMusic("missing.ogg");

	CHECK(id == 0);
}


TEST_CASE("LoadSFX returns 0 when backend returns 0")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	mock->nextSfxId = 0; // Backend fails to load

	auto id = manager->LoadSFX("missing.wav");

	CHECK(id == 0);
}


TEST_CASE("PlayMusics sets volume then plays via backend")
{
	auto config = AudioConfig
	{
		.musicVolume = 0.8f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id = manager->LoadMusic("song.ogg");
	auto result = manager->PlayMusics(id, 0);

	CHECK(result == true);
	CHECK(mock->lastMusicVolume == doctest::Approx(0.4f)); // 0.5 * 0.8
	CHECK(mock->lastPlayedMusic == id);
	CHECK(mock->playMusicCount == 1);
}


TEST_CASE("PlayMusics with invalid id (0) returns false and does not call backend")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto result = manager->PlayMusics(0, 0);

	CHECK(result == false);
	CHECK(mock->playMusicCount == 0);
}


TEST_CASE("PlaySFX sets channel volume after playing via backend")
{
	auto config = AudioConfig
	{
		.sfxVolume = 0.6f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id = manager->LoadSFX("sfx.wav");
	auto channel = manager->PlaySFX(id, 0);

	CHECK(channel >= 0);
	CHECK(mock->lastPlayedSFX == id);
	CHECK(mock->lastSFXVolume == doctest::Approx(0.3f)); // 0.5 * 0.6
	CHECK(mock->playChannelCount == 1);
}


TEST_CASE("PlaySFX with invalid id (0) returns -1 and does not call backend")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto channel = manager->PlaySFX(0, 0);

	CHECK(channel == -1);
	CHECK(mock->playChannelCount == 0);
}


TEST_CASE("Stop/Pause/Resume music delegates to backend")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->StopMusic();
	CHECK(mock->lastStoppedMusicCount == 1);

	manager->PauseMusic();
	CHECK(mock->lastPausedMusicCount == 1);

	manager->ResumeMusic();
	CHECK(mock->lastResumedMusicCount == 1);
}


TEST_CASE("StopChannel/PauseChannel/ResumeChannel delegates to backend")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->StopChannel(3);
	CHECK(mock->lastStoppedChannel == 3);

	manager->PauseChannel(5);
	CHECK(mock->lastPausedChannel == 5);

	manager->ResumeChannel(7);
	CHECK(mock->lastResumedChannel == 7);
}


TEST_CASE("Different file paths produce different cache entries")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id1 = manager->LoadMusic("song1.ogg");
	auto id2 = manager->LoadMusic("song2.ogg");

	CHECK(id1 != id2);
	CHECK(mock->loadMusicCount == 2);
}


TEST_CASE("Different SFX file paths produce different cache entries")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id1 = manager->LoadSFX("sfx1.wav");
	auto id2 = manager->LoadSFX("sfx2.wav");

	CHECK(id1 != id2);
	CHECK(mock->loadSFXCount == 2);
}


TEST_CASE("LoadMusic with backend returning 0 is returned and not cached")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	mock->nextMusicId = 0; // Backend fails

	auto id1 = manager->LoadMusic("song.ogg");
	CHECK(id1 == 0);
	CHECK(mock->loadMusicCount == 1);

	mock->nextMusicId = 42; // Backend succeeds on retry

	auto id2 = manager->LoadMusic("song.ogg");
	CHECK(id2 == 42); // Not cached — re-attempted
	CHECK(mock->loadMusicCount == 2);
}


TEST_CASE("LoadSFX with backend returning 0 is not cached")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	mock->nextSfxId = 0; // Backend fails

	auto id1 = manager->LoadSFX("sfx.wav");

	CHECK(id1 == 0);
	CHECK(mock->loadSFXCount == 1);

	mock->nextSfxId = 99; // Backend succeeds on retry

	auto id2 = manager->LoadSFX("sfx.wav");

	CHECK(id2 == 99); // Not cached — re-attempted
	CHECK(mock->loadSFXCount == 2);
}


TEST_CASE("SetSFXVolume with specific channel does not update global sfxVolume")
{
	auto config = AudioConfig
	{
		.sfxVolume = 0.8f,
		.masterVolume = 0.5f
	};

	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	manager->SetSFXVolume(0.6f, 3);

	CHECK(manager->GetSFXVolume() == doctest::Approx(0.8f)); // Global unchanged
	CHECK(mock->lastSFXVolume == doctest::Approx(0.5f * 0.8f * 0.6f));
}


TEST_CASE("PlayMusics with valid id passes through to backend")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id = manager->LoadMusic("song.ogg"); // id == 1
	auto result = manager->PlayMusics(id, 0);

	CHECK(result == true);
	CHECK(mock->playMusicCount == 1);
	CHECK(mock->lastPlayedMusic == id);
}


TEST_CASE("PlaySFX when backend returns -1 channel propagates -1")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	mock->playChannelStart = -1;

	auto id = manager->LoadSFX("sfx.wav");
	auto channel = manager->PlaySFX(id, 0);

	CHECK(channel == -1);
	CHECK(mock->playChannelCount == 1); // Backend was called
}


TEST_CASE("IsMusicPlaying reflects play/stop/pause/resume state")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	auto id = manager->LoadMusic("song.ogg");
	manager->PlayMusics(id, 0);
	CHECK(manager->IsMusicPlaying() == true);

	manager->PauseMusic();
	CHECK(manager->IsMusicPlaying() == false);

	manager->ResumeMusic();
	CHECK(manager->IsMusicPlaying() == true);

	manager->StopMusic();
	CHECK(manager->IsMusicPlaying() == false);
}


TEST_CASE("ICoreSystem lifecycle methods are safe no-ops")
{
	AudioConfig config;
	MockAudioBackend* mock = nullptr;
	auto manager = MakeManager(config, mock);

	ICoreSystem* system = manager.get();

	system->BeginFrame();

	CHECK(mock->loadMusicCount == 0);
	CHECK(mock->playMusicCount == 0);

	system->PreUpdate(0.0f);
	system->EndUpdate(0.0f);
	system->EndDraw();
}


TEST_SUITE_END();