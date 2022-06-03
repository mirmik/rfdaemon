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
	RestartMode restartMode() const;
	int pid() const;
	const std::string& name() const;
	const std::vector<std::string>& args() const;
	int64_t uptime() const;
	uint32_t restartAttempts() const;
	pid_t waitFinish();
	void run();
	std::vector<std::string> logPaths() const;
	std::queue<int>& errors();
	std::string status_string() const;

	bool need_to_another_attempt() const;
	void increment_attempt_counter();
	void restart_attempt_counter();

	static std::vector<char*> tokens_for_execve(const std::vector<std::string>& args);

private:
	void watchFunc();
	pid_t appFork();

private:	
	std::chrono::time_point<std::chrono::system_clock> _startTime;
	std::vector<std::string> tokens;
	int32_t _attempts_initializer = 5;
	int32_t _attempts = _attempts_initializer;
	bool _watcher_guard = false;
	std::thread _watcher_thread = {};
	bool isStopped = true;
	int _exitStatus = 0;
	RestartMode _restartMode = RestartMode::ALWAYS;
	std::string _name;
	std::queue<int> _errors;
	int _pid = 0;

};
