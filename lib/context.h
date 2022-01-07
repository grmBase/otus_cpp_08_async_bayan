//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
//#include <vector>
#include <string>
#include <mutex> // для todo
//---------------------------------------------------------------------------
#include "bulk_machine.h"  // используем как данные этого класа
//---------------------------------------------------------------------------


namespace impl 
{


// Контекст (данные) одного "соединения"
class t_context
{

  public: 

    // конструктор с одним параметром - там удобно так использовать
    t_context(size_t aun_bulk);

    // функция потокового добавления чего-то нового:
    void receive(const char* ap_data, std::size_t a_un_size);


  private:

    // буфер, где будем хранить промежуточные данные, пока не получили перевод строки
    // (в этом задании данные уже стали передавать в том числе "побайтно", а не построчно как было в 6-ом)
    //std::vector<uint8_t> m_buf_not_ready_yet_str;
    // решил переделать на строку. Будем хранить её не оканчивающуюся на 0
    std::string m_buf_not_ready_yet;


    // машина обработки "пачек" из строковых инструкций
    t_bulk_machine m_bulk_machine;

    // mutex, на случай если нужно делать расширенное задание, где receive можно вызывать из нескольких потоков
    // mutable std::mutex m_mutex;

};


}
//---------------------------------------------------------------------------