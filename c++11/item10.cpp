#include <type_traits>
#include <iostream>
#include <tuple>
#include <string>

using UserInfo = std::tuple<std::string,std::string,std::size_t>;

enum class UserInfoFields {uiName,uiEmail,uiReputation};

template <typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

int main()
{
	UserInfo uInfo{"wong","email",90};
	std::cout<<std::get<1>(uInfo)<<std::endl;
	std::cout<<std::get<toUType(UserInfoFields::uiEmail)>(uInfo)<<std::endl;
	return 0;
}
