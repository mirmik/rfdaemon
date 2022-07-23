#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <AppManager.h>
#include <memory>
#include <nos/io/sstream.h>
#include <nos/fprint.h>

extern std::unique_ptr<AppManager> appManager;

void start_httpserver() 
{
	std::thread([](){
		// HTTP
		httplib::Server svr;
		
		// HTTPS
//		httplib::SSLServer svr;

		svr.Get("/", [](const httplib::Request &, httplib::Response &res) {

			auto& apps = appManager->applications();
			nos::stringstream ss;
    		for (auto &app : apps)
        		nos::fprintln_to(ss, "{} : {} : {}", app.name(), app.status_string(),
                    app.pid());

			std::string text = "Hello World!\n\n" + ss.str(); 
			res.set_content(text, "text/plain");
		});
		
		svr.listen("0.0.0.0", 8080);
	}).detach();
}