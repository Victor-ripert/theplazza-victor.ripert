/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Pizza
*/

/*
 * Define the erors.
 */

#ifndef PIZZAEXCEPTION_HPP_
    #define PIZZAEXCEPTION_HPP_
    #include <exception>
    #include <string>

class PizzaException : public std::exception {
    private:
        std::string _msg;
    public:
        PizzaException(const std::string &str);
        const char *what() const noexcept override;
};

#endif PIZZAEXCEPTION_HPP
