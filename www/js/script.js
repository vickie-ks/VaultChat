function sendMessage(side) {
  var input = document.getElementById('input-' + side);
  var message = input.value.trim();
  if (message === '') return;

  fetch('api/echo', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ message: message })
  })
  .then(response => response.json())
  .then(data => {
    if (!data.message) {
      alert('Invalid response format!');
      throw new Error('Invalid response format');
    }

    // Display the message on the same side
    var sameSide = document.getElementById('messages-' + side);
    var messageElement = getMsgEle(data.message);  // Assuming response JSON has {message: 'text'}
    messageElement.className = 'message-right';
    sameSide.appendChild(messageElement);
    var newEle = document.createElement('div');
    newEle.style = 'padding: 23px;';
    sameSide.appendChild(newEle);

    // Display the message on the opposite side
    var oppositeSide = document.getElementById('messages-' + revSide(side));
    var messageElementOpposite = getMsgEle(data.message);
    messageElementOpposite.className = 'message-left';
    oppositeSide.appendChild(messageElementOpposite);
    var newEleOpposite = document.createElement('div');
    newEleOpposite.style = 'padding: 23px;';
    oppositeSide.appendChild(newEleOpposite);
  })
  .catch(error => {
    console.error('Error sending message:', error);
  });

  // Clear input
  input.value = '';
}

function revSide(side) {
  return side === 'left' ? 'right' : 'left';
}

function getMsgEle(message) {
  var messageElement = document.createElement('div');
  messageElement.textContent = message;
  
  return messageElement;
}

window.onload = function () {
  document.getElementById('input-left').addEventListener('keyup', function (event) {
    if (event.key === 'Enter') sendMessage('left');
  });

  document.getElementById('input-right').addEventListener('keyup', function (event) {
    if (event.key === 'Enter') sendMessage('right');
  });
};
