**Input/Output Functions**

### 1. `readline()`

Description: Reads a line from the standard input.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *line = readline("Enter your name: ");
    if (line == NULL) {
        printf("Error reading line\n");
        return 1;
    }
    printf("Hello, %s!\n", line);
    free(line);
    return 0;
}
```
Usage: Used to read user input from the standard input.

### 2. `rl_clear_history()`

Description: Clears the command history of a terminal.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    rl_clear_history();
    printf("Command history cleared\n");
    return 0;
}
```
Usage: Used to clear the command history when a new program starts.

### 3. `rl_on_new_line()`

Description: Enables line editing on new lines.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    rl_on_new_line();
    printf("Line editing enabled\n");
    return 0;
}
```
Usage: Used to enable line editing when a new line is entered.

### 4. `rl_replace_line()`

Description: Replaces the current line with a new one.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *line = "Hello, World!";
    rl_replace_line(line);
    printf("Line replaced\n");
    return 0;
}
```
Usage: Used to replace the current line with a new one.

### 5. `rl_redisplay()``

Description: Redraws the terminal screen.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    rl_redisplay();
    printf("Terminal redrawn\n");
    return 0;
}
```
Usage: Used to redraw the terminal screen after changes have been made.

**String Manipulation Functions**

### 1. `printf()`

Description: Formats and prints data to the standard output.

Example:
```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```
Usage: Used to print formatted data to the standard output.

### 2. `malloc()` and `free()`

Description: Allocates memory using malloc and frees it using free.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = malloc(sizeof(int));
    *ptr = 10;
    printf("%d\n", *ptr);
    free(ptr);
    return 0;
}
```
Usage: Used to dynamically allocate and deallocate memory.

### 3. `write()`

Description: Writes data to a file descriptor.

Example:
```c
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    write(fd, "Hello, World!\n", 13);
    close(fd);
    return 0;
}
```
Usage: Used to write data to a file descriptor.

**File Operations Functions**

### 1. `access()`

Description: Checks the accessibility of a path.

Example:
```c
#include <stdio.h>
#include <sys/stat.h>

int main() {
    struct stat sb;
    if (access("/bin/ls", R_OK) == -1) {
        printf("Permission denied\n");
    } else {
        printf("Readable\n");
    }
    return 0;
}
```
Usage: Used to check the accessibility of a path.

### 2. `open()`, `close()`

Description: Opens and closes files.

Example:
```c
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    write(fd, "Hello, World!\n", 13);
    close(fd);
    return 0;
}
```
Usage: Used to open and close files.

### 3. `fork()``

Description: Creates a new process using fork.

Example:
```c
#include <stdio.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error forking\n");
        return 1;
    }
    if (pid == 0) {
        // Child process
        printf("Child process\n");
    } else {
        // Parent process
        printf("Parent process\n");
    }
    return 0;
}
```
Usage: Used to create a new process using fork.

### 4. `wait()`, `waitpid()`, `wait3()`, `wait4()``

Description: Waits for child processes to finish.

Example:
```c
#include <stdio.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error forking\n");
        return 1;
    }
    if (pid == 0) {
        // Child process
        sleep(5);
        _exit(0);
    } else {
        // Parent process
        wait(NULL); // Wait for child process to finish
        printf("Child process finished\n");
    }
    return 0;
}
```
Usage: Used to wait for child processes to finish.

### 5. `signal()`, `sigaction()`

Description: Sends signals and handles signal actions.

Example:
```c
#include <stdio.h>
#include <sys/types.h>

int main() {
    // Send a SIGINT signal
    kill(getpid(), SIGINT);

    struct sigaction sa;
    sa.sa_handler = SIG_DFL; // Restore default signal action
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaction(SIGINT, &sa, NULL);

    return 0;
}
```
Usage: Used to send signals and handle signal actions.

**Miscellaneous Functions**

### 1. `getcwd()``

Description: Returns the current working directory.

Example:
```c
#include <stdio.h>
#include <unistd.h>

char *path = getcwd(NULL, sizeof(path));
printf("%s\n", path);
free(path);
return 0;
```
Usage: Used to return the current working directory.

### 2. `chdir()``

Description: Changes the current working directory.

Example:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    chdir("/home/user");
    printf("Current directory changed\n");
    return 0;
}
```
Usage: Used to change the current working directory.

### 3. `stat()` and `lstat()` and `fstat()`

Description: Returns information about files and directories.

Example:
```c
#include <stdio.h>
#include <sys/stat.h>

struct stat sb;
if (stat("/bin/ls", &sb) == -1) {
    printf("Error getting file stats\n");
} else {
    printf("File type: %d\n", sb.st_mode);
}
```
Usage: Used to return information about files and directories.

### 4. `unlink()``

Description: Deletes a file or directory.

Example:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    unlink("example.txt");
    printf("File deleted\n");
    return 0;
}
```
Usage: Used to delete files and directories.

### 5. `execve()``

Description: Replaces the current process image with a new one.

