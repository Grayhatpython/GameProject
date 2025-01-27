#pragma once

class Timer
{
private:
	Timer() {};
	~Timer() {};

public:
	static std::shared_ptr<Timer> GetInstance()
	{
		std::call_once(S_InitializeFlag, []() {
			S_Instance = std::make_shared<Timer>();
			});

		return S_Instance;
	}

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

public:
	void Initialize();
	void Update();

public:
	uint32_t	GetFPS() const { return _fps; }
	float		GetDeltaTime() const { return _deltaTime; }

private:
	static std::shared_ptr<Timer>		S_Instance;
	static std::once_flag				S_InitializeFlag;

private:
	uint64_t	_frequency = 0;
	uint64_t	_prevCount = 0;
	float		_deltaTime = 0.f;

	uint32_t	_frameCount = 0;
	float		_frameTime = 0.f;
	uint32_t	_fps = 0;
};