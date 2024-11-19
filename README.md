# HTTP Server with CGI Support  

This project is a lightweight HTTP server implemented in C. It can handle HTTP requests, serve files, execute CGI scripts, and dynamically generate responses in either HTML or plain text format.  

## Features  

1. **File Handling**  
   - Serves static files, such as plain text or HTML.  
   - Lists the contents of directories dynamically.  

2. **CGI Support**  
   - Executes CGI scripts and returns their output as part of the HTTP response.  
   - Parses and passes query strings from the HTTP request to the CGI scripts.  

3. **Error Handling**  
   - Responds with a `404 Not Found` error for missing or inaccessible files.  

4. **Dynamic Content-Type Support**  
   - Determines the content type (HTML or plain text) based on the response format.  

5. **Standards-Compliant HTTP Responses**  
   - Generates valid HTTP headers, including `Content-Type` and `Content-Length`.  

## How It Works  

1. **Request Parsing**  
   - The server reads the HTTP request and extracts the method, path, and query string.  

2. **Response Generation**  
   - **Static File**: Reads and sends file content.  
   - **Directory**: Lists directory contents using the `ls` command.  
   - **CGI Execution**: Runs executable files, captures their output, and includes it in the HTTP response.  

3. **Dynamic Header and Content Generation**  
   - Constructs headers dynamically based on the response type (HTML or plain text).  
   - Adjusts the `Content-Length` header based on the size of the response body.  

4. **Error Responses**  
   - Generates a custom HTML page for `404 Not Found` errors.  

## Installation  

### Prerequisites  

- Linux-based operating system.  
- GCC or another C compiler.  

### Compilation  

```bash  
gcc -o server server.c clientAPI.c
./ server <"IP Address"> <"Port number">
```
 ### Usage
 - Place static files (HTML or text) and CGI scripts in the server's working directory.
 - Access the server from a browser or an HTTP client (e.g., `curl`).
## Known Limitations  

1. **Limited HTTP Methods**  
   - The server currently supports only `GET` requests. Other HTTP methods like `POST`, `PUT`, or `DELETE` are not implemented.  

2. **Static File Handling**  
   - Only regular files and directories are supported. Symlinks and other file types are not handled.  

3. **Basic Error Handling**  
   - Errors like invalid paths or unsupported requests return a generic `404 Not Found` response, with limited logging or debugging information.  

4. **CGI Environment Variables**  
   - Only the `QUERY_STRING` environment variable is passed to CGI scripts. Other HTTP environment variables, such as `HTTP_USER_AGENT` or `HTTP_REFERER`, are not supported.  

5. **Single-Threaded Design**  
   - The server processes one connection at a time, which may lead to delays when handling multiple concurrent requests.  

6. **Content-Type Detection**  
   - Content types are determined statically based on the response message content. There is no dynamic MIME type detection based on file extensions.
  ## Future Enhancements  

1. **Persistent Connections**  
   - Implement HTTP persistent connections to keep the connection alive for multiple requests, improving performance.  

2. **Additional HTTP Methods**  
   - Add support for methods like `POST`, `PUT`, and `DELETE` to make the server more versatile.  

3. **Advanced Content-Type Detection**  
   - Use file extensions or file headers to improve the accuracy of `Content-Type` detection.  

4. **Improved Error Handling**  
   - Enhance error reporting with detailed logs and user-friendly error pages.  

5. **Scalability**  
   - Introduce multithreading or asynchronous processing to handle multiple simultaneous client requests efficiently.  

6. **Configuration File Support**  
   - Allow customization of server settings such as port, document root, and error pages through a configuration file.  

7. **Security Enhancements**  
   - Add features like HTTPS support, input validation, and protection against common web vulnerabilities (e.g., directory traversal, injection attacks).
    
8. **File Upload Support**  
    - Enable file uploads via `POST` requests for dynamic applications.

### Author
Developed by Bassel Mahmoud.

  
