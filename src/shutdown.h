#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

// Global shutdown flag - checked by all server loops
extern std::atomic<bool> g_shutdown_requested;

// Condition variable to wake up blocking operations
extern std::condition_variable g_shutdown_cv;
extern std::mutex g_shutdown_mutex;

// Call this to request shutdown (async-signal-safe for the flag)
inline void request_shutdown()
{
    g_shutdown_requested.store(true, std::memory_order_release);
    g_shutdown_cv.notify_all();
}

// Check if shutdown was requested
inline bool is_shutdown_requested()
{
    return g_shutdown_requested.load(std::memory_order_acquire);
}

// Wait for shutdown with timeout (returns true if shutdown requested)
inline bool wait_for_shutdown_ms(int ms)
{
    std::unique_lock<std::mutex> lock(g_shutdown_mutex);
    return g_shutdown_cv.wait_for(lock, std::chrono::milliseconds(ms), [] {
        return g_shutdown_requested.load();
    });
}
