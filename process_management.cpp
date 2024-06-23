#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <string>

/*
pid_t:
- custom type probably integer, used to store return pid
  of fork function.

fork():
- returns a pid_t, it clones the calling process. parent clones a child
  child shares the copy of parents address space. not copy itself

exit():
- default c/cpp std library call used to terminate a process.
  0 = EXIT_SUCCESS
  1 = EXIT_FAILURE

waitpid():
- waits for a process with specified PIDs status
  to update. normally to die. option parameter is
  normally 0.

getpid():
- get process id of current process

WIFEXITED():
- macro to check how a specified process has exited

WEXITSTATUS():
- macro to get exit_status from specified process

execlp():
- executes from file PATH, takes a list of parameters
  to execute followed by a nullptr as per cpp syntax
*/

void process_management(std::vector<std::string> cmds)
{
    pid_t pid;
    for (const std::string& cmd : cmds)
    {
        pid = fork();
        switch (pid)
        {
            case -1:
                std::cerr << "PROCESS_MANAGER  fork failed" << std::endl;
                exit(EXIT_FAILURE);
            case 0:
                std::cout << "PROCESS_MANAGER  child process(" << getpid() << ") executing: " << cmd << std::endl;

                // child logic
                execlp(cmd.c_str(), cmd.c_str(), nullptr); // will return if successful
                std::cout << "PROCESS_MANAGER command execution failed" << std::endl;
                exit(EXIT_FAILURE);
            default:
                std::cout << "PROCESS_MANAGER  parent process(" << getpid() << ")" << std::endl;
                
                // child handling logic
                int status;
                pid_t child = waitpid(pid, &status, 0); // wait for some event
                if (child == -1)
                {
                    std::cerr << "PROCESS_MANAGER  waitpid() failed" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if (WIFEXITED(status)) // if event is IFEXITED then
                {
                    std::cout << "PROCESS_MANAGER child(" << getpid() << 
                    ") exited with status: " << WEXITSTATUS(status) << std::endl << std::endl;
                }
                else                   // otherwise probably crashed etc
                {
                    std::cout << "PROCESS_MANAGER child(" << getpid() << 
                    ") failed to exit switfly" << std::endl;
                    exit(EXIT_FAILURE);
                }
        }
    }
}


int main()
{
    const std::vector<std::string> cmds = { "ls", "date"};
    process_management(cmds);
    return 0;
}