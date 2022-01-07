//---------------------------------------------------------------------------
#include <iostream>

#include <fstream> // вывод в файл
#include <filesystem>  // определ€ем наличие уже такого файла

#include <string>
//---------------------------------------------------------------------------
#include "file_conv.h"
//---------------------------------------------------------------------------
#include "async.h"
//---------------------------------------------------------------------------



impl::t_file_conv::t_file_conv()
  : m_threads{c_un_num_of_file_thread}
{

  // запускаем нити:
  for(auto& curr : m_threads) {
    curr = std::thread([this] {this->work(); });
  };
}
//---------------------------------------------------------------------------


// деструктор, чтобы остановить нить
impl::t_file_conv::~t_file_conv()
{
  impl::logout("before set_exit_flag()");
  set_exit_flag();

  impl::logout("before join() threads");
  for (auto& curr : m_threads) {
    curr.join();
  };


  impl::logout("after join() threads");
}
//---------------------------------------------------------------------------


int impl::t_file_conv::push(const std::string& astr_info)
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
void impl::t_file_conv::set_exit_flag()
{
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_exit_flag = true;
  }

  m_cv.notify_all();
}
//---------------------------------------------------------------------------


// будем вызывать из нити
void impl::t_file_conv::work()
{

  while(true) 
  {

    std::unique_lock<std::mutex> lock(m_mutex);
    while(!m_buf.size() && !m_exit_flag) {
  
       m_cv.wait(lock);
    }

    // сначала посмотрим что у нас с задани€ми
    while(m_buf.size()) 
    {
      std::string str = m_buf.front();
      m_buf.pop_front();
      impl::logout(">> from file thread. Found new task to save in file: " + str);
 
      // сохран€ем в файл:
      {

        //std::chrono::time_point begin = std::chrono::steady_clock::now().time_since_epoch();
        auto duration = std::chrono::steady_clock::now().time_since_epoch();
        //std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::string str_filename_base = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

        std::string str_filename = str_filename_base + ".txt";
        size_t un_postfix = 0;
        while (true) {
          std::filesystem::path p = std::filesystem::u8path(str_filename);
          std::error_code status;

          // if file not exist
          if(!std::filesystem::exists(p, status)) {
            break;
          }

          str_filename = str_filename_base + "_" + std::to_string(un_postfix) + ".txt";
          un_postfix++;
        }

        impl::logout(">> filename to save: " + str_filename);

        std::ofstream file;

        file.open(str_filename);
        file << str;
        file.close();
      }
    }

    // потом посмотрим - может пора выходить
    if(m_exit_flag) {
      impl::logout("Exit flag detected. Exiting from thread...");
      return;
    }

  }

}
//---------------------------------------------------------------------------