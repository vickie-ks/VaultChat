import socket
import sys
from cryptography.fernet import Fernet

def create_server_socket(host, port):
    """Creates and binds the server socket."""
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((host, port))
        return server_socket
    except socket.error as e:
        print(f"Failed to create or bind the server socket on {host}:{port}, error: {e}")
        sys.exit()

def accept_client_connection(server_socket):
    """Accepts a client connection."""
    try:
        server_socket.listen(1)
        client_socket, addr = server_socket.accept()
        print(f"Connection established with {addr}")
        return client_socket
    except socket.error as e:
        print("Error accepting connection from client:", e)
        sys.exit()

def handle_client_communication(client_socket, cipher_suite):
    """Handles the communication with the client."""
    try:
        while True:
            encrypted_msg = client_socket.recv(1024)
            if not encrypted_msg:
                print("Connection closed by client.")
                break

            decrypted_msg = cipher_suite.decrypt(encrypted_msg)
            print("Received:", decrypted_msg.decode())

            msg = input("Enter a message: ")
            encrypted_msg = cipher_suite.encrypt(msg.encode())
            client_socket.send(encrypted_msg)
    except Exception as e:
        print(f"An error occurred during communication: {e}")
    finally:
        client_socket.close()

def main(host, port):
    """Main function to run the chat server."""
    server_socket = create_server_socket(host, port)
    client_socket = accept_client_connection(server_socket)
    key = Fernet.generate_key()
    cipher_suite = Fernet(key)
    client_socket.send(key)
    handle_client_communication(client_socket, cipher_suite)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python server.py <Host> <Port>")
        sys.exit()

    host = sys.argv[1]
    try:
        port = int(sys.argv[2])
    except ValueError:
        print("Port must be an integer.")
        sys.exit()

    main(host, port)
