# C-AI Chat Application

A simple C-based chat application that uses the Gemini Pro API for conversations.

## Prerequisites

- C compiler (gcc recommended)
- libcurl
- json-c
- Python (for frontend server)

### Installing Prerequisites

On Debian-based systems:
```bash
sudo apt-get update
sudo apt-get install gcc libcurl4-openssl-dev libjson-c-dev python3
```

On Red Hat-based systems:
```bash
sudo yum update
sudo yum install gcc libcurl-devel json-c-devel python3
```
### Configuring the Application

Replace the placeholder API key in the `ai.c` file at line 36 with your Gemini Pro API key:
```c
const char *api_key = "your_gemini_pro_api_key_here";
```


## Build and Run

### Backend

```bash
cd backend
make
./server
```

### Frontend

```bash
cd frontend
python -m http.server
```

## Configuration

The application supports:
- Max input size: 20KB
- History storage: 40KB
- Uses Gemini Pro API (30.7K token limit)


