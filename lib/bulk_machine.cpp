//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include "bulk_machine.h"
//---------------------------------------------------------------------------
#include "async.h"
//---------------------------------------------------------------------------




// конструктор с одним параметром - там удобно так использовать
impl::t_bulk_machine::t_bulk_machine(size_t aun_stat_bulk_size)
  :m_un_stat_buld_size(aun_stat_bulk_size),
  m_un_dyn_depth(0)
{
  // зарезервируем. Типа чуть пооптимальнее с большой вероятностью
  m_vec_buffer.reserve(m_un_stat_buld_size);
};
//---------------------------------------------------------------------------


// деструктор, т.к. в конце иногда могут оставаться данные, и их нужно тоже распечатать
impl::t_bulk_machine::~t_bulk_machine()
{
  if(m_vec_buffer.size()) {
    impl::logout("<< eof detected. so purging final batch if any:");
    print_batch_and_clear();
  }
};
//---------------------------------------------------------------------------


void impl::t_bulk_machine::handle_instruction(const std::string& astr_inst)
{

  if (!astr_inst.length()) {
    impl::log_err("empty string in handle_instruction()");
    return;
  };


  if(astr_inst == "{") {

    // if we were in regular block:
    if (m_un_dyn_depth == 0) {
      impl::logout("comming dyn block, so purgint regular one if any:");
      print_batch_and_clear();
    }

    m_un_dyn_depth++;
    impl::logout("new dyn depth: " + std::to_string(m_un_dyn_depth));
    return;
  }

  if (astr_inst == "}") {
    if (m_un_dyn_depth >= 1) {
      m_un_dyn_depth--;
      impl::logout("} detected. new depth: " + std::to_string(m_un_dyn_depth));
    }
    else {
      // todo: hope test input will come in correct order. Otherwise ask how to handle such case
      impl::logout("some protocol violation detected. extra closing bracket. just ignored so far");
    }

    if (m_un_dyn_depth == 0) {
      impl::logout("dyn block is finished. printing content: ");
      print_batch_and_clear();
    }

    return;
  }

  
  // если инструкция обычная, то сохраняем её сначала в буфере:
  m_vec_buffer.push_back(std::move(astr_inst));

  if (m_un_dyn_depth == 0) {
    if (m_vec_buffer.size() >= m_un_stat_buld_size) {

      impl::logout("<< regular batch ready ----------");
      print_batch_and_clear();
    }
  }

  return;
};
//---------------------------------------------------------------------------


// выводим текущий буфер куда надо (в файл, в консоль...)
void impl::t_bulk_machine::print_batch_and_clear()
{

  std::string str_bulk;
  for(auto& curr : m_vec_buffer)
  {
    if (str_bulk.length()) {
      str_bulk += ", ";
    }
    str_bulk += curr;
  }

  impl::push_to_console_conv(str_bulk);
  impl::push_to_file_conv(str_bulk);

  m_vec_buffer.clear();
};
//---------------------------------------------------------------------------

