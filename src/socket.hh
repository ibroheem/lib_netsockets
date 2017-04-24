#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#if defined (_MSC_VER)
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <string>
#if defined (HAVE_JANSSON)
#include <jansson.h>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
//utils
/////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long long http_extract_field(const std::string& str_field, const std::string& str_header);
std::string str_extract(const std::string &str_in);
std::string prt_time();
int set_daemon(const char* str_dir);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class socket_t
{
public:
  socket_t();
  socket_t(int socket_fd, sockaddr_in sock_addr);
  ~socket_t();
  void close();
  int write(const void *buf, int size_buf);
  int read(void *buf, int size_buf);
  int read_all_get_close(const char *file_name, bool verbose);
  int hostname_to_ip(const char *host_name, char *ip);

public:
  int m_socket_fd; // socket descriptor 
  sockaddr_in m_sockaddr_in; // client address (used to store return value of server accept())

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //HTTP functions, used by server and clients
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  int parse_http_headers(std::string &http_headers);

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //JSON functions, used by server and clients
  /////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (HAVE_JANSSON)
  int write_json(json_t *json);
  json_t * read_json();
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//tcp_server_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class tcp_server_t : public socket_t
{
public:
  tcp_server_t(const unsigned short server_port);
  socket_t accept_client();
  ~tcp_server_t();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//tcp_client_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class tcp_client_t : public socket_t
{
public:
  tcp_client_t(const char *host_name, const unsigned short server_port);
  ~tcp_client_t();
  int open();

protected:
  std::string m_server_ip;
  unsigned short m_server_port;
};

#endif