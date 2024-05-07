import socket
import sys
from cryptography.fernet import Fernet

def create_socket_connection(host, port):
    """Establishes a socket connection to the given host and port."""
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((host, port))
        return client_socket
    except socket.error as e:
        print(f"Failed to connect to {host}:{port}, error: {e}")
        sys.exit()

def receive_key(socket):
    """Receives the encryption key from the server."""
    key = socket.recv(1024)
    return Fernet(key)

def send_and_receive_messages(socket, cipher_suite):
    """Handles sending and receiving messages."""
    try:
        while True:
            msg = input("Enter a message: ")
            encrypted_msg = cipher_suite.encrypt(msg.encode())
            socket.send(encrypted_msg)

            encrypted_msg = socket.recv(1024)
            if not encrypted_msg:
                print("Connection closed by the server.")
                break

            decrypted_msg = cipher_suite.decrypt(encrypted_msg)
            print("Received:", decrypted_msg.decode())
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        socket.close()

def main(host, port):
    """Main function to run the chat client."""
    client_socket = create_socket_connection(host, port)
    cipher_suite = receive_key(client_socket)
    send_and_receive_messages(client_socket, cipher_suite)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <Host> <Port>")
        sys.exit()

    host = sys.argv[1]
    try:
        port = int(sys.argv[2])
    except ValueError:
        print("Port must be an integer.")
        sys.exit()

    main(host, port)
