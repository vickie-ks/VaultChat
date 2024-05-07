# VaultChat
A secure, enterprise-grade chat system designed to facilitate encrypted communications within organizations, ensuring confidentiality, integrity, and availability of data.

## Features

- **End-to-End Encryption**: Messages are encrypted on the sender's side and decrypted on the receiver's side, ensuring no readable messages are transmitted over the network.
- **CLI-Based Interface**: Easy to use command-line interface for both starting the server and connecting as a client.
- **Logging**: Integrated logging to help with troubleshooting and understanding application flow.
- **Robust Error Handling**: Graceful handling of common issues such as network errors or encryption failures.

## Prerequisites

Before you run VaultChat, you need to have Python installed on your machine. Python 3.6 or higher is recommended. You can download it from [python.org](https://www.python.org/downloads/).

## Installation

Clone the VaultChat repository:

```bash
git clone https://github.com/yourgithubusername/VaultChat.git
cd VaultChat
```

Install the required Python libraries:

```bash
pip install -r requirements.txt
```

## Usage

To start the server:

```bash
python server.py
```

To connect as a client from another terminal:

```bash
python client.py
```

## Encryption Details

VaultChat uses symmetric encryption provided by the `cryptography` library's Fernet scheme. At the beginning of a session, the server generates a key and shares it with the client securely using a simplified key exchange protocol.

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.