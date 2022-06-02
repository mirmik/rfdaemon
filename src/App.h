#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>

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
	App(const std::string& name,
		const std::string& cmd,
		RestartMode mode = ALWAYS,
		const std::vector<std::string>& logs = std::vector<std::string>());
	void stop(bool atStart = false);
	void start();
	bool stopped() const;
	bool finished() const;
	RestartMode restartMode() const;
	int pid() const;
	const std::string& name() const;
	const std::string& command() const;
	const std::vector<std::string>& args() const;
	int exitStatus() const;
	int64_t uptime() const;
	uint32_t restartAttempts() const;
	pid_t waitFinish();
	void run();
	const std::vector<std::string>& logPaths() const;
	std::queue<int>& errors();
	std::string status_string() const;

	bool need_to_another_attempt() const;
	void increment_attempt_counter();
	void restart_attempt_counter();

private:
	void watchFunc();
	pid_t appFork();
	
	int32_t _attempts_initializer = 5;
	int32_t _attempts = _attempts_initializer;

	bool _watcher_guard = false;
	std::thread _watcher_thread = {};

	int restartCount = 0;
	bool isStopped = true;
	//bool successStart = true;
	int _exitStatus = 0;
	RestartMode _restartMode = RestartMode::ALWAYS;
	std::string _cmd, _name;
	std::vector<std::string> _logPaths;
	std::vector<std::string> _args;
	std::queue<int> _errors;
	int _shPid = 0, _pid = 0;
	static std::mutex ioMutex;
};
