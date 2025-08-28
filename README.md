File Uploader
=============

A multi-threaded command-line file uploader for the Progress MOVEit Transfer API.
Supports multiple users, automatic token management, and a request queue for orderly file uploads.

------------------------------------------------------

Quick Start (Linux)
------------------

Prerequisites:

sudo apt-get install cmake g++ git

Dependencies:
- nlohmann/json (https://github.com/nlohmann/json)
- cpr (https://github.com/libcpr/cpr) - C++ HTTP requests library
  > These are included via CMake as submodules.

Build & Run:

git clone https://github.com/paveltodorov/CppFileUploader.git

cd CppFileUploader

mkdir build && cd build

cmake ..

make

./file_uploader

------------------------------------------------------

Configuration
-------------

You can set the API URL in config.json:

{
    "transferApiUrl": "https://testserver.moveitcloud.com/api/v1"
}

------------------------------------------------------

Usage
-----

The application runs as a command-line interface:

> upload <username> <password> <file_path>       # Upload a file for a user
> status                                         # Show the request queue status
> help                                           # Show available commands
> quit / exit                                    # Exit the application

------------------------------------------------------

Features
--------

- Handles multiple users with individual credentials.
- Maintains a request queue to manage uploads.
- Automatically refreshes expired tokens.
- Supports uploading files to the user’s home folder on MOVEit Transfer.
- Designed with concurrency in mind (serial queue; can be extended to multithreading).

------------------------------------------------------

Notes
-----

- File paths: Provide the full or relative path to the file you want to upload.
- Queue: Requests are processed in order. Multiple requests can be added without waiting for previous uploads to finish.
- Error handling: Basic HTTP and API error responses are displayed in the console.

------------------------------------------------------

Troubleshooting
---------------

- If CMake fails to find cpr or libcurl, ensure libcurl is installed on your system:

sudo apt-get install libcurl4-openssl-dev

- Make sure you are providing valid credentials when uploading files.
- Disable CORS in Chrome if testing API calls via the browser.

------------------------------------------------------

Future improvements
------------------

- Thread Pool & Concurrency;
Currently, upload tasks are processed serially.
Using a fixed-size thread pool and a thread-safe queue would allow multiple requests to be handled concurrently.

- Retry Mechanism & Exponential Backoff;
Implementing retries for failed uploads with exponential backoff would make the system more robust to network/API failures.

- Batch Uploads;
If multiple files need to be uploaded at once, batching could reduce network overhead and improve efficiency.

- Token Revocation;
Tokens could be revoked on application shutdown for improved security, though current implementation relies on token expiry.

- Scalability;
For larger user bases, caching, connection pooling, and more advanced concurrency mechanisms (e.g., lock-free queues) could be considered.

