# C-Gemini Chatbot: A C-Based Chat Application with Gemini Pro API Integration

 
*A lightweight and efficient chatbot built in C, leveraging the power of the Gemini Pro API for natural language processing and conversational AI.*

---

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Support](#support)

---

## Features
- **Lightweight and Fast**: Built in C for optimal performance.
- **Gemini Pro API Integration**: Leverages the Gemini Pro API for advanced conversational AI.
- **Cross-Platform**: Works on Linux, macOS, and Windows (with minor adjustments).
- **Customizable**: Easily modify the chatbot's behavior and responses.
- **History Storage**: Supports up to 40KB of conversation history.
- **Simple Frontend**: Includes a Python-based web interface for easy interaction.

---

## Prerequisites
To run this application, you'll need the following:
- **C Compiler**: GCC recommended.
- **libcurl**: For making HTTP requests to the Gemini Pro API.
- **json-c**: For parsing JSON responses from the API.
- **Python 3**: For running the frontend server.

---

## Installation
### On Debian-based systems:
```bash
sudo apt-get update
sudo apt-get install gcc libcurl4-openssl-dev libjson-c-dev python3
```

### On Red Hat-based systems:
```bash
sudo yum update
sudo yum install gcc libcurl-devel json-c-devel python3
```

---

## Configuration
1. Replace the placeholder API key in the `ai.c` file at line 36 with your Gemini Pro API key:
   ```c
   const char *api_key = "your_gemini_pro_api_key_here";
   ```
2. Adjust the following settings in `ai.c` if needed:
   - Max input size: 20KB
   - History storage: 40KB

---

## Usage
### Backend
1. Navigate to the `backend` directory:
   ```bash
   cd backend
   ```
2. Build the application:
   ```bash
   make
   ```
3. Run the server:
   ```bash
   ./server
   ```

### Frontend
1. Navigate to the `frontend` directory:
   ```bash
   cd frontend
   ```
2. Start the Python server:
   ```bash
   python -m http.server
   ```
3. Open your browser and navigate to `http://localhost:8000` to interact with the chatbot.

---

## Contributing
Contributions are welcome! Here's how you can help:
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Submit a pull request with a detailed description of your changes.

Please ensure your code follows the existing style and includes appropriate documentation.



---

## Support
If you find this project useful, please consider giving it a ⭐️ on GitHub! For questions or feedback, open an issue or reach out to [your email or social media handle].

---

## FAQ
### How do I get a Gemini Pro API key?
Visit the [Gemini Pro website](https://aistudio.google.com/) to sign up and obtain an API key.

### Can I use this on Windows?
Yes, but you may need to adjust the build process. Contributions for Windows support are welcome!

---

## Acknowledgments
- [Gemini Pro API](https://aistudio.google.com) for providing the conversational AI backend.
- [libcurl](https://curl.se/libcurl/) and [json-c](https://github.com/json-c/json-c) for enabling HTTP requests and JSON parsing in C.