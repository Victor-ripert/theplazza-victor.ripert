/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Parser
*/

/*
 * Handles the lexical analysis and validation of user input.
 * Converts raw command-line strings from the user into structured, validated lists of Pizza objects.
 */

#ifndef PARSER_HPP_
    #define PARSER_HPP_
#include <string>
#include <vector>
#include "../common/Pizza.hpp"
#include "../common/PizzaException.hpp"

class Parser {
    public:
        Parser() = default;
        ~Parser() = default;

        std::vector<Pizza> parseCommand(const std::string& input);
};

#endif
