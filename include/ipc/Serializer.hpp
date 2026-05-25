/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Serializer
*/

/*
 * Responsible for data serialization.
 * Provides the logic to pack Pizza objects into a string or byte stream for transmission over IPC,
 * and unpack them back into objects upon receipt.
 */

#ifndef SERIALIZER_HPP_
    #define SERIALIZER_HPP_

#include <string>
#include "../common/Pizza.hpp"

class Serializer {
    public:
        static std::string pack(const Pizza& pizza);
        static Pizza unpack(const std::string& data);
};

#endif
