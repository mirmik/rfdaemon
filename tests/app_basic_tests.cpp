#include "App.h"
#include "guard/guard.h"

#include <string>
#include <vector>

TEST_CASE(
    "LinkedFile::to_trent: корректное отображение полей в трент-структуру")
{
    LinkedFile lf;
    lf.path = "/tmp/test.conf";
    lf.name = "config";
    lf.editable = true;

    nos::trent tr = lf.to_trent();

    CHECK_EQ(tr["path"].as_string(), lf.path);
    CHECK_EQ(tr["name"].as_string(), lf.name);
    CHECK_EQ(tr["editable"].as_bool(), lf.editable);
}


TEST_CASE(
    "App::command и App::token_list_as_string строят строку из токенов команды")
{
    std::vector<LinkedFile> linkeds;
    App app(0, "echo_app", "echo 1 2", App::RestartMode::ONCE, linkeds, "");

    CHECK_EQ(app.command(), std::string("echo 1 2"));
    CHECK_EQ(app.token_list_as_string(), std::string("[echo,1,2]"));
}

// Lifecycle integration is checked separately on a target with systemd.
// These tests must never create units or start processes on the build host.
TEST_CASE("App generates a systemd unit with the requested restart policy")
{
    App app(0, "worker", "/usr/local/bin/rfmeas --debug", App::ALWAYS, {}, "rfmeas");
    const auto unit = app.generate_service_content();
    CHECK_NEQ(unit.find("ExecStart=/usr/local/bin/rfmeas --debug\n"), std::string::npos);
    CHECK_NEQ(unit.find("Restart=always\n"), std::string::npos);
    CHECK_NEQ(unit.find("User=rfmeas\n"), std::string::npos);
    app.setRestartMode(App::ONCE);
    CHECK_NEQ(app.generate_service_content().find("Restart=no\n"), std::string::npos);
}

TEST_CASE("App updates and sanitizes its service identity")
{
    App app(0, "old", "sleep 10", App::ONCE, {}, "");
    app.setName("positioner worker/1");
    CHECK_EQ(app.service_name(), std::string("rfd-positioner_worker_1"));
    CHECK_EQ(app.service_path(), std::string("/etc/systemd/system/rfd-positioner_worker_1.service"));
    CHECK_NEQ(app.generate_service_content().find("Description=rfdaemon managed: positioner worker/1\n"), std::string::npos);
}

TEST_CASE("App serializes configuration edits without touching systemd")
{
    App app(0, "worker", "sleep 10", App::ONCE, {}, "rfmeas");
    app.setCommand("echo updated");
    app.setRestartMode(App::ALWAYS);
    app.set_environment_variables({{"POSITIONER_PROFILE", "host"}});
    const auto config = app.toTrent();
    CHECK_EQ(config["command"].as_string(), std::string("echo updated"));
    CHECK_EQ(config["restart"].as_string(), std::string("always"));
    CHECK_EQ(config["user"].as_string(), std::string("rfmeas"));
    CHECK_EQ(config["env"]["POSITIONER_PROFILE"].as_string(), std::string("host"));
    CHECK_NEQ(app.generate_service_content().find("Environment=\"POSITIONER_PROFILE=host\"\n"), std::string::npos);
}
