//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <cstddef>
#include <vector>
#include <deque>
#include <string>
//---------------------------------------------------------------------------
#include <mutex>
#include <condition_variable>
#include <thread>
//---------------------------------------------------------------------------


namespace impl 
{

 const size_t c_un_num_of_file_thread = 2;
 
class t_file_conv
{

  public:

    // контсруктор, чтобы указать функцию для нити
    t_file_conv();

    // деструктор, чтобы остановить нить
    ~t_file_conv();

    
    int push(const std::string& astr_info); // кладём в буфер на обработку

    void set_exit_flag(); // ствим флаг, что пора выходить
    

  private: 

    void work(); // будем вызывать из нити
    
    std::deque<std::string> m_buf; // буфер отложенных задач
    bool m_exit_flag = false;

    mutable std::mutex m_mutex;     // защита от мнгопоточного доступа
    std::condition_variable m_cv;   // сигнал наличия работы

    std::vector<std::thread> m_threads; // список рабочих нитией
    //std::thread m_thread; // рабочая нить
};


}
//---------------------------------------------------------------------------
