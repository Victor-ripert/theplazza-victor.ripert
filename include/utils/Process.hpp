/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Process
*/

/*
 * Wraps system-level process management calls (fork, wait, exit).
 * Handles the creation and termination of child processes (Kitchens) from the main parent process (Reception).
 */

#ifndef PROCESS_HPP_
    #define PROCESS_HPP_

#include <sys/types.h>

class Process {
    public:
        Process();
        ~Process();
        pid_t spawn();
        void wait();
        void terminate();

    private:
        pid_t _pid;
};

#endif
