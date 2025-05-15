## #1. `readline`

	***Purpose : ** `readline` is used to read input from the user while keeping a history of previous commands.
						***Syntax : **Not applicable; it's a function in Bash.
*   **Example:**
    ```bash
$ readline -s "Enter your name: "
```
    This will prompt the user for their name and store it in the `NAME` variable.

### 2. `rl_clear_history`

*   **Purpose:** Clears the command history.
*   **Syntax:** `rl_clear_history()`
*   **Example:**
    ```bash
$ rl_clear_history
```
    This will clear all previous commands from the history.

### 3. `rl_on_new_line`

*   **Purpose:** Allows a function to be called when the newline character is pressed.
*   **Syntax:** `rl_on_new_line(func)`, where `func` is the callback function.
*   **Example:**
    ```bash
$ rl_on_new_line() {
  echo "New line"
}

$ readline -s "Enter your name: "
```
    In this example,
	whenever a newline character is pressed while reading input,
	it will print "New line".

### 4. `rl_replace_line`

*   **Purpose:** Replaces the current line with a new one.
*   **Syntax:** `rl_replace_line(line)`,
	where `line` is the new line to be entered.
*   **Example:**
    ```bash
$ rl_replace_line("Hello, World!")
```
    This will replace the current input line with "Hello, World!".

### 5. `rl_redisplay`

*   **Purpose:** Updates the terminal display to reflect changes in the user's input.
*   **Syntax:** `rl_redisplay()`
*   **Example:**
    ```bash
$ rl_redisplay()
```
    This will update the terminal display after a change.

### 6. `add_history`

*   **Purpose:** Adds a new command to the history.
*   **Syntax:** `add_history(command)`,
	where `command` is the new command to be added.
*   **Example:**
    ```bash
$ add_history "ls -l"
```
    This will store "ls -l" in the command history.

### 7. `printf`

*   **Purpose:** Formats and prints data to the standard output.
*   **Syntax:** `printf(format, ...)` ,
	where `format` is a format string and `...` are arguments.
*   **Example:**
    ```bash
$ printf "Hello, World! \n"
```
    This will print "Hello, World!" followed by a newline character.

### 8. `malloc`

*   **Purpose:** Allocates memory for use in the program.
*   **Syntax:** `malloc(size)`,
	where `size` is the amount of memory to be allocated.
*   **Example:**
    ```c
#include <stdlib.h>

int	main(void)
{
	int	*ptr;

	ptr = malloc(sizeof(int));
	if (ptr == NULL)
	{
		printf("Memory allocation failed\n");
		return (-1);
	}
	*ptr = 10;
	free(ptr);
	return (0);
}
```
    This will allocate memory for an integer and store the value 10 in it.

### 9. `free`

*   **Purpose:** Releases memory allocated using `malloc`.
*   **Syntax:** `free(ptr)`, where `ptr` is a pointer to the memory block.
*   **Example:**
    ```c
#include <stdlib.h>

int	main(void)
{
	int	*ptr;

	ptr = malloc(sizeof(int));
	if (ptr == NULL)
	{
		printf("Memory allocation failed\n");
		return (-1);
	}
	*ptr = 10;
	free(ptr);
	return (0);
}
```
    This will release the memory allocated for an integer.

### 10. `write`

*   **Purpose:** Writes data to a file descriptor.
*   **Syntax:** `write(fd, buf, size)`,
	where `fd` is the file descriptor and `buf` and `size` are the data buffer and size of the data to be
written respectively.
*   **Example:**
    ```c
#include <unistd.h>

int	main(void)
{
	int		fd;
	char	buf[];
	ssize_t	bytes_written;

	fd = open("file.txt", O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		perror("Failed to create file");
		return (-1);
	}
	buf[] = "Hello, World!";
	bytes_written = write(fd, buf, strlen(buf));
	close(fd);
	return (0);
}
```
    This will write "Hello, World!" to a new file named "file.txt".

### 11. `access`

*   **Purpose:** Checks if the specified path exists or not.
*   **Syntax:** `access(path, mode)`,
	where `path` is the path and `mode` is the check type (e.g.,
		`R_OK` for readable).
*   **Example:**
    ```c
#include <sys/types.h>
#include <unistd.h>

int	main(void)
{
	char	path[];
	int		mode;

	path[] = "/home/user";
	mode = R_OK | W_OK;
	if (access(path, mode) == 0)
	{
		printf("The path exists and is readable.\n");
	}
	return (0);
}
```
    This will check if the specified path exists and is readable.

### 12. `open`

*   **Purpose:** Opens a file for reading or writing.
*   **Syntax:** `open(path, flags)`,
	where `path` is the path to be opened and `flags` are the access mode (e.g.,
		`O_RDONLY` for read-only).
*   **Example:**
    ```c
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	path[];
	int		fd;

	path[] = "file.txt";
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (-1);
	}
	printf("File opened successfully.\n");
	return (0);
}
```
    This will open a file named "file.txt" for reading.

