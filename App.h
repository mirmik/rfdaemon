#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

class App
{
public:
	enum RestartMode
	{
		ALWAYS = 0,
		ERROR,
		NEVER
	};
	App(const std::string& name, const std::string& cmd, RestartMode mode);
	void stop(bool atStart = false);
	void beforeRunning();
	bool stopped() const;
	bool finished() const;
	RestartMode restartMode() const;
	bool successfulStart() const;
	int pid() const;
	const std::string& name() const;
	const std::string& command() const;
	const std::vector<std::string>& args() const;
	int exitStatus() const;
	void setPid(int pid);
	int64_t uptime() const;
	uint32_t restartAttempts() const;
	pid_t waitFinish();
	void run();
private:
	volatile int watchFunc();
	pid_t appFork();
	std::thread* _thread = NULL;
	int restartCount = 0;
	bool isStopped = false;
	bool isFinished = false;
	bool successStart = true;
	uint32_t _restartAttempts = 0;
	int _exitStatus = 0;
	RestartMode _restartMode = RestartMode::ALWAYS;
	std::string _cmd, _name;
	std::vector<std::string> _args;
	int _pid = 0;
};
