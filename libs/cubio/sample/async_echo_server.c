#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cubio/cubio.h"

const unsigned buffer_size = 1024 * 2;

void free_user_data(void *_pv);
void on_connection_recv(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error);
void on_connection_send(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error);
void on_listener_accept(cubio_listener_t* _plis, cubio_connection_t* _pcon, void *, const cubio_error_code _error);
void on_listener_start(cubio_listener_t* _plis, void *, const cubio_error_code _error);

#if 0
#define dbg(d, ...) printf("[%s][%d][%s]"d"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#else
#define dbg(d, ...)
#endif

void free_user_data(void *_pv){
  dbg("");
  if(_pv) free(_pv);
}

void on_connection_send(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error){
  if(cubio_success(&_error)){
    dbg("");
    cubio_connection_async_recv_some(_pcon, cubio_connection_get_user_data(_pcon), buffer_size, on_connection_recv, NULL);
  }else{
    cubio_connection_close(_pcon);
  }
}

void on_connection_recv(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error){
  if(cubio_success(&_error)){
    
    cubio_connection_async_send_all(_pcon, cubio_connection_get_user_data(_pcon), _len, on_connection_send, NULL);
  }else{
    cubio_connection_close(_pcon);
  }
}

void on_listener_accept(cubio_listener_t* _plis, cubio_connection_t* _pcon, void *_pd, const cubio_error_code _error){
  if(cubio_success(&_error)){
    assert(_pcon != NULL);
    dbg("");
    cubio_connection_set_no_delay(_pcon, true);
    cubio_connection_set_user_data(_pcon, malloc(buffer_size), &free_user_data);
    cubio_connection_async_recv_some(_pcon, cubio_connection_get_user_data(_pcon), buffer_size, on_connection_recv, NULL);
    cubio_listener_async_accept(_plis, &on_listener_accept, NULL);
  }else{
    exit(0);
  }
}

void on_listener_start(cubio_listener_t* _plis, void *_pd, const cubio_error_code _error){
  if(cubio_success(&_error)){
    dbg("");
    cubio_listener_async_accept(_plis, &on_listener_accept, NULL);
  }else{
    exit(0);
  }
}

int main(int argc, char *argv[]){
  
  if(argc != 3){
    printf("Usage:\n %s HOST SERVICE\n", argv[0]);
    return 0;
  }
  
  cubio_service_t * psvc = cubio_create_active_service(0);
  
  cubio_listener_start(psvc, argv[1], argv[2], 100, NULL, NULL, on_listener_start, NULL);
  
  printf("Press any key + ENTER to continue...\n");
  char c = ' ';
  fflush(stdin);
  scanf("%c", &c);
  
  cubio_service_destroy(psvc);
  
  return 0;
}

