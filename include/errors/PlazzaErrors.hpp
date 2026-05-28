/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Pizza
*/

/*
 * Define the erors.
 */

#ifndef HIGHERROR_HPP_
    #define HIGHERROR_HPP_
    #include <exception>
    #include <string>

class HighError : public std::exception {
    private:
        std::string _msg;
    public:
        HighError(const std::string &str);
        const char *what() const noexcept override;
};

class LowError : public std::exception {
    private:
        std::string _msg;
    public:
        LowError(const std::string &str);
        const char *what() const noexcept override;
};


#endif /* HIGHERROR_HPP_ */
