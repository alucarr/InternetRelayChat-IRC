#if !defined(UTILS_HPP)
#define UTILS_HPP
#include <iostream>
#include <string>

# define RESET "\033[0m"
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
class Utils
{
private:
	/* data */
public:
	// Utils(/* args */);
	// ~Utils();
	void printer(std::string text);
};



#endif // UTILS_HPP
