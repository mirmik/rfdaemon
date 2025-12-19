#ifndef CONSOLE_H
#define CONSOLE_H

void start_tcp_console(int tcp_console_port);
void start_stdstream_console();
void stop_tcp_consoles();
void join_tcp_console_threads();

#endif