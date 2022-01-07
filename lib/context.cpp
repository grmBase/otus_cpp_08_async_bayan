//---------------------------------------------------------------------------
#include <cstring> //std::strlen
#include <cstddef> //size_t
//---------------------------------------------------------------------------
#include "context.h"
//---------------------------------------------------------------------------




// конструктор с одним параметром - там удобно так использовать
impl::t_context::t_context(size_t aun_bulk)
  :m_bulk_machine(aun_bulk)
{
};
//---------------------------------------------------------------------------


// функция потокового добавления чего-то нового:
void impl::t_context::receive(const char* ap_data, std::size_t a_un_size)
{
  // создаём полную строчку из предыдущего буфера плюс то, что нам передали:
  std::string str_curr = m_buf_not_ready_yet + std::string(ap_data, a_un_size);

  // ищем переводы строк, и обрабатываем:
  while (true) {

    auto iter1 = str_curr.find("\n");
    auto iter2 = str_curr.find("\r\n");

    // Если ничего не нашли:
    if (iter1 == std::string::npos && iter2 == std::string::npos) {
      break;
    }


    if(iter2 != std::string::npos) {
      std::string str_new_to_handle = str_curr.substr(0, iter2);

      m_bulk_machine.handle_instruction(str_new_to_handle);

      str_curr = str_curr.substr(iter2 + std::strlen("\r\n"), std::string::npos);

      continue;
    }


    if (iter1 != std::string::npos) {
      std::string str_new_to_handle = str_curr.substr(0, iter1);

      m_bulk_machine.handle_instruction(str_new_to_handle);

      str_curr = str_curr.substr(iter1 + std::strlen("\r"), std::string::npos);

      continue;
    }

  }

  m_buf_not_ready_yet = str_curr; // запоминаем остаток

};
//---------------------------------------------------------------------------

