#pragma once

class Timer
{
public:
	void Initialize();
	void Update();

public:
	uint32_t	GetFPS() const { return _fps; }
	float		GetDeltaTime() const { return _deltaTime; }

private:
	uint64_t	_frequency = 0;
	uint64_t	_prevCount = 0;
	float		_deltaTime = 0.f;

	uint32_t	_frameCount = 0;
	float		_frameTime = 0.f;
	uint32_t	_fps = 0;
};