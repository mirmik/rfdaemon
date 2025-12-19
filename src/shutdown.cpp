#include "shutdown.h"

std::atomic<bool> g_shutdown_requested{false};
std::condition_variable g_shutdown_cv;
std::mutex g_shutdown_mutex;
