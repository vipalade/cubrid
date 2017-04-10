#include <cstddef>
#include "boost/asio.hpp"
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "cubio/cubio.h"
#include <thread>
#include <vector>
#include <memory>

using namespace boost;
using namespace std;

struct cubio_service{
  
  static cubio_service* get(asio::io_service &_rio_svc){
    return reinterpret_cast<cubio_service*>(&_rio_svc);
  }
  
  using thread_vector_t = std::vector<thread>;
  
  asio::io_service          io_service_;
  asio::io_service::work    io_service_work_;
  thread_vector_t           io_thr_vec_;
  asio::ip::tcp::resolver   io_resolver_;
  cubio_service():io_service_work_(io_service_), io_resolver_(io_service_){}
  
};

cubio_service_t* cubio_create_active_service(unsigned _thread_count){
  
  if(_thread_count == 0){
    _thread_count = thread::hardware_concurrency();
  }
  
  cubio_service *psvc = new cubio_service;
  
  psvc->io_thr_vec_.reserve(_thread_count);
  
  auto l = [psvc](){
    psvc->io_service_.run();
  };
  
  for(size_t i = 0; i < _thread_count; ++i){
    psvc->io_thr_vec_.emplace_back(l);
  }
  
  return psvc;
}

void cubio_free_user_data_noop(void*){
}

void cubio_free_user_data_clean(void*_pv){
  if(_pv){
    free(_pv);
  }
}

//-----------------------------------------------------------------------------