### 13. `read`

*   **Purpose:** Reads data from the file descriptor.
*   **Syntax:** `read(fd, buf, size)`,
	where `fd` is the file descriptor and `buf` and `size` are the data buffer and size of the data to be
read respectively.
*   **Example:**
    ```c
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	buf[1024];
	ssize_t	bytes_read;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (-1);
	}
	bytes_read = read(fd, buf, sizeof(buf));
	printf("%s\n", buf);
	close(fd);
	return (0);
}
``` This will read data from a file named "file.txt" and print it to the standard output.

															## #14. `close`

																***Purpose : **Closes the file descriptor.*
		**Syntax : ** `close(fd)`,
	where `fd` is the file descriptor.
		***Example : **
    ```c
#include <stdio.h>

						int main()
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (-1);
	}
	printf("File opened successfully.\n");
	close(fd);
	return (0);
}
``` This will close the file descriptor of a file named "file.txt".

	## #15. `fork`

		***Purpose : **Creates a new process by duplicating an existing one.
							***Syntax : ** `fork()` .
											***Example : **
    ```c
#include <stdio.h>
#include <unistd.h>

														int main()
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process.\n");
	}
	else
	{
		printf("This is the parent process.\n");
	}
	return (0);
}
```
    This will create a new process by duplicating an existing one.

### 16. `wait`

*   **Purpose:** Waits for the specified process to finish execution.
*   **Syntax:** `wait(pid)` ,
	where `pid` is the ID of the process to be waited on.
*   **Example:**
    ```c
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process.\n");
		sleep(2);
		exit(0);
	}
	else
	{
		printf("This is the parent process. Waiting for the child...\n");
		wait(NULL);
		printf("Child finished execution.\n");
	}
	return (0);
}
```
    This will create a new process, let it execute for some time,
	and then wait for its completion.

### 17. `waitpid`

*   **Purpose:** Waits for the specified process to finish execution.
*   **Syntax:** `waitpid(pid, status, options)`,
	where `pid` is the ID of the process to be waited on,
	`status` is a pointer to store the
exit status of the process,
	and `options` are flags specifying how to handle signals.
*   **Example:**
    ```c
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process.\n");
		sleep(2);
		exit(0);
	}
	else
	{
		printf("This is the parent process. Waiting for the child...\n");
		waitpid(pid, NULL, 0); // Wait and return without printing anything
		printf("Child finished execution.\n");
	}
	return (0);
}
```
    This will create a new process, let it execute for some time,
	and then wait for its completion.

### 18. `wait3`

*   **Purpose:** Waits for the specified process to finish execution.
*   **Syntax:** `wait3(status, wstatus, &wppid)`,
	where `status` is a pointer to store the exit status of the process,
	`wstatus` is an
integer value used for system status,
	and `&wppid` is a pointer to store the ID of the child whose status was reported.
*   **Example:**
    ```c
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process.\n");
		sleep(2);
		exit(0);
	}
	else
	{
		printf("This is the parent process. Waiting for the child...\n");
		wait3(NULL, NULL, NULL); // Wait and return without printing anything
		printf("Child finished execution.\n");
	}
	return (0);
}
```
    This will create a new process, let it execute for some time,
	and then wait for its completion.

### 19. `wait4`

*   **Purpose:** Waits for the specified process to finish execution.
*   **Syntax:** `wait4(status, wstatus, option, &wppid)`,
	where `status` is a pointer to store the exit status of the process,
	`wstatus` is
an integer value used for system status,
	`option` specifies how to handle signals,
	and `&wppid` is a pointer to store the ID of the child
whose status was reported.
*   **Example:**
    ```c
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process.\n");
		sleep(2);
		exit(0);
	}
	else
	{
		printf("This is the parent process. Waiting for the child...\n");
		wait4(NULL, NULL, 0, NULL); // Wait and return without printing anything
		printf("Child finished execution.\n");
	}
	return (0);
}
```
    This will create a new process, let it execute for some time,
	and then wait for its completion.

### 20. `signal`

*   **Purpose:** Sends a signal to the specified process.
*   **Syntax:** `signal(sig, handler)`,
	where `sig` is the signal number and `handler` is a function pointer specifying how to handle the
signal.
*   **Example:**
    ```c
#include <stdio.h>
#include <stdlib.h>

int	my_handler(int sig)
{
	printf("Received signal %d.\n", sig);
}

int	main(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create process");
		return (-1);
	}
	if (pid == 0)
	{
		printf("This is the child process. Handling signals...\n");
		signal(SIGINT, my_handler); // Install handler for SIGINT
		sleep(2);
		exit(0);
	}
	else
	{
		printf("This is the parent process. Waiting for the child...\n");
		pause(); // Wait indefinitely to see the output in the child
	}
	return (0);
}
```
    This will create a new process, let it execute for some time,
	and then wait for its completion.

### 21. `access`

