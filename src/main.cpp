#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <dpp/dpp.h>
#include <fstream>
#include <string>
#include <sys/types.h>

std::string getToken() {
    std::ifstream file("../.env");
    std::string line;
    while (std::getline(file, line)) {
	if (line.rfind("TOKEN=", 0) == 0)
	    return line.substr(6);
    }
    return "";
}

// dont worry about this part. Ctrl + c/v is def easier than make new function
std::string getServer() {
    std::ifstream file("../.env");
    std::string line;
    while (std::getline(file, line)) {
	if (line.rfind("SERVER=", 0) == 0)
	    return line.substr(7);
    }
    return "";
}
class Denia {
    private:
	dpp::cluster denia;
    public:
	Denia(std::string token) : denia(token) {}
	void init() {
	    denia.on_log(dpp::utility::cout_logger());

	    denia.on_ready([this](const dpp::ready_t& event) {
		    if (dpp::run_once<struct register_bot_commands>()) {
		    dpp::slashcommand ping("ping", "check if bot is alive", denia.me.id);
		    dpp::slashcommand play("play", "play", denia.me.id);
		    dpp::slashcommand pause("pause", "pause", denia.me.id);
		    denia.guild_bulk_command_create(
			    {ping, play, pause},
			    getServer()
			    );
		    }
		    });

	    denia.on_slashcommand([this](const dpp::slashcommand_t& event) {
		    std::string command = event.command.get_command_name();
		    if (command == "ping") event.reply("pong");
		    else if (command == "play") {
		    event.reply("play current song!");
		    } else if (command == "pause") {
		    event.reply("pause current song.");
		    } else {
		    event.reply("command not found");
		    }
		    });

	    denia.start(dpp::st_wait);
	}
};

int main(){
    //   dpp::cluster Denia(getToken());
    //
    //   Denia.on_log(dpp::utility::cout_logger());
    //
    //   Denia.on_ready([&Denia](const dpp::ready_t& event) {
    //    if (dpp::run_once<struct register_bot_commands>() ) {
    //    Denia.global_command_create(
    //     dpp::slashcommand("ping", "Check if bot is alive", Denia.me.id)
    //     );
    //    }
    //    });
    //
    //   Denia.on_slashcommand([&Denia](const dpp::slashcommand_t& event) { 	
    //    std::string command = event.command.get_command_name();
    //    if (command == "ping") event.reply("pong!");
    //    else if (command == "play") {
    // event.reply("Play what?");
    //    }
    //    else { event.reply("Command not found 😕"); }
    //    });
    //
    //   Denia.start(dpp::st_wait);
    Denia denia(getToken());
    denia.init();
    return 0;
}
