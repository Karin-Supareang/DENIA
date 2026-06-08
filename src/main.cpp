#include <dpp/cache.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <dpp/dpp.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <iomanip>
#include <sstream>

static std::string getToken() {
    std::ifstream file("../.env");
    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("TOKEN=", 0) == 0)
            return line.substr(6);
    }
    return "";
}

// dont worry about this part. Ctrl + c/v is def easier than make new function
static std::string getServer() {
    std::ifstream file("../.env");
    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("SERVER=", 0) == 0)
            return line.substr(7);
    }
    return "";
}

struct DeniaState {
    bool is_joined = false;
    bool is_playing = false;
};
class Denia {
    private:
        dpp::cluster denia;
        DeniaState state;
    public:
        Denia() : denia(getToken()){}
        void init() {
            denia.on_log(dpp::utility::cout_logger());

            denia.on_ready([this](const dpp::ready_t& event) {
                    if (dpp::run_once<struct register_bot_commands>()) {
                    dpp::slashcommand ping("ping", "Changed description", denia.me.id);
                    dpp::slashcommand join("join", "Test joining VC", denia.me.id);
                    dpp::slashcommand play("play", "play", denia.me.id);
                    dpp::slashcommand pause("pause", "pause", denia.me.id);
                    dpp::slashcommand leave("leave", "Kick bot off voice channel", denia.me.id);
                    dpp::slashcommand rin("rin", "Who is he", denia.me.id);
                    denia.guild_bulk_command_create(
                            {ping, play, pause, join, leave, rin},
                            getServer()
                            );
                    }
                    });

            denia.on_slashcommand([this](const dpp::slashcommand_t& event) {
                    std::string command = event.command.get_command_name();
                    if (command == "ping") event.reply("pong!");
                    else if (command == "join") {
                    joinVC(event);
                    } else if (command == "pause") {
                    event.reply("pause current song.");
                    } else if (command == "leave") {
                    leaveCurrVC(event);
                    } else if (command == "rin") {
                    event.reply("He is my daddy!");
                    }
                    else {
                    event.reply("command not found");
                    }
                    });
            denia.on_voice_state_update([this](const dpp::voice_state_update_t& event) {
                    if (event.state.user_id == denia.me.id) {
                    std::cout << "Voice state update! channel_id: " << event.state.channel_id << std::endl;
                    if (event.state.channel_id == 0) {
                    this->state.is_joined = false;
                    }
                    }
                    });
            denia.start(dpp::st_wait);
        }

        void joinVC(const dpp::slashcommand_t& event) {
            dpp::guild* g = dpp::find_guild(event.command.guild_id);
            auto curr_vc = event.from()->get_voice(event.command.guild_id);
            this->state.is_joined = true;
            if(curr_vc){
                auto users_vc = g->voice_members.find(event.command.get_issuing_user().id);
                if (users_vc != g->voice_members.end() && curr_vc->channel_id == users_vc->second.channel_id) {
                    this->state.is_joined = false;
                }else {
                    event.from()->disconnect_voice(event.command.guild_id);
                    this->state.is_joined = true;
                }
            }

            if (this->state.is_joined) {
                if(!g->connect_member_voice(*event.owner, event.command.get_issuing_user().id)) {
                    event.reply("You are not not in any voice channel!");
                    return;
                }
                event.reply("Joined your VC!");
            } else {
                event.reply("No need to join your channel. Im already with you!");
            }
        }
        void leaveCurrVC (const dpp::slashcommand_t& event) {
            if(!this->state.is_joined){ 
                event.reply( "I'm not in any voice channel dummy."); 
                return;
            }
            event.reply("Leaving...");
            event.from()->disconnect_voice(event.command.guild_id);
            this->state.is_joined = false;
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
    Denia denia;
    denia.init();
    return 0;
}
