//---------------------------------------------------------------------------
#include <mutex> //lock_guard
#include <iostream>
#include <sstream>
//---------------------------------------------------------------------------
#include "storage.h"
//---------------------------------------------------------------------------
#include "context.h"
//---------------------------------------------------------------------------



// храню указатели на распределённые данные - нужно удалить данные в деструкторе
impl::handle_storage::~handle_storage()
{

  // для пущей надёжности под критической секцией, хотя по идее это не надо
  std::lock_guard<std::mutex> lock(m_mutex);

  for (auto curr : m_set)
  {
    if (curr) {
      delete curr;
    }
  }

  m_set.clear();
};
//---------------------------------------------------------------------------



// когда нас просят создать новый контекст:
// для простоты, будем считать, что 0-ой возврат это ошибка (нет памяти например)
async::handle_t impl::handle_storage::register_new_handle(size_t a_un_stat_bulk)
{
  impl::t_context* p_context = new impl::t_context(a_un_stat_bulk);

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_set.emplace(p_context);
  }

  return async::handle_t(p_context);
};
//---------------------------------------------------------------------------


// запрашиваем контекст по значению хендла (по сути только для проведения операции под крит секцией)
impl::t_context* impl::handle_storage::get_context(async::handle_t a_handle)
{

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_set.find(static_cast<impl::t_context*>(a_handle));
    if (iter == m_set.end()) {
      return nullptr;
    }

    return *iter;
  }

};
//---------------------------------------------------------------------------

// удаляем данный контекст. Если не 0, то была какая-то ошибка (не было такого контекста уже например)
int impl::handle_storage::destroy_context(async::handle_t a_handle)
{

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_set.find(static_cast<impl::t_context*>(a_handle));
    if (iter == m_set.end()) {
      return -33;
    }

    delete *iter;
    m_set.erase(iter);

    return 0;
  }

};
//---------------------------------------------------------------------------





// пока включим отладку:
//#define DBG_LOGGING

void impl::handle_storage::log_info(__attribute__((unused)) const std::string_view& astr_info)
{

#ifdef DBG_LOGGING
  std::lock_guard<std::mutex> lock(m_mutex_cout);

  std::cout << astr_info << " [tid:" << std::this_thread::get_id() << "]" << std::endl;
#endif
}
//---------------------------------------------------------------------------

// 
void impl::handle_storage::log_err(__attribute__((unused)) const std::string_view& astr_info)
{
#ifdef DBG_LOGGING
  std::lock_guard<std::mutex> lock(m_mutex_cout);

  std::cerr << astr_info << " [tid:" << std::this_thread::get_id() << "]" << std::endl;
#endif
}
//---------------------------------------------------------------------------





void impl::handle_storage::push_to_console_conv(const std::string& astr_info)
{
  m_console_conv.push(astr_info);
};
//---------------------------------------------------------------------------


void impl::handle_storage::push_to_file_conv(const std::string& astr_info)
{
  m_file_conv.push(astr_info);
};
//---------------------------------------------------------------------------
