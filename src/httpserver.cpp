//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <AppManager.h>
#include <memory>
#include <nos/io/sstream.h>
#include <nos/fprint.h>
#include <thread>
#include <chrono>

extern std::unique_ptr<AppManager> appManager;
using namespace nos::argument_literal;
using namespace std::chrono_literals;

void start_httpserver() 
{
	std::thread([](){
		// HTTP
		httplib::Server svr;
		
		// HTTPS
//		httplib::SSLServer svr;

		svr.Get("/", [](const httplib::Request &req, httplib::Response &res) {
			std::string cmd = req.get_param_value("cmd");
			
			if (cmd == "stop") 
			{
				int arg = std::stoi(req.get_param_value("arg"));
				nos::println(cmd, arg);
				appManager->applications()[arg].stop();
				std::this_thread::sleep_for(200ms);
			}

			if (cmd == "start") 
			{
				int arg = std::stoi(req.get_param_value("arg"));
				nos::println(cmd, arg);
				appManager->applications()[arg].start();
				std::this_thread::sleep_for(200ms);
			}

			auto& apps = appManager->applications();
			nos::stringstream ss;
    		for (size_t i=0; i<apps.size(); i++)
			{
				auto& app = apps[i];
				std::string button_stop = nos::format(R"(<button type="button" onclick="window.location.href='/?cmd=start&arg={}';">start</button>)", i);
				std::string button_start = nos::format(R"(<button type="button" onclick="window.location.href='/?cmd=stop&arg={}';">stop</button>)", i);
        		nos::fprintln_to(ss, R"(<p>{} : {} : {} {} {})", app.name(), app.status_string(),
                    app.pid(), button_stop, button_start);
			}
			std::string text = nos::format(R"(
<!DOCTYPE HTML>
<html>
 <head>
  <meta charset="utf-8">
  <title>Тег BUTTON</title>
 </head>
 <body>
  <p style="text-align: center"><button>Кнопка с текстом</button>
  <button><img src="images/umbrella.gif" alt="Зонтик" 
         style="vertical-align: middle"> Кнопка с рисунком</button></p>

		 {tasks}
 </body>
</html>
			)", "tasks"_a=ss.str()); 
			res.set_content(text, "text/html");
		});
		
		svr.listen("0.0.0.0", 8080);
	}).detach();
}