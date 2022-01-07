//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <cstddef>
#include <string_view>
#include <string>
//---------------------------------------------------------------------------

namespace async 
{


// объявление хендла:
using handle_t = void *;


// функции "интерфейса" - C-стайл (хотя и в нэймспейсе) - требование условия ДЗ
handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

}; // end of namespace async




// внутренний "интерфейс" для "не смешанного" вывода в std::cout, std::cerr
namespace impl
{

// моя "многопоточная" отладка
void logout(const std::string_view& astr_info);
void log_info(const std::string_view& astr_info);
void log_err(const std::string_view& astr_info);


// для вывода в std::cout через нить как требуется в задании:
void push_to_console_conv(const std::string& astr_info);
// закидываем задание на сохранение в файл
void push_to_file_conv(const std::string& astr_info);

};


//---------------------------------------------------------------------------