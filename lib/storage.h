//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <cstddef>
#include <string_view>
#include <mutex>
#include <unordered_set>  // храним контексты
//---------------------------------------------------------------------------
#include "async.h"        // используем определение handle
#include "console_conv.h" // создаём здесь объект с буфером и нитью для вывода в std::cout
#include "file_conv.h" // создаём здесь объект с буфером и нитью для вывода в std::cout
//---------------------------------------------------------------------------


namespace impl 
{
 
  class t_context; // forward declaration, чтобы не подключать хидер


// Класс глобального хранилища всех контекстов
class handle_storage
{

  public:

    // храню указатели на распределённые данные - нужно удалить данные в деструкторе
    ~handle_storage();

    // когда нас просят создать новый контекст:
    // для простоты, будем считать, что 0-ой возврат это ошибка (нет памяти например)
    async::handle_t register_new_handle(size_t a_un_stat_bulk);

    // запрашиваем контекст по значению хендла (по сути только для проведения операции под крит секцией)
    impl::t_context* get_context(async::handle_t a_handle);

    // удаляем данный контекст. Если не 0, то была какая-то ошибка (не было такого контекста уже например)
    int destroy_context(async::handle_t a_handle);



    void log_info(const std::string_view& astr_info);
    void log_err(const std::string_view& astr_info);

    void push_to_console_conv(const std::string& astr_info);
    void push_to_file_conv(const std::string& astr_info);

  private: 

    // контейнер контекстов
    //std::unordered_set<std::unique_ptr<impl::context>> m_set;
    std::unordered_set<impl::t_context*> m_set;

    // всякие операции регистрации, удаления будем делать под крит секцией
    mutable std::mutex m_mutex;



    // отдельный мьютекс для синхронизации моего отладочного вывода в std::cout, std::cerr
    mutable std::mutex m_mutex_cout;



    // объект нитка + буфер для вывода в консоль накопленных пачек
    impl::t_console_conv m_console_conv;

    // объект нитка + буфер для вывода в консоль накопленных пачек
    impl::t_file_conv m_file_conv;
};


}
//---------------------------------------------------------------------------