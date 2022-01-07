//---------------------------------------------------------------------------
#include "async.h"
//---------------------------------------------------------------------------
#include "storage.h"
#include "context.h"
//---------------------------------------------------------------------------

// ������ �� ������� ������
extern impl::handle_storage* gp_storage;


async::handle_t async::connect(std::size_t a_un_bulk)
{
  return gp_storage->register_new_handle(a_un_bulk);
};
//---------------------------------------------------------------------------



void async::receive(async::handle_t a_handle, const char* ap_data, std::size_t a_un_size)
{

  // ��� ����� "�����", ������ ��� ����� ��������� ��������� �� �� ���� ��� ����� �� "��������"
  // �� ��� �������� ������� �� ����������� "������������" ����������
  impl::t_context* p_context = gp_storage->get_context(a_handle);
  if (!p_context) {
    return;
  }

  p_context->receive(ap_data, a_un_size);
};
//---------------------------------------------------------------------------


void async::disconnect(async::handle_t a_handle)
{
  gp_storage->destroy_context(a_handle);
};
//---------------------------------------------------------------------------



void impl::log_info(const std::string_view& astr_info)
{
  gp_storage->log_info(astr_info);
};
//---------------------------------------------------------------------------

void impl::logout(const std::string_view& astr_info)
{
  impl::log_info(astr_info);
};
//---------------------------------------------------------------------------


void impl::log_err(const std::string_view& astr_info)
{
  gp_storage->log_err(astr_info);
};
//---------------------------------------------------------------------------

// ��� ������ � std::cout ����� ���� ��� ��������� � �������:
void impl::push_to_console_conv(const std::string& astr_info)
{
  gp_storage->push_to_console_conv(astr_info);
};
//---------------------------------------------------------------------------

// ���������� ������� �� ���������� � ����
void impl::push_to_file_conv(const std::string& astr_info)
{
  gp_storage->push_to_file_conv(astr_info);
};
//---------------------------------------------------------------------------
