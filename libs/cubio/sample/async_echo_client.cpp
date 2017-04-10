#include <vector>
#include <iostream>
#include <cassert>
#include "boost/program_options.hpp"

#include <condition_variable>
#include <mutex>

#include "cubio/cubio.h"

using namespace std;

namespace{

const size_t sizes[]{
    100,
    200,
    400,
    800,
    1600,
    2400,
    3200,
    4800,
    6400,
    100,
    200,
    400,
    800,
    1600,
    2400,
    3200,
    4800,
    100,
    200,
    400,
    800,
    1600,
    2400,
    3200,
    100,
    200,
    400,
    800,
    1600,
    2400};
size_t sizes_size = sizeof(sizes) / sizeof(size_t);

vector<string>      send_data_vec;
mutex               mtx;
condition_variable  cnd;
size_t              connection_count = 0;
uint64_t            read_size = 0;

struct Parameters{
  uint32_t    connection_count;
  uint32_t    repeat_count;
  
  string      host;
  string      service;
} parameters;

bool parse_parameters(Parameters &_rp, int argc, char *argv[]);
void prepare_data();

struct ConnectionData{
  
  static void erase(void *_pv){
    delete static_cast<ConnectionData*>(_pv);
  }
  
  enum{
    BufCapacity = 1024
  };
  
  ConnectionData():crt_send_index_(0), expect_recv_size_(0), read_size_(0){}
  
  char      buf_[BufCapacity];
  size_t    index_;
  size_t    crt_send_index_;
  size_t    expect_recv_size_;
  uint64_t  read_size_;
  
};

void on_connection_create(cubio_connection_t *_pcon, void *);
void on_connection_connect(cubio_connection_t *, void *, const cubio_error_code);
void on_connection_recv(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error);
void on_connection_send(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error);

}//namespace

int main(int argc, char *argv[]){
  
  if(parse_parameters(parameters, argc, argv)) return 0;
  
  prepare_data();
  
  cubio_service_t * psvc = cubio_create_active_service(0);
  
  connection_count = parameters.connection_count;
  
  for(size_t i = 0; i < parameters.connection_count; ++i){
    ConnectionData     *pcon_data = new ConnectionData;
    pcon_data->index_ = i;
    cubio_service_create_connection(psvc, nullptr, pcon_data, ConnectionData::erase, on_connection_create, nullptr);
  }
  
  {
    unique_lock<mutex> lock(mtx);
    while(connection_count){
      cnd.wait(lock);
    }
  }
  cout<<"Read "<<(read_size/1024)<<"KB on "<<parameters.connection_count<<" connections."<<endl;
  
  return 0;
}


namespace{

//-----------------------------------------------------------------------------

bool parse_parameters(Parameters &_par, int argc, char *argv[]){
  using namespace boost::program_options;
  try {
    options_description desc("async_echo client application");
    desc.add_options()
    ("host,H", value<string>(&_par.host)->default_value("localhost"), "Server host name")
    ("service,s", value<string>(&_par.service)->default_value("4444"), "Server service port")
    ("connection-count,c", value<uint32_t>(&_par.connection_count)->default_value(1), "Connection count")
    ("repeat-count,r", value<uint32_t>(&_par.repeat_count)->default_value(100), "Repeat count")
    ("help,h", "List program options");
    
    variables_map vm;
    
    store(parse_command_line(argc, argv, desc), vm);
    
    notify(vm);
    
    if (vm.count("help")) {
        cout << desc << "\n";
        return true;
    }

    return false;
  } catch (exception& e) {
    cout << e.what() << "\n";
    return true;
  }
}

//-----------------------------------------------------------------------------

void prepare_data()
{
  string pattern;
  pattern.reserve(256);

  for (int i = 0; i < 256; ++i) {
    if (isgraph(i)) {
      pattern += static_cast<char>(i);
    }
  }

  send_data_vec.resize(sizes_size);
  for (size_t i = 0; i < sizes_size; ++i) {
    auto& s = send_data_vec[i];
    s.reserve(sizes[i]);
    for (size_t j = 0; j < sizes[i]; ++j) {
      s += pattern[(i + j) % pattern.size()];
    }
  }
}

//-----------------------------------------------------------------------------

void on_connection_connect(cubio_connection_t *_pcon, void *, const cubio_error_code _error){
  if(cubio_success(&_error)){
    ConnectionData *pcon_data = (ConnectionData*)cubio_connection_get_user_data(_pcon);
    
    const size_t send_idx = (pcon_data->crt_send_index_ + pcon_data->crt_send_index_) % sizes_size;
    
    pcon_data->expect_recv_size_ = send_data_vec[send_idx].size();
    
    //issue a read
    cubio_connection_async_recv_some(_pcon, pcon_data->buf_, ConnectionData::BufCapacity, on_connection_recv, nullptr);
    cubio_connection_async_send_all(
      _pcon, send_data_vec[send_idx].data(),
      send_data_vec[send_idx].size(),
      on_connection_send,
      nullptr
    );
  }else{
    //do not give up - retry
    cubio_connection_async_connect(_pcon, parameters.host.c_str(), parameters.service.c_str(), on_connection_connect, nullptr);
  }
}

void on_connection_recv(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error){
  if(cubio_success(&_error)){
    ConnectionData *pcon_data = (ConnectionData*)cubio_connection_get_user_data(_pcon);
    
    assert(_len <= pcon_data->expect_recv_size_);
    
    pcon_data->expect_recv_size_ -= _len;
    pcon_data->read_size_ += _len;
    
    if(pcon_data->expect_recv_size_ == 0){
      
      ++pcon_data->crt_send_index_;
      
      if(pcon_data->crt_send_index_ < parameters.repeat_count){
      
        const size_t send_idx = (pcon_data->crt_send_index_ + pcon_data->crt_send_index_) % sizes_size;
        
        pcon_data->expect_recv_size_ = send_data_vec[send_idx].size();
        
        cubio_connection_async_recv_some(_pcon, pcon_data->buf_, ConnectionData::BufCapacity, on_connection_recv, nullptr);
        
        cubio_connection_async_send_all(
          _pcon, send_data_vec[send_idx].data(),
          send_data_vec[send_idx].size(),
          on_connection_send,
          nullptr
        );
      }else{
        unique_lock<mutex> lock(mtx);
        --connection_count;
        read_size += pcon_data->read_size_;
        if(connection_count == 0){
          cnd.notify_one();
        }
      }
    }else{
      cubio_connection_async_recv_some(_pcon, pcon_data->buf_, ConnectionData::BufCapacity, on_connection_recv, nullptr);
    }
  }
}

void on_connection_send(cubio_connection_t *_pcon, void *_pd, unsigned _len, const cubio_error_code _error){
  if(cubio_success(&_error)){
    
  }
}

void on_connection_create(cubio_connection_t *_pcon, void *){
  cubio_connection_async_connect(_pcon, parameters.host.c_str(), parameters.service.c_str(), on_connection_connect, nullptr);
}

//-----------------------------------------------------------------------------

}//namespace
