#include "ChatServer.h"
#include "crypto.h"
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/json.h>
#include <fstream>
#include <iostream>

ChatServer::ChatServer(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("", &ChatServer::serve_index, this);
    dispatcher().assign("/js/(.+)", &ChatServer::serve_js, this, 1);
    dispatcher().assign("/css/(.+)", &ChatServer::serve_css, this, 1);
    dispatcher().assign("/img/(.+)", &ChatServer::serve_img, this, 1);

    dispatcher().assign("/api/test", &ChatServer::api_test, this);
    dispatcher().assign("/api/echo", &ChatServer::api_echo, this);
    
    mapper().root("/");
}


// File Loading: Functions responsible for loading static files
void ChatServer::serve_index() {
    response().set_header("Content-Type", "text/html");
    std::ifstream ifs("www/index.html");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    response().out() << content;
}

void ChatServer::serve_js(std::string file_name) {
    serve_static_file("www/js/" + file_name, "application/javascript");
}

void ChatServer::serve_css(std::string file_name) {
    serve_static_file("www/css/" + file_name, "text/css");
}

void ChatServer::serve_img(std::string file_name) {
    serve_static_file("www/img/" + file_name, "text/css");
}


// API Calls: Functions that handle API requests and responses
void ChatServer::api_test() {
    cppcms::json::value response_data;
    response_data["message"] = "API Test";
    response().content_type("application/json");
    std::ostringstream os;
    response_data.save(os, cppcms::json::compact);
    response().out() << os.str();
}

void ChatServer::api_echo() {
    try {
        std::pair<void*, size_t> data = request().raw_post_data();
        std::string raw_post_data((char*)data.first, data.second);

        std::istringstream stream(raw_post_data);
        cppcms::json::value input_data;
        input_data.load(stream, true);

        if (!input_data["message"].is_undefined() && input_data["message"].type() == cppcms::json::is_string) {
            std::string message = input_data["message"].str();

            NTRUCrypto ntru(11, 3, 29);  // Example parameters; ensure they're appropriate
            ntru.generateKeys();
            
            std::vector<int> message_vector(message.begin(), message.end());  // Simple ASCII to int conversion
            std::vector<int> encrypted_message = ntru.encrypt(message_vector, std::vector<int>(message.size(), 1));  // Example simple 'r'

            std::ofstream mdFile("log/encrypted_messages.md", std::ios::app);
            mdFile << message << ": ";
            for (size_t i = 0; i < encrypted_message.size(); ++i) {
                mdFile << encrypted_message[i] << " ";
            }
            mdFile << std::endl;
            mdFile.close();

            std::vector<int> decrypted_message = ntru.decrypt(encrypted_message);
            std::string decrypted_message_str;
            for (size_t i = 0; i < decrypted_message.size(); ++i) {
                decrypted_message_str += static_cast<char>(decrypted_message[i]);  // Ensuring it's within ASCII
            }

            cppcms::json::value output_data;
            output_data["message"] = message;
            response().content_type("application/json");
            output_data.save(response().out(), cppcms::json::compact);
        } else {
            response().make_error_response(400, "Bad Request: Missing or invalid 'message' field");
        }
    }
    catch (std::exception const& e) {
        response().make_error_response(500, "Internal Server Error: " + std::string(e.what()));
    }
}


// Private Functions: Internal and private functions utilized within the code
void ChatServer::serve_static_file(std::string const &path, std::string const &content_type) {
    std::ifstream file(path);
    if (!file) {
        response().status(404);
        response().out() << "File not found";
        return;
    }

    response().content_type(content_type);
    response().out() << file.rdbuf();
}