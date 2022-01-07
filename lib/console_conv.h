//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <cstddef>
#include <thread> // создаём нити
#include <deque>  // буфер для заданий
#include <string>
//---------------------------------------------------------------------------
#include <mutex>
#include <condition_variable>
//---------------------------------------------------------------------------


namespace impl 
{
 
class t_console_conv
{

  public:

    // контсруктор, чтобы указать функцию для нити
    t_console_conv();

    // деструктор, чтобы остановить нить
    ~t_console_conv();

    
    int push(const std::string& astr_info); // кладём в буфер на обработку

    void set_exit_flag(); // ствим флаг, что пора выходить
    

  private: 

    void work(); // будем вызывать из нити
    
    std::deque<std::string> m_buf; // буфер отложенных задач
    bool m_exit_flag = false;

    mutable std::mutex m_mutex;     // защита от мнгопоточного доступа
    std::condition_variable m_cv;   // сигнал наличия работы

    std::thread m_thread; // рабочая нить
};


}
//---------------------------------------------------------------------------