*   **Purpose:** Checks if the specified path exists or not.
*   **Syntax:** `access(path, mode)`,
	where `path` is the path to be checked and `mode` are the check types (e.g.,
		`R_OK` for readable).
*   **Example:**
    ```c
#include <stdio.h>
#include <sys/types.h>

int	main(void)
{
	char	path[];
	int		mode;

	path[] = "/home/user";
	mode = R_OK | W_OK;
	if (access(path, mode) == 0)
	{
		printf("The path exists and is readable.\n");
	}
	return (0);
}
```
    This will check if the specified path exists and is readable.

### 22. `open`

*   **Purpose:** Opens a file for reading or writing.
*   **Syntax:** `open(path, flags)`,
	where `path` is the path to be opened and `flags` are the access modes (e.g.,
		`O_RDONLY` for read-only).
*   **Example:**
    ```c
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	path[];
	int		fd;

	path[] = "file.txt";
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (-1);
	}
	printf("File opened successfully.\n");
	return (0);
}
```
    This will open a file named "file.txt" for reading.

### 23. `read`

*   **Purpose:** Reads data from the file descriptor.
*   **Syntax:** `read(fd, buf, size)`,
	where `fd` is the file descriptor and `buf` and `size` are the data buffer and size of the data to be
read respectively.
*   **Example:**
    ```c
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	buf[1024];
	ssize_t	bytes_read;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (-1);
	}
	bytes_read = read(fd, buf, sizeof(buf));
	printf("%s\n", buf);
	close(fd);
	return (0);
}
``` This will read data from a file named "file.txt" and print it to the standard output.

															## #24. `write`

																***Purpose : **Writes data to a file descriptor.*
		**Syntax : ** `write(fd, buf, size)`,
	where `fd` is the file descriptor and `buf` and `size` are the data buffer and size of the data to be
	written respectively.
		***Example : **
    ```c
#include <stdio.h>

						int main()
{
	int		fd;
	char	buf[];
	ssize_t	bytes_written;

	fd = open("file.txt", O_WRONLY | O_CREAT);
	if (fd == -1)
	{
		perror("Failed to create file");
		return (-1);
	}
	buf[] = "Hello, World!";
	bytes_written = write(fd, buf, strlen(buf));
	printf("%zu bytes written.\n", bytes_written);
	close(fd);
	return (0);
}
``` This will write data to a new file named "file.txt".

	## #25. `write`

		***Purpose : **Writes data to the standard output.
							***Syntax : ** `write(STDOUT_FILENO, buf, size)`,
	where `buf` and `size` are the data buffer and size of the data to be written
	respectively.
		***Example : **
    ```c
#include <stdio.h>

						int main()
{
	char	buf[];
	ssize_t	bytes_written;

	buf[] = "Hello, World!";
	bytes_written = write(STDOUT_FILENO, buf, strlen(buf));
	printf("%zu bytes written.\n", bytes_written);
	return 0;
}
``` This will write data to the standard output.

	## #26. `write`

		***Purpose : **Writes an array of characters to a file descriptor.
							***Syntax : ** `write(fd, buf, size)`,
	where `fd` is the file descriptor and `buf` and `size` are the data buffer and size of the data to be
	written respectively.
		***Example : **
    ```c
#include <stdio.h>

						int main()
{
	int		fd;
	char	buf[1024];
	ssize_t	bytes_written;

	fd = open("file.txt", O_WRONLY | O_CREAT);
	if (fd == -1)
	{
		perror("Failed to create file");
		return -1;
	}
	buf[1024] = "Hello, World!";
	bytes_written = write(fd, buf, sizeof(buf));
	printf("%zu bytes written.\n", bytes_written);
	close(fd);
	return 0;
}
``` This will write data to a file named "file.txt".

	## #27. `write`

		***Purpose : **Writes an array of characters to the standard output.
							***Syntax : ** `write(STDOUT_FILENO, buf, size)`,
	where `buf` and `size` are the data buffer and size of the data to be written
	respectively.
		***Example : **
    ```c
#include <stdio.h>

						int main()
{
	char	buf[1024];
	ssize_t	bytes_written;

	buf[1024] = "Hello, World!";
	bytes_written = write(STDOUT_FILENO, buf, sizeof(buf));
	printf("%zu bytes written.\n", bytes_written);
	return 0;
}
```
    This will print data to the standard output.

### 28. `fopen`

*   **Purpose:** Opens a file for reading or writing.
*   **Syntax:** `fopen(path, mode)`,
	where `path` is the path to be opened and `mode` are the access modes (e.g.,
		`"r"` for read-only).
*   **Example:**
    ```c
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	path[];
	FILE	*fp;

	path[] = "file.txt";
	fp = fopen(path, "w");
	if (fp == NULL)
	{
		perror("Failed to open file");
		return -1;
	}
	fprintf(fp, "Hello, World!");
	fclose(fp);
	return 0;
}
```
    This will open a new file named "file.txt" for writing and print data to it.
