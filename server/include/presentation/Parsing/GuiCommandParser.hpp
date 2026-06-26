/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GuiCommandParser.hpp
*/

#ifndef GUI_COMMAND_PARSER_HPP_
#define GUI_COMMAND_PARSER_HPP_

#include <string>
#include <memory>
#include "INetworkProvider.hpp"

class GUI;
class Server;

/**
 * @class GuiCommandParser
 * @ingroup visitor_gui
 * @brief Parses GUI command lines and builds the matching command DTO/visitor.
 *
 * Each @c parse_gui_* method extracts the arguments, fills the DTO and dispatches
 * it to a @ref GuiVisitor. Protocol details live on the corresponding DTO class.
 */
class GuiCommandParser {
    public:
        GuiCommandParser() = default;
        ~GuiCommandParser() = default;

        /** @brief Parses GUI "msz" (map size). */
        void parse_gui_msz(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_bct(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_mct(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_tna(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_ppo(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_plv(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_pin(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_sgt(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
        void parse_gui_sst(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);
};

#endif /* !GUI_COMMAND_PARSER_HPP_ */
