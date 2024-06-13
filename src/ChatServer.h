#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <cppcms/application.h>

class ChatServer : public cppcms::application {
public:
  ChatServer(cppcms::service &srv);
  void serve_js(std::string file_name);
  void serve_css(std::string file_name);
  void serve_img(std::string file_name);
  void serve_index();
  
  void api_test();
  void api_echo();

private:
  void serve_static_file(std::string const &path, std::string const &content_type);
};

#endif // CHAT_SERVER_H
