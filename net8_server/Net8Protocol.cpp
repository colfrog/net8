//
// Created by laurent on 11/01/2026.
//

#include <string>
#include <iostream>

#include "Net8Protocol.h"
#include "Server.h"

Net8Protocol::Net8Protocol(Server *server) : m_server{server}, m_playground{this} {
    // Commands initialisation
    m_commands["IDENT"] = [this](int s, const std::string &m){ on_ident(s, m); };
    m_commands["CHAT"] = [this](int s, const std::string &m){ on_chat(s, m); };
    m_commands["ADDROOM"] = [this](int s, const std::string &m){ on_addroom(s, m); };
    m_commands["SWITCHROOM"] = [this](int s, const std::string &m){ on_switchroom(s, m); };
    m_commands["JOINGAME"] = [this](int s, const std::string &m){ on_joingame(s, m); };
    m_commands["PARTGAME"] = [this](int s, const std::string &m){ on_partgame(s, m); };
    m_commands["PLAY"] = [this](int s, const std::string &m){ on_play(s, m); };
    m_commands["DRAW"] = [this](int s, const std::string &m){ on_draw(s, m); };
    m_commands["HAND"] = [this](int s, const std::string &m){ on_hand(s, m); };
    m_commands["ROOMS"] = [this](int s, const std::string &m){ on_rooms(s, m); };
}

void Net8Protocol::send_to_room(int game_id, const std::string &message) const {
    const std::list<const Player *> players = m_playground.get_players(game_id);
    for (const Player *player : players) {
        m_server->send_message(player->get_socket(), message);
    }
}

bool Net8Protocol::is_identified(int socket) const {
    return m_playground.get_player(socket) != nullptr;
}

void Net8Protocol::fail_if_unidentified(int socket) const {
    if (!is_identified(socket)) throw protocol_error("Player not identified");
}

void Net8Protocol::handle_message(int socket, const std::string &message) {
    size_t command_end = message.find(':');
    std::string command = message.substr(0, command_end);
    std::string arg = message.substr(command_end + 1);
    if (m_commands.contains(command))
        m_commands[command](socket, arg);
    else throw protocol_error("Received unknown command " + command);
}

void Net8Protocol::log_message(bool out, int socket, const std::string &message) const {
    const Player *player = m_playground.get_player(socket);
    const std::string ip4 = m_server->get_ip4_addr(socket);
    std::cout << std::string(out ? "<--" : "-->") << std::string(player ? player->get_name() : "UNIDENTIFIED") << " (" << ip4 << ") - " << message << std::endl;
}

void Net8Protocol::announce_hand(const Player *player) const {
    m_server->send_message(player->get_socket(), "HAND:" + player->get_hand().to_string());
}
void Net8Protocol::announce_join(const Player *player) const {
    send_to_room(player->get_game_id(), "JOIN:" + player->get_name());
}
void Net8Protocol::announce_part(const Player *player) const {
    send_to_room(player->get_game_id(), "PART:" + player->get_name());
}
void Net8Protocol::announce_leave(const Player *player) const {
    send_to_room(player->get_game_id(), "LEAVE:" + player->get_name());
}
void Net8Protocol::announce_spectator(const Player *player) const {
    send_to_room(player->get_game_id(), "SPECTATOR:" + player->get_name());
}
void Net8Protocol::announce_turn(const Player *player, const Player *to) const {
    if (to == nullptr)
        send_to_room(player->get_game_id(), "TURN:" + player->get_name());
    else
        m_server->send_message(to->get_socket(), "TURN:" + player->get_name());
}
void Net8Protocol::announce_top_card(const Game *game, const Player *to) const {
    if (to == nullptr)
        send_to_room(game->get_game_id(), "TOP:" + game->get_pile().top_card()->to_string());
    else
        m_server->send_message(to->get_socket(), "TOP:" + game->get_pile().top_card()->to_string());
}
void Net8Protocol::announce_inactive_room(const Game *game) const {
    m_server->send_to_clients("INACTIVE:" + std::to_string(game->get_game_id()));
}
void Net8Protocol::announce_new_room(const Game *game) const {
    m_server->send_to_clients("ROOM:" + std::to_string(game->get_game_id()) + ":" + game->get_name());
}

void Net8Protocol::announce_rooms(const Player *player) const {
    m_server->send_message(player->get_socket(), "ROOMS:" + m_playground.get_rooms().to_string());
}

void Net8Protocol::on_connect(int socket) {
    m_server->send_message(socket, "IDENT");
}

void Net8Protocol::on_disconnect(int socket) {
    m_playground.remove_player(socket);
}

void Net8Protocol::on_ident(int socket, const std::string &name) {
    m_playground.add_player(socket, name); // TODO: Sanitize the name
}

void Net8Protocol::on_chat(int socket, const std::string &message) {
    const Player *player = m_playground.get_player(socket);
    send_to_room(player->get_game_id(), "CHAT:" + player->get_name() + ":" + message);
}

void Net8Protocol::on_addroom(int socket, const std::string &message) {
    m_playground.add_room(message); // TODO: sanitize the name
    announce_new_room(m_playground.get_room(m_playground.get_player(socket)->get_game_id()));
}

void Net8Protocol::on_switchroom(int socket, const std::string &message) {
    m_playground.transfer_player(socket, std::stoi(message)); // TODO: Sanitize the message to only accept int
}

void Net8Protocol::on_joingame(int socket, const std::string &message) {
    m_playground.player_join(socket);
}

void Net8Protocol::on_partgame(int socket, const std::string &message) {
    m_playground.player_part(socket);
}

void Net8Protocol::on_play(int socket, const std::string &message) {
    size_t card_end = message.find(':');
    std::string card_index = message.substr(0, card_end);
    std::string arg = message.substr(card_end + 1);
    Player *player = m_playground.get_player(socket);
    int game_id = player->get_game_id();
    Game *room = m_playground.get_room(game_id);
    room->do_turn(player, true, std::stoi(card_index), arg);
}

void Net8Protocol::on_draw(int socket, const std::string &message) {
    Player *player = m_playground.get_player(socket);
    Game *room = m_playground.get_room(player->get_game_id());
    room->do_turn(player, false);
}

void Net8Protocol::on_hand(int socket, const std::string &message) {
    Player *player = m_playground.get_player(socket);
    if (!player->is_playing())
        throw protocol_error("Spectators have no hand");
    announce_hand(player);
}

void Net8Protocol::on_rooms(int socket, const std::string &message) {
    announce_rooms(m_playground.get_player(socket));
}