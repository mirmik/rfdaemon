#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <queue>

namespace std
{
	class thread;
}

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
	bool stopped() const;
	bool finished() const;
	RestartMode restartMode() const;
	bool successfulStart() const;
	int pid() const;
	const std::string& name() const;
	const std::string& command() const;
	const std::vector<std::string>& args() const;
	int exitStatus() const;
	int64_t uptime() const;
	uint32_t restartAttempts() const;
	void clearRestartAttempts();
	pid_t waitFinish();
	void run();
	const std::string& logPath() const;
	std::queue<int8_t>& errors();
private:
	void watchFunc();
	pid_t appFork();
	std::thread* _thread = NULL;
	int restartCount = 0;
	bool isStopped = true;
	bool isFinished = false;
	bool successStart = true;
	uint32_t _restartAttempts = 0;
	int _exitStatus = 0;
	RestartMode _restartMode = RestartMode::ALWAYS;
	std::string _cmd, _name, _logPath;
	std::vector<std::string> _args;
	std::queue<int8_t> _errors;
	int _shPid = 0, _pid = 0;
};
