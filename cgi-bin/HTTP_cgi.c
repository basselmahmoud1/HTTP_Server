#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *query = getenv("QUERY_STRING");
    


    printf("Content-Type: text/html\r\n\r\n");

    printf("<html><body>\n");
    printf("<h1>Welcome to My CGI Program!</h1>\n");
    if (query) {
        printf("<p>Query_String: %s</p>\n", query);
    } else {
        printf("<p>No query  received.</p>\n");
    }
     if (query) {
        char *query_copy = strdup(query);  
        char *token = strtok(query_copy, "&");
        int i = 0;
        while (token) {

            char *equal_pos = strchr(token, '=');
            if (equal_pos) {
                *equal_pos = '\0';
                char *argv = token;
                char *value = equal_pos + 1;

                // Print the key and its corresponding value
                printf("<p>argv[%d]: %s, Value[%d]: %s</p>\n",i,argv,i, value);
                i++;
            }
            token = strtok(NULL, "&");  // Get the next key-value pair
        }

        free(query_copy);  // Free the memory allocated for the copied query string
    } else {
        printf("<p>No query string .</p>\n");
    }
    printf("</body></html>\n");

    return 0;
}