void cubio_service_destroy(cubio_service_t* _psvc){
  _psvc->io_service_.stop();
  
  for(auto &rthr: _psvc->io_thr_vec_){
        rthr.join();
  }
  delete _psvc;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct cubio_connection: std::enable_shared_from_this<cubio_connection>{
  asio::ip::tcp::socket   socket_;
  
  void*                   user_data_;
  cubio_free_user_data_t  free_user_data_fnc_;
  
  
  cubio_connection(
    asio::io_service &_rio_service
  ):socket_(_rio_service), user_data_(nullptr), free_user_data_fnc_(nullptr){}
  
  ~cubio_connection(){
    setUserData(nullptr, nullptr);
  }
  
  void setUserData(void *_pd, cubio_free_user_data_t _pf){
    if(user_data_ && free_user_data_fnc_){
      (*free_user_data_fnc_)(user_data_);
      user_data_ = nullptr;
    }
    user_data_ = _pd;
    free_user_data_fnc_ = _pf;
  }
  
};

using connection_ptr_t = std::shared_ptr<cubio_connection>;

struct cubio_listener: std::enable_shared_from_this<cubio_listener>{
    
  asio::ip::tcp::acceptor acceptor_;
  connection_ptr_t        connection_ptr_;
  
  void*                   user_data_;
  cubio_free_user_data_t  free_user_data_fnc_;
  
  
  cubio_listener(
    asio::io_service &_rio_service
  ):acceptor_(_rio_service), user_data_(nullptr), free_user_data_fnc_(nullptr){}
  
  ~cubio_listener(){
    setUserData(nullptr, nullptr);
  }
  
  void setUserData(void *_pd, cubio_free_user_data_t _pf){
    if(user_data_ && free_user_data_fnc_){
      (*free_user_data_fnc_)(user_data_);
      user_data_ = nullptr;
    }
    user_data_ = _pd;
    free_user_data_fnc_ = _pf;
  }
  
};

//-----------------------------------------------------------------------------

void cubio_listener_start(
  cubio_service_t* _psvc,
  const char *_host, const char *_service,
  long _listen_queue_size,
  void *_puser_data, cubio_free_user_data_t _puser_data_free_fnc,
  cubio_listen_function_t _pf, void* _pd
){
  auto lisptr = std::make_shared<cubio_listener>(_psvc->io_service_);
  
  lisptr->setUserData(_puser_data, _puser_data_free_fnc);
  
  if(_listen_queue_size <= 0){
    _listen_queue_size = asio::socket_base::max_connections;
  }
  
  auto l = [lisptr, _listen_queue_size, _pf, _pd](const system::error_code& error, asio::ip::tcp::resolver::iterator iterator){
    if(error){
      (*_pf)(nullptr, _pd, cubio_error_make(error.value(), &error.category()));
      //let lisptr delete listener
    }else{
      boost::system::error_code err;
      boost::asio::ip::tcp::endpoint endpoint(*iterator);
      
      lisptr->acceptor_.open(endpoint.protocol(), err);
      
      if(err){
        (*_pf)(nullptr, _pd, cubio_error_make(err.value(), &err.category()));
        return;
      }
      
      boost::asio::socket_base::reuse_address option(true);
      
      lisptr->acceptor_.set_option(option, err);
      
      if(err){
        (*_pf)(nullptr, _pd, cubio_error_make(err.value(), &err.category()));
        return;
      }
      
      lisptr->acceptor_.bind(endpoint, err);
      if(err){
        (*_pf)(nullptr, _pd, cubio_error_make(err.value(), &err.category()));
        return;
      }
      
      lisptr->acceptor_.listen(_listen_queue_size, err);
      
      if(err){
        (*_pf)(nullptr, _pd, cubio_error_make(err.value(), &err.category()));
        return;
      }
      
      (*_pf)(lisptr.get(), _pd, cubio_error_init());
    }
  };
  
  _psvc->io_resolver_.async_resolve({_host, _service}, l);
}

//-----------------------------------------------------------------------------

void cubio_listener_stop(cubio_listener_t*, cubio_listener_stop_function_t _pf, void *_pd){
  //TODO:
}

//-----------------------------------------------------------------------------

void* cubio_listener_get_user_data(cubio_listener_t *_plis){
  return _plis->user_data_;
}

//-----------------------------------------------------------------------------

void cubio_listener_set_user_data(cubio_listener_t *_plis, void *_pd, cubio_free_user_data_t _pf){
  _plis->setUserData(_pd, _pf);
}

//-----------------------------------------------------------------------------

void cubio_listener_async_accept(
  cubio_listener_t* _plis,
  cubio_accept_function_t _pf, void* _pd
){
  auto lisptr = _plis->shared_from_this();
  
  auto l = [lisptr, _pf, _pd](const system::error_code& error){
    if(error){
      (*_pf)(lisptr.get(), nullptr, _pd, cubio_error_make(error.value(), &error.category()));
    }else{
      connection_ptr_t conptr{std::move(lisptr->connection_ptr_)};
      (*_pf)(lisptr.get(), conptr.get(), _pd, cubio_error_make(error.value(), &error.category()));
    }
  };
  
  _plis->connection_ptr_ = std::make_shared<cubio_connection>(_plis->acceptor_.get_io_service());
  
  _plis->acceptor_.async_accept(_plis->connection_ptr_->socket_, l);
}

//-----------------------------------------------------------------------------

cubio_connection* cubio_service_create_connection_native(
  cubio_service_t *_psvc, const cubio_native_socket_t _native_socket,
  void *_user_data,
  cubio_secure_context_t* _psecure_context
){
  //TODO:
  return NULL;
}

//-----------------------------------------------------------------------------

void cubio_service_create_connection(
  cubio_service_t *_psvc,
  cubio_secure_context_t* /*_psecure_context*/,
  void *_user_data, cubio_free_user_data_t _pfree_user_data_fnc,
  cubio_connection_create_t _pf, void *_pd
){
  auto conptr = std::make_shared<cubio_connection>(_psvc->io_service_);
  conptr->user_data_ = _user_data;
  conptr->free_user_data_fnc_ = _pfree_user_data_fnc;
  auto l = [conptr, _pf, _pd] (){
    (*_pf)(conptr.get(), _pd);
  };
  _psvc->io_service_.post(l);
}

//-----------------------------------------------------------------------------

cubio_service_t* cubio_connection_service(cubio_connection_t *_pcon){
  return cubio_service::get(_pcon->socket_.get_io_service());
}

//-----------------------------------------------------------------------------

void* cubio_connection_get_user_data(cubio_connection_t *_pcon){
  return _pcon->user_data_;
}

//-----------------------------------------------------------------------------

void cubio_connection_set_user_data(cubio_connection_t *_pcon, void *_pd, cubio_free_user_data_t _pf){
  _pcon->setUserData(_pd, _pf);
}

//-----------------------------------------------------------------------------

void cubio_connection_async_connect(
  cubio_connection_t *_pcon,
  const char *_host, const char *_service,
  cubio_connect_function_t _pf, void *_pd
){
  auto conptr = _pcon->shared_from_this();
  
  auto l = [conptr, _pf, _pd](const system::error_code& error, asio::ip::tcp::resolver::iterator iterator){
    if(error){
      (*_pf)(conptr.get(), _pd, cubio_error_make(error.value(), &error.category()));
    }else{
      
      auto l = [conptr, _pf, _pd](const system::error_code& error){
        (*_pf)(conptr.get(), _pd, cubio_error_make(error.value(), &error.category()));
      };
      
      boost::asio::ip::tcp::endpoint endpoint(*iterator);
      conptr->socket_.async_connect(endpoint, l);
    }
  };
  
  cubio_service *psvc = cubio_connection_service(_pcon);
  psvc->io_resolver_.async_resolve({_host, _service}, l);
}

//-----------------------------------------------------------------------------

void cubio_connection_async_recv_some(
  cubio_connection_t *_pcon,
  char *_pbuf, unsigned _pbuf_len,
  cubio_recv_function_t _pf, void *_pd
){
  auto conptr = _pcon->shared_from_this();
  
  auto l = [conptr, _pf, _pd](const system::error_code& error, std::size_t _len){
    (*_pf)(conptr.get(), _pd, _len, cubio_error_make(error.value(), &error.category()));
  };
  
  _pcon->socket_.async_read_some(asio::buffer(_pbuf, _pbuf_len), l);
}

//-----------------------------------------------------------------------------

void cubio_connection_async_send_all(
  cubio_connection_t *_pcon,
  const char *_pbuf, unsigned _pbuf_len,
  cubio_send_function_t _pf, void *_pd
){
  auto conptr = _pcon->shared_from_this();
  
  auto l = [conptr, _pf, _pd](const system::error_code& error, std::size_t _len){
    (*_pf)(conptr.get(), _pd, _len, cubio_error_make(error.value(), &error.category()));
  };
  
  asio::async_write(_pcon->socket_, asio::buffer(_pbuf, _pbuf_len), l);
}

//-----------------------------------------------------------------------------

void cubio_connection_post(
  cubio_connection_t *_pcon,
  cubio_post_function_t _pf, void *_pd
){
  auto conptr = _pcon->shared_from_this();
  
  auto l = [conptr, _pf, _pd](){
    (*_pf)(conptr.get(), _pd);
  };
  _pcon->socket_.get_io_service().post(l);
}

//-----------------------------------------------------------------------------

cubio_error_code cubio_connection_cancel(cubio_connection_t *_pcon){
  boost::system::error_code error;
  _pcon->socket_.cancel(error);
  return cubio_error_make(error.value(), &error.category());
}

//-----------------------------------------------------------------------------

cubio_error_code cubio_connection_close(cubio_connection_t *_pcon){
  boost::system::error_code error;
  _pcon->socket_.close(error);
  return cubio_error_make(error.value(), &error.category());
}

//-----------------------------------------------------------------------------

void cubio_connection_timer_async_wait(
  cubio_connection_t *_pcon,
  long _seconds,
  long _milli_seconds,
  cubio_timer_function_t _pf, void *_pd
){
  
}

//-----------------------------------------------------------------------------
/* Synchronous*/
//-----------------------------------------------------------------------------


cubio_service_t* cubio_create_passive_service(){
  return NULL;
}

//-----------------------------------------------------------------------------

cubio_acceptor_t* cubio_create_plain_acceptor(cubio_service_t*, const char *_host, const char *_service, long _listen_queue_size){
  return NULL;
}

//-----------------------------------------------------------------------------

cubio_stream_t* cubio_acceptor_accept(cubio_acceptor_t*){
  return NULL;
}

//-----------------------------------------------------------------------------

cubio_stream_t* cubio_create_plain_stream(cubio_service_t*){
  return NULL;
}

//-----------------------------------------------------------------------------
//cubio_stream* cubio_create_secure_stream(cubio_service*);

cubio_stream_t* cubio_create_plain_stream_native(cubio_service_t*, cubio_native_socket_t){
  return NULL;
}

//-----------------------------------------------------------------------------

bool cubio_stream_connect(cubio_stream_t*, const char *_host, const char *_service, long _time, cubio_error_code*){
  return false;
}

//-----------------------------------------------------------------------------

int cubio_stream_read_some(cubio_stream_t*, char *_buf, unsigned _len, long _time, cubio_error_code*){
  return 0;
}

//-----------------------------------------------------------------------------

int cubio_stream_read_at_least(cubio_stream_t*, char *_buf, unsigned _len, unsigned _at_least, long _time, cubio_error_code*){
  return -1;
}

//-----------------------------------------------------------------------------

bool cubio_stream_write_all(cubio_stream_t*, const char*_buf, unsigned _len, long _time, cubio_error_code*){
  return false;
}
//-----------------------------------------------------------------------------
