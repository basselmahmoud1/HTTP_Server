#include "clientAPI.h"



void handle_response(char **response, const char *status, char *message )
{
    // Handling the status
    int type =  1 ;
    if(strcmp(message," ") == 0 )
        return;
    char *status_c = NULL;
    printf("\n\n\n%s\n\n",message);

    
    if(strstr(message,"<html>") == NULL )
        type = 0 ;

    if (strcmp(status, "OK") == 0)
    {
        status_c = strdup("200 OK");
    }
    else if (strcmp(status, "Not Found") == 0)
    {
        status_c = strdup("404 Not Found");
    }

    // Handling the message length
    int content_length = strlen(message);
    char copy[2048];
    sprintf(
        copy,
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        status_c , (type == 0)?"text/plain":"text/html" , content_length, message);
    *response = strdup(copy);
}

void handle_message(char *path, char **status_value, char **message, int cfd)
{
    struct stat operation_selector;
    if (stat(path, &operation_selector) == -1)
    {
        perror("SERVER:");
    }
    // check if regular file to cat it's content
    if ((operation_selector.st_mode & S_IFMT) == S_IFREG && !(operation_selector.st_mode & S_IXUSR))
    {
        printf("SERVER: Catting content of the file\n");

        int fd[2];
        if (pipe(fd) == -1)
        {
            perror("SERVER:pipe:");
            return;
        }
        pid_t CPID = fork();
        // parent proccess
        if (CPID > 0)
        {
            int state;
            close(fd[1]);
            char buffer[2048];
            int size = read(fd[0], buffer, 2047);
            if (size >= 0)
            {
                buffer[size] = '\0';
                *message = strdup(buffer);
            }
            else
            {
                perror("Server:read:");
            }
            close(fd[0]);
            CPID = wait(&state);

            if (CPID == -1)
            {
                perror("Server:waiting:");
                return;
            }
        }
        // child proccess
        else if (CPID == 0)
        {

            int state = 0;
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            state = execl("/usr/bin/cat", "cat", path, NULL);

            if (state == -1)
            {
                perror("Server:execution:");
                exit(0);
            }
        }
        else
        {
            perror("Server:fork:");
            return;
        }
    }
    // check if directory to list it's content
    else if ((operation_selector.st_mode & S_IFMT) == S_IFDIR)
    {
        printf("SERVER: Listing content of the Directory\n");
        int fd[2];
        if (pipe(fd) == -1)
        {
            perror("SERVER:pipe:");
            return;
        }
        pid_t CPID = fork();
        // parent proccess
        if (CPID > 0)
        {
            int state;
            close(fd[1]);
            char buffer[2048];
            int size = read(fd[0], buffer, 2047);
            if (size >= 0)
            {
                buffer[size] = '\0';
                *message = strdup(buffer);
            }
            else
            {
                perror("SERVER:read:");
            }
            close(fd[0]);
            CPID = wait(&state);

            if (CPID == -1)
            {
                perror("SERVER:waiting:");
                return;
            }
        }
        // child proccess
        else if (CPID == 0)
        {

            int state = 0;
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            state = execl("/usr/bin/ls", "ls", path, NULL);

            if (state == -1)
            {
                perror("SERVER:execution:");
                exit(0);
            }
        }
        else
        {
            perror("SERVER:fork:");
            return;
        }
    }
    // check if it is an CGI to run it
    else if ((operation_selector.st_mode & S_IFMT) == S_IFREG && (operation_selector.st_mode & S_IXUSR))
    {
        printf("SERVER: Executing CGI script\n");

        int fd[2];
        if (pipe(fd) == -1)
        {
            perror("Server:pipe:");
            return;
        }
        pid_t CPID = fork();
        // parent proccess
        if (CPID > 0)
        {
            int state;
            close(fd[1]);
            char buffer[2048];
            int size = read(fd[0], buffer, 2047);
            if (size >= 0)
            {
                buffer[size] = '\0';
                *message = strdup(buffer);
                char* response ;
                
                handle_response(&response, *status_value, *message); 
                if (write(cfd, response, strlen(response)) != (ssize_t)strlen(response))
                {
                    printf("SERVER: Failed to send response (%s)\n", strerror(errno));
                }
                *message = strdup(" ");
#ifdef DEBUG
                printf("\n\nSERVER: CGI Output:\n%s\n\n", buffer);
#endif
            }
            else
            {
                perror("SERVER:read:");
            }
            close(fd[0]);
            CPID = wait(&state);

            if (CPID == -1)
            {
                perror("SERVER:waiting:");
                return;
            }
        }
        // child proccess
        else if (CPID == 0)
        {

            int state = 0;
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
#ifdef DEBUG
            // char *env_query = getenv("QUERY_STRING");
            // write(fd[1],"\n\n",3);
            // write(fd[1],env_query,strlen(env_query));
            // write(fd[1],"\n\n",3);
#endif
            state = execl(path, path, NULL);

            if (state == -1)
            {
                perror("SERVER:execution:");
                exit(0);
            }
        }
    }
    else
    {
        *message = strdup("<html><body>\n"
            "<h1><p><pre>&nbsp;&nbsp;Error 404 !!</pre></p></h1>\n"
            "</body></html>\n"
            );
        *status_value = strdup("Not Found");
    }
}

void handle_client(int cfd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;
    char *status_value = "OK";
    // type = 0 ; // is variable is responsible for the formate "HTML" OR "Plain-text"

    // Read the client's request
    numRead = read(cfd, buf, BUF_SIZE - 1);
    if (numRead < 0)
    {
        printf("SERVER: read failed (%s)\n", strerror(errno));
        close(cfd);
        return;
    }

    buf[numRead] = '\0';
    printf("SERVER: Received request:\n%s\n", buf);

    // Parse the HTTP request
    char method[16], path[256], version[16];
    if (sscanf(buf, "%15s %255s %15s", method, path, version) != 3)
    {
        printf("SERVER: Invalid HTTP request\n");
        close(cfd);
        return;
    }
    char *query = strchr(path, '?');
    if (query != NULL)
    {
        *query = '\0';
        query++;
        setenv("QUERY_STRING", query, 1);
    }

    printf("SERVER: Method: %s, Path:%s, Version:%s ,QUERY_STRING: %s \n", method, path, version, getenv("QUERY_STRING"));

    char *message = "NULL";
    if (strcmp(path, "/favicon.ico") == 0)
    {
        status_value = strdup("Not Found");
        message = strdup(" ");
    }

    handle_message(path, &status_value, &message, cfd);
    // Form a simple HTTP response
    // char *response = (char *)malloc(2048); // Allocate enough space for the response

    // if (response == NULL) {
    //     printf("SERVER: Memory allocation failed\n");
    //     close(cfd);
    //     return;
    // }
    char *response = NULL;
    handle_response(&response, status_value, message);///////////////////////////////////////
    // "HTTP/1.1 200 OK\r\n"
    // "Content-Type: text/plain\r\n"
    // "Content-Length: 24\r\n"
    // "\r\n"
    // "bassel send hello from C";
    // handle_response (response);

    if(response != NULL)
    {
        if (write(cfd, response, strlen(response)) != (ssize_t)strlen(response))
        {
            printf("SERVER: Failed to send response (%s)\n", strerror(errno));
        }
    }

    close(cfd); // Close the connection
    printf("SERVER: Connection closed\n");
}
