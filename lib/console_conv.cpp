//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include "console_conv.h"
//---------------------------------------------------------------------------
#include "async.h"
//---------------------------------------------------------------------------



impl::t_console_conv::t_console_conv()
 :m_thread([this] {this->work();})
{
}
//---------------------------------------------------------------------------


// деструктор, чтобы остановить нить
impl::t_console_conv::~t_console_conv()
{
  impl::logout("before set_exit_flag()");
  set_exit_flag();

  impl::logout("before join() thread");
  m_thread.join();

  impl::logout("after join() thread");
}
//---------------------------------------------------------------------------


int impl::t_console_conv::push(const std::string& astr_info)
{
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_buf.push_back(astr_info);
  }

  m_cv.notify_one();

  return 0;
}
//---------------------------------------------------------------------------



// ствим флаг, что пора выходить
void impl::t_console_conv::set_exit_flag()
{
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_exit_flag = true;
  }

  m_cv.notify_one();
}
//---------------------------------------------------------------------------


// будем вызывать из нити
void impl::t_console_conv::work()
{

  while(true) 
  {

    std::unique_lock<std::mutex> lock(m_mutex);
    while(!m_buf.size() && !m_exit_flag) {
  
       m_cv.wait(lock);
    }

    // сначала посмотрим что у нас с заданиями
    while(m_buf.size()) {

      impl::logout(">> from console thread. Found new task to print");
      std::string str = m_buf.front();
      m_buf.pop_front();
  
      std::cout << str << std::endl;
    }

    // потом посмотрим - может пора выходить
    if (m_exit_flag) {
      impl::logout("Exit flag detected. Exiting from thread...");
      return;
    }

  }

}
//---------------------------------------------------------------------------