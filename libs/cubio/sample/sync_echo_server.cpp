#include <thread>
#include <vector>
#include <stack>
#include <mutex>
#include <condition_variable>

#include "cubio/cubio.h"

using namespace std;

using thread_vector_t = std::vector<thread>;
using stream_vector_t = std::vector<cubio_stream_t*>;
using size_stack_t = std::stack<size_t>;
enum {BufSize = 2 * 1024};

struct context{
  size_stack_t        freestk;
  thread_vector_t     thrvec;
  stream_vector_t     streamvec;
  mutex               mtx;
};

void stream_run(context &_rctx, cubio_stream_t *_ps, const size_t _off){
  char              buf[BufSize];
  cubio_error_code  error = cubio_error_init();
  int               len;
  
  const long timeout = 60 * 1000;//1 min
  
  cubio_stream_set_no_delay(_ps, true);
  
  while(cubio_success(&error) && (len = cubio_stream_read_some(_ps, buf, BufSize, timeout, &error)) >= 0){
    cubio_stream_write_all(_ps, buf, len, timeout, &error);
  }
  
  //connection closed
  {
    unique_lock<mutex> lock(_rctx.mtx);
    _rctx.streamvec[_off] = nullptr;
    _rctx.freestk.push(_off);
  }
  cubio_stream_close(_ps);
  cubio_stream_destroy(_ps);
}

void acceptor_run(context &_rctx, cubio_acceptor_t *_pacc){
  cubio_stream_t    *pstream;
  cubio_error_code  error;
  
  while((pstream = cubio_acceptor_accept(_pacc, -1, &error))){
    size_t off;
    unique_lock<mutex> lock(_rctx.mtx);
    if(_rctx.freestk.size()){
      off = _rctx.freestk.top();_rctx.freestk.pop();
      _rctx.thrvec[off].join();
      _rctx.thrvec[off] = std::move(thread(stream_run, ref(_rctx), pstream, off));
    }else{
      off = _rctx.streamvec.size();
      _rctx.streamvec.push_back(pstream);
      _rctx.thrvec.emplace_back(stream_run, ref(_rctx), pstream, off);
    }
  }
}

int main(int argc, char *argv[]){
  
  if(argc != 3){
    printf("Usage:\n %s HOST SERVICE\n", argv[0]);
    return 0;
  }
  cubio_error_code  error;
  cubio_service_t   *psvc = cubio_create_passive_service();
  cubio_acceptor_t  *pacc = cubio_create_plain_acceptor(psvc, argv[1], argv[2], 100, &error);
  
  if(!pacc){
    return 0;
  }
  
  context   ctx;      
  
  thread    accthr{acceptor_run, ref(ctx), pacc};
  
  printf("Press any key + ENTER to continue...\n");
  char c = ' ';
  fflush(stdin);
  scanf("%c", &c);
  
  cubio_acceptor_close(pacc);
  accthr.join();//wait listener thread
  cubio_acceptor_destroy(pacc);
  
  {
    unique_lock<mutex> lock(ctx.mtx);
    for(const auto& pstream: ctx.streamvec){
      cubio_stream_shutdown(pstream);
    }
  }
  
  for(auto& thr: ctx.thrvec){
    if(thr.joinable()){
      thr.join();
    }
  }
  
  cubio_service_destroy(psvc);
  
  return 0;
}
