#include "cas_cci.h"
#include <iostream>
#include <sstream>
using namespace std;

int dump_sailors(int _con_handle){
  int req = 0, res = 0;
  int ind = 0, i = 0, col_count = 0;
  void *buffer;
  T_CCI_ERROR error;
  T_CCI_COL_INFO *res_col_info;
  T_CCI_SQLX_CMD cmd_type;
  
  req = cci_prepare(_con_handle, "select * from sailors;", 0, &error);
  
  res_col_info = cci_get_result_info (req, &cmd_type, &col_count);
  
  res = cci_execute (req, 0, 1024, &error);
  
  res = cci_cursor (req, 1, CCI_CURSOR_FIRST, &error);
  
  res = cci_fetch (req, &error);
  
  do{
    
    //cout<<"col_count = "<<col_count<<endl;
    
    for (i = 1; i <= col_count; i++)
      {
          res =  cci_get_data (req, i, CCI_A_TYPE_STR, &buffer, &ind);
//           if(ind < 0){
//             cout<<"(NULL)"<<endl;
//           }else{
//             cout<<(const char *)buffer<<endl;
//           }
      }
    res = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &error); 
    if(res == CCI_ER_NO_MORE_DATA) break;
    res = cci_fetch (req, &error);
  }while(res >= 0);
  return 0;
}

int main(int argc, char *argv[]){
  //cci_init();
  int major, minor, patch;
  T_CCI_ERROR err_buf;
  
  cci_get_version(&major, &minor, &patch);
  cout<<"version: "<<major<<' '<<minor<<' '<<patch<<endl;
  
  int con_handle = cci_connect_ex("127.0.0.1", 30000, "test", "dba", "", &err_buf);
  
  if(con_handle < 0){
    cout<<err_buf.err_code<<' '<<err_buf.err_msg<<endl;
    return 0;
  }
  
  int exec_retval;
  int rv;
  
  {
    for(int j = 0; j < 100; ++j){
      ostringstream oss;
      oss<<"insert into sailors values ("<<j<<", 'super_mike_"<<j<<"', 'super_mike_"<<j<<"@master_bike.com');";
      //cout<<oss.str()<<endl;
      rv = cci_prepare_and_execute(con_handle, const_cast<char*>(oss.str().c_str()), 1024, &exec_retval, &err_buf);
    }
  }
  
  for(int i = 0; i < 1000; ++i){
    rv = dump_sailors(con_handle);
  }
  
  rv = cci_prepare_and_execute(con_handle, "truncate table sailors;", 1024, &exec_retval, &err_buf);
  
  cci_end();
  return 0;
}
