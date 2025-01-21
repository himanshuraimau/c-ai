function appendMessage(message, isUser) {
    const chatBox = document.getElementById('chatBox'); // Chat box element
    const messageDiv = document.createElement('div'); // New message div
    messageDiv.className = `message ${isUser ? 'user-message' : 'bot-message'}`; // Set class based on user or bot
    messageDiv.textContent = message;

    chatBox.appendChild(messageDiv); // Append message to chat box
    chatBox.scrollTop = chatBox.scrollHeight; // Scroll to the bottom
}

async function sendMessage() {
    const input = document.getElementById('userInput'); // User input element
    const message = input.value.trim();
    
    if (message === '') return;
    
    appendMessage(message, true); // Append user message
    input.value = '';

    try {
        const response = await fetch('http://localhost:8080/chat', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ message: message }) // Send message to server
        });
        
        const data = await response.json();
        const aiText = data.response.replace(/^Assistant:\s*/g, '').trim(); // Clean up AI response
        appendMessage(aiText, false); // Append AI response
    } catch (error) {
        appendMessage('Error: Could not connect to server', false); // Handle error
        console.error('Error:', error);
    }
}

document.getElementById('userInput').addEventListener('keypress', function(e) {
    if (e.key === 'Enter') {
        sendMessage(); // Send message on Enter key press
    }
});
