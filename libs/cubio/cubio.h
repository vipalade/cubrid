#ifndef CUBRID_CUBIO_H
#define CUBRID_CUBIO_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
/* Asynchronous API */
//-----------------------------------------------------------------------------

typedef struct cubio_service cubio_service_t;
typedef struct cubio_secure_context cubio_secure_context_t;
typedef struct cubio_connection cubio_connection_t;
typedef struct cubio_listener cubio_listener_t;

typedef int cubio_native_socket_t;

typedef struct {
  long        code;
  const void  *category;
} cubio_error_code;

static inline cubio_error_code cubio_error_init(){
  cubio_error_code err;
  err.code = 0;
  err.category = 0;
  return err;
}

static inline cubio_error_code cubio_error_make(long _code, const void *_categ){
  cubio_error_code err;
  err.code = _code;
  err.category = _categ;
  return err;
}

static inline bool cubio_error(const cubio_error_code* _perr){
  return _perr->code != 0;
}

static inline bool cubio_success(const cubio_error_code* _perr){
  return _perr->code == 0;
}

cubio_service_t* cubio_create_active_service(unsigned _thread_count);

void cubio_service_destroy(cubio_service_t*);

typedef void (*cubio_listen_function_t)(cubio_listener_t*, void *, const cubio_error_code);
typedef void (*cubio_free_user_data_t)(void *);

void cubio_listener_start(
  cubio_service_t*,
  const char *_host, const char *_service,
  long _listen_queue_size,
  void *_puser_data, cubio_free_user_data_t _puser_data_free_fnc,
  cubio_listen_function_t _pf, void* _pd
);

typedef void (*cubio_accept_function_t)(cubio_listener_t*, cubio_connection_t*, void *, const cubio_error_code);

void cubio_free_user_data_noop(void*);
void cubio_free_user_data_clean(void*);

void* cubio_listener_get_user_data(cubio_listener_t *_plis);

void cubio_listener_set_user_data(cubio_listener_t *_plis, void *, cubio_free_user_data_t _pf);


void cubio_listener_async_accept(
  cubio_listener_t*,
  cubio_accept_function_t _pf, void* _pd
);

typedef void (*cubio_listener_stop_function_t)(cubio_listener_t *, void *);

void cubio_listener_stop(cubio_listener_t*, cubio_listener_stop_function_t _pf, void *_pd);

cubio_connection_t* cubio_service_create_connection_native(
  cubio_service_t *_psvc, const cubio_native_socket_t _native_socket,
  void *_user_data,
  cubio_secure_context_t* _psecure_context
);

typedef void (*cubio_connection_create_t)(cubio_connection_t *, void *);

void cubio_service_create_connection(
  cubio_service_t *_psvc,
  cubio_secure_context_t* _psecure_context,
  void *_user_data, cubio_free_user_data_t _pfree_user_data_fnc,
  cubio_connection_create_t _pf, void *_pd
);

cubio_service_t* cubio_connection_service(cubio_connection_t *_pcon);

void* cubio_connection_get_user_data(cubio_connection_t *_pcon);

void cubio_connection_set_user_data(cubio_connection_t *_pcon, void *, cubio_free_user_data_t _pf);

cubio_error_code cubio_connection_set_no_delay(cubio_connection_t *_pcon, bool _option);

typedef void (*cubio_connect_function_t)(cubio_connection_t *, void *, const cubio_error_code);

void cubio_connection_async_connect(
  cubio_connection_t *_pcon,
  const char *_host, const char *_service,
  cubio_connect_function_t _pf, void *_pd
);

typedef void (*cubio_recv_function_t)(cubio_connection_t *, void *, unsigned, const cubio_error_code);

void cubio_connection_async_recv_some(
  cubio_connection_t *_pcon,
  char *_pbuf, unsigned _pbuf_len,
  cubio_recv_function_t _pf, void *_pd
);

typedef void (*cubio_send_function_t)(cubio_connection_t *, void *, unsigned, const cubio_error_code);

void cubio_connection_async_send_all(
  cubio_connection_t *_pcon,
  const char *_pbuf, unsigned _pbuf_len,
  cubio_send_function_t _pf, void *_pd
);

typedef void (*cubio_post_function_t)(cubio_connection_t *, void *);

void cubio_connection_post(
  cubio_connection_t *_pcon,
  cubio_post_function_t _pf, void *_pd
);

cubio_error_code cubio_connection_close(cubio_connection_t *_pcon);
cubio_error_code cubio_connection_cancel(cubio_connection_t *_pcon);

typedef void (*cubio_timer_function_t)(cubio_connection_t *, void *, const cubio_error_code);

bool cubio_connection_timer_async_wait(
  cubio_connection_t *_pcon,
  unsigned long _seconds,
  unsigned long _milliseconds,
  cubio_timer_function_t _pf, void *_pd
);

bool cubio_connection_timer_cancel(cubio_connection_t *_pcon);

//-----------------------------------------------------------------------------
/* Synchronous API */
//-----------------------------------------------------------------------------

typedef struct cubio_stream cubio_stream_t;
typedef struct cubio_acceptor cubio_acceptor_t;

cubio_service_t* cubio_create_passive_service();

cubio_acceptor_t* cubio_create_plain_acceptor(cubio_service_t*, const char *_host, const char *_service, long _listen_queue_size, cubio_error_code*);

void cubio_acceptor_close(cubio_acceptor_t *);
void cubio_acceptor_destroy(cubio_acceptor_t *);

cubio_stream_t* cubio_acceptor_accept(cubio_acceptor_t*, long _time, cubio_error_code*);

cubio_stream_t* cubio_create_plain_stream(cubio_service_t*);

//cubio_stream_t* cubio_create_secure_stream(cubio_service_t*);

cubio_stream_t* cubio_create_plain_stream_native(cubio_service_t*, cubio_native_socket_t);

bool cubio_stream_connect(cubio_stream_t*, const char *_host, const char *_service, long _time, cubio_error_code*);

int cubio_stream_read_some(cubio_stream_t*, char *_buf, unsigned _len, long _time, cubio_error_code*);

int cubio_stream_read_at_least(cubio_stream_t*, char *_buf, unsigned _len, unsigned _at_least, long _time, cubio_error_code*);

bool cubio_stream_write_all(cubio_stream_t*, const char*_buf, unsigned _len, long _time, cubio_error_code*);

cubio_error_code cubio_stream_set_no_delay(cubio_stream_t *_ps, bool _option);

void cubio_stream_shutdown(cubio_stream_t* _ps);
void cubio_stream_close(cubio_stream_t*);
void cubio_stream_destroy(cubio_stream_t*);


#ifdef __cplusplus
}
#endif

#endif