Example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    execve("/bin/ls", NULL, NULL);
    return 0;
}
```
Usage: Used to replace the current process image with a new one.

### 6. `dup()` and `dup2()`

Description: Creates a duplicate file descriptor.

Example:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    dup2(fd, 1); // Duplicate file descriptor
    return 0;
}
```
Usage: Used to create a duplicate file descriptor.

### 7. `pipe()``

Description: Creates a pipe for inter-process communication.

Example:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    return 0;
}
```
Usage: Used to create a pipe for inter-process communication.

### 8. `opendir()` and `readdir()` and `closedir()`

Description: Opens a directory, reads files from it, and closes it.

Example:
```c
#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        printf("Error opening directory\n");
    } else {
        dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    }
    return 0;
}
```
Usage: Used to open a directory, read files from it, and close it.

### 9. `strerror()``

Description: Returns the string corresponding to an error number.

Example:
```c
#include <stdio.h>
#include <errno.h>

int main() {
    int err = -1;
    char *msg = strerror(err);
    if (msg != NULL) {
        printf("Error message: %s\n", msg);
    } else {
        printf("Unknown error\n");
    }
    return 0;
}
```
Usage: Used to return the string corresponding to an error number.

### 10. `perror()``

Description: Prints an error message and exit status.

Example:
```c
#include <stdio.h>
#include <errno.h>

int main() {
    int err = -1;
    perror("Error");
    return 0;
}
```
Usage: Used to print an error message and exit status.

### 11. `isatty()` and `ttyname()` and `ttyslot()` and `ioctl()`

Description: Checks if a file descriptor is a terminal, returns the terminal name, returns the terminal slot number, and performs I/O control
operations.

Example:
```c
#include <stdio.h>
#include <termios.h>

int main() {
    // Check if file descriptor is a terminal
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    if (isatty(fd) == -1) {
        printf("Not a terminal\n");
    }

    char *name = ttyname(0); // Return the terminal name
    if (name != NULL) {
        printf("Terminal name: %s\n", name);
    } else {
        printf("Unknown terminal\n");
    }

    int slot = ttyslot(0); // Return the terminal slot number
    if (slot == -1) {
        printf("Unknown terminal\n");
    } else {
        printf("Terminal slot: %d\n", slot);
    }

    struct termios terms;
    ioctl(fd, TIOCGWINSZ, &terms);
    printf("Terminal size: %dx%d\n", terms.c_winsz.ws_col, terms.c_winsz.ws_row);

    return 0;
}
```
Usage: Used to check if a file descriptor is a terminal, return the terminal name, return the terminal slot number, and perform I/O control
operations.

### 12. `getenv()`

Description: Returns the value of an environment variable.

Example:
```c
#include <stdio.h>

int main() {
    char *var = getenv("PATH");
    if (var != NULL) {
        printf("%s\n", var);
    } else {
        printf("Unknown variable\n");
    }
    return 0;
}
```
Usage: Used to return the value of an environment variable.

### 13. `tcsetattr()` and `tcgetattr()`

Description: Sets or gets the terminal attributes.

Example:
```c
#include <stdio.h>
#include <termios.h>

int main() {
    struct termios terms;

    // Get terminal attributes
    tcgetattr(0, &terms);
    printf("Terminal speed: %d bps\n", terms.c_iflag.iflag.ibrty);

    // Set terminal attributes
    struct termios new_terms;
    new_terms.c_lflag &= ~ICANON; // Turn off canonical mode
    tcsetattr(0, TCSANOW, &new_terms);
}

return 0;
```
Usage: Used to set or get the terminal attributes.

### 14. `tgetent()` and `tgetflag()` and `tgetnum()` and `tgetstr()`

Description: Returns a string, a flag, a number, or a string from the terminal database.

Example:
```c
#include <stdio.h>
#include <termios.h>

int main() {
    char *name = tgetent(NULL);
    if (name != NULL) {
        printf("Terminal name: %s\n", name);
    } else {
        printf("Unknown terminal\n");
    }

    int flag = tgetflag(0); // Flag
    if (flag == -1) {
        printf("Unknown flag\n");
    } else {
        printf("Flag: %d\n", flag);
    }

    int num = tgetnum(0); // Number
    if (num == -1) {
        printf("Unknown number\n");
    } else {
        printf("Number: %d\n", num);
    }

    char *str = tgetstr(0, 10); // String
    if (str != NULL) {
        printf("%s\n", str);
    } else {
        printf("Unknown string\n");
    }
}

return 0;
```
Usage: Used to return a string, a flag, a number, or a string from the terminal database.

### 15. `tgoto()` and `tputs()`

Description: Moves the cursor to a specific position in the terminal screen and prints output to the terminal screen.

Example:
```c
#include <stdio.h>
#include <termios.h>

int main() {
    // Move cursor to 5, 10 positions
    int res = tgoto("SO", 5, 10);
    if (res == -1) {
        printf("Error moving cursor\n");
    }

    // Print output to terminal screen
    char *str = "Hello, World!";
    tputs(str, 0, NULL);
}

return 0;
```
Usage: Used to move the cursor to a specific position in the terminal screen and print output to the terminal screen.

Note: This is not an exhaustive list of functions, but rather a selection of commonly used functions.

