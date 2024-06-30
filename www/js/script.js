document.addEventListener("DOMContentLoaded", () => {
  const aliceInput = document.getElementById('input-alice');
  const bobInput = document.getElementById('input-bob');
  const aliceChatBox = document.getElementById('chat-box-alice');
  const bobChatBox = document.getElementById('chat-box-bob');

  // Add default messages
  addMessageToChat('Alice', 'Welcome to the chat, Alice!', aliceChatBox, 'bg-light', 'start');
  addMessageToChat('Bob', 'Welcome to the chat, Bob!', bobChatBox, 'bg-light', 'start');

  document.getElementById('send-alice').addEventListener('click', () => sendMessage('Alice', aliceInput.value, aliceChatBox, bobChatBox));
  document.getElementById('send-bob').addEventListener('click', () => sendMessage('Bob', bobInput.value, bobChatBox, aliceChatBox));
  
  aliceInput.addEventListener('keypress', (event) => {
    if (event.key === 'Enter') {
      event.preventDefault();
      sendMessage('Alice', aliceInput.value, aliceChatBox, bobChatBox);
    }
  });
  
  bobInput.addEventListener('keypress', (event) => {
    if (event.key === 'Enter') {
      event.preventDefault();
      sendMessage('Bob', bobInput.value, bobChatBox, aliceChatBox);
    }
  });

  async function sendMessage(sender, message, senderChatBox, receiverChatBox) {
    message = message.trim();
    if (message === "") return;

    try {
      const response = await fetch('/api/echo', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ message }),
      });

      if (response.ok) {
        const data = await response.json();
        addMessageToChat(sender, data.message, senderChatBox, 'bg-success text-white', 'end');
        addMessageToChat(sender, data.message, receiverChatBox, 'bg-light', 'start', true);
      } else {
        addMessageToChat('Error', 'Error sending message', senderChatBox, 'bg-danger text-white', 'start');
      }
    } catch (error) {
      addMessageToChat('Error', 'Network error', senderChatBox, 'bg-danger text-white', 'start');
    }

    if (sender === 'Alice') {
      aliceInput.value = '';
    } else {
      bobInput.value = '';
    }

    senderChatBox.scrollTop = senderChatBox.scrollHeight;
    receiverChatBox.scrollTop = receiverChatBox.scrollHeight;
  }

  function addMessageToChat(sender, message, chatBox, bgClass, justify, fromOtherUser = false) {
    const messageElement = document.createElement('div');
    messageElement.classList.add('d-flex', `justify-content-${justify}`, 'mb-2');
    messageElement.innerHTML = `<div class="p-2 ${bgClass} rounded-pill mb-1">${fromOtherUser ? sender + ': ' : ''}${message}</div>`;
    chatBox.appendChild(messageElement);
  }
});
