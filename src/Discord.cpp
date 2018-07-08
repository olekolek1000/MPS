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

#include "Discord.hpp"
#include <string.h>
#include <lib/discord_rpc.h>

/* Constructor */
    
    DiscordRPC::DiscordRPC() {
        Discord_Shutdown();
        this->init();
    }
        
/* Init Method */

    void DiscordRPC::init() {
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(DiscordID, &handlers, 1, NULL);

        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state = State.c_str();
        discordPresence.details = Details.c_str();
        discordPresence.largeImageKey = LargeImageKey.c_str();
        discordPresence.largeImageText = LargeImageText.c_str();
        discordPresence.instance = 1;
        discordPresence.startTimestamp = 0;
        Discord_UpdatePresence(&discordPresence);

        memset(&this->statusData, 0, sizeof(statusData));
    }

/* Status methods */

    //Get and set state
    std::string DiscordRPC::getState() {
        return this->statusData.state;
    }
    void DiscordRPC::setState(std::string state) {
        this->statusData.state = state.c_str();
    }

    //Get and set detail
    std::string DiscordRPC::getDetails() {
        return this->statusData.details;
    }
    void DiscordRPC::setDetails(std::string details) {
        this->statusData.details = details.c_str();
    }

    //Get and set large image key
    std::string DiscordRPC::getLargeImageKey() {
        return this->statusData.largeImageKey;
    }
    void DiscordRPC::setLargeImageKey(std::string largeImageKey) {
        this->statusData.largeImageKey = largeImageKey.c_str();
    }   

    //Get and set large image text
    std::string DiscordRPC::getLargeImageText() {
        return this->statusData.largeImageText;
    }
    void DiscordRPC::setLargeImageText(std::string largeImageText) {
        this->statusData.largeImageText = largeImageText.c_str();
    }

    //Get and set small image key
    std::string DiscordRPC::getSmallImageKey() {
        return this->statusData.smallImageKey;
        }
    void DiscordRPC::setSmallImageKey(std::string smallImageKey) {
        this->statusData.smallImageKey = smallImageKey.c_str();
    }

    //Get and set small image text
    std::string DiscordRPC::getSmallImageText() {
        return this->statusData.smallImageText;
    }
    void DiscordRPC::setSmallImageText(std::string smallImageText) {
        this->statusData.smallImageText = smallImageText.c_str();
    }

    //Get and set secret of match
    std::string DiscordRPC::getMatchSecret() {
        return this->statusData.matchSecret;
    }
    void DiscordRPC::setMatchSecret(std::string matchSecret) {
        this->statusData.matchSecret = matchSecret.c_str();
    }

    //Get and set secret of spectate
    std::string DiscordRPC::getSpectateSecret() {
        return this->statusData.spectateSecret;
    }
    void DiscordRPC::setSpectateSecret(std::string spectateSecret) {
        this->statusData.spectateSecret = spectateSecret.c_str();
    }

    //Get and set secret of join
    std::string DiscordRPC::getJoinSecret() {
        return this->statusData.joinSecret;
    }
    void DiscordRPC::setJoinSecret(std::string joinSecret) {
        this->statusData.joinSecret = joinSecret.c_str();
    }

    //Get and set start timestamp
    int64_t DiscordRPC::getStartTimestamp() {
        return this->statusData.startTimestamp;
    }
    void DiscordRPC::setStartTimestamp(int64_t timestamp) {
        this->statusData.startTimestamp = timestamp;
    }

    //Get and set end of timestamp
    int64_t DiscordRPC::getEndTimestamp() {
        return this->statusData.endTimestamp;
    }
    void DiscordRPC::setEndTimestamp(int64_t timestamp) {
        this->statusData.endTimestamp = timestamp;
    }

    //Get and set party size
    int DiscordRPC::getPartySize() {
        return this->statusData.partySize;
    }
    void DiscordRPC::setPartySize(int size) {
        this->statusData.partySize = size;
    }
            
    //Get and set maximum of party size
    int DiscordRPC::getMaxPartySize() {
        return this->statusData.partyMax;
    }
    void DiscordRPC::setMaxPartySize(int size) {
        this->statusData.partyMax = size;
    }

    //Get and set instance(?)
    int DiscordRPC::getInstance() {
        return this->statusData.instance;
    }
    void DiscordRPC::setInstance(int size) {
        this->statusData.instance = size;
    }

    //Update status
    void DiscordRPC::updateStatus() {
        Discord_UpdatePresence(&statusData);
    }
    void DiscordRPC::updateStatus(const DiscordRichPresence* presence) {
        Discord_UpdatePresence(presence);
    }

/* Exit function */
    
    void DiscordRPC::exit() {
        Discord_Shutdown();
    }
    
