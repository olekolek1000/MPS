/*
 * Discord RPC Wrapper
 * Copyright (c) 2018 Firstbober
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once 
#include <lib/discord_rpc.h>
#include <string>

class DiscordRPC {
private:
    /* Default engine data */
        const char* DiscordID {"465595228225798144"};

        std::string State          {"Nothing"};       //State
        std::string Details        {"In MPS"}; //Details
        std::string LargeImageKey  {"default"};       //Key of large image on status
        std::string LargeImageText {"MPS"};    //Description of large image

    /* Standart application data */
        char* appID;                    //ID of application
        DiscordRichPresence statusData; //Data of currect status
public:
    /* Constructor */
        
        DiscordRPC();
        
    /* Init Methods */

        void init();
           
    /* Status methods */

        //Get and set state
        std::string getState();
        void setState(std::string state);

        //Get and set detail
        std::string getDetails();
        void setDetails(std::string details);

        //Get and set large image key
        std::string getLargeImageKey();
        void setLargeImageKey(std::string largeImageKey);

        //Get and set large image text
        std::string getLargeImageText();
        void setLargeImageText(std::string largeImageText);

        //Get and set small image key
        std::string getSmallImageKey();
        void setSmallImageKey(std::string smallImageKey);

        //Get and set small image text
        std::string getSmallImageText();
        void setSmallImageText(std::string smallImageText);

        //Get and set secret of match
        std::string getMatchSecret();
        void setMatchSecret(std::string matchSecret);

        //Get and set secret of spectate
        std::string getSpectateSecret();
        void setSpectateSecret(std::string spectateSecret);

        //Get and set secret of join
        std::string getJoinSecret();
        void setJoinSecret(std::string joinSecret);

        //Get and set start timestamp
        int64_t getStartTimestamp();
        void setStartTimestamp(int64_t timestamp);

        //Get and set end of timestamp
        int64_t getEndTimestamp();
        void setEndTimestamp(int64_t timestamp);

        //Get and set party size
        int getPartySize();
        void setPartySize(int size);
            
        //Get and set maximum of party size
        int getMaxPartySize();
        void setMaxPartySize(int size);

        //Get and set instance(?)
        int getInstance();
        void setInstance(int size);

        //Update status
        void updateStatus();
        void updateStatus(const DiscordRichPresence* presence);

    /* Exit function */

        void exit();
};
