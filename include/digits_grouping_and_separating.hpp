#pragma once
//
//     digits_grouping_and_separating.hpp
//
//  	Sergej Edgar Heckel 
//  	20.02.2026
//
// 	Demonstration program: prints arithmetical types variables and literals
//						  with grouping and group separating of digits.
//	Code refined after technical consultation with ChatGPT (OpenAI), 2026.
//
// 	C++20 or higher
//	
#include <iostream>
#include <string>
#include <algorithm>
#include <bitset>										//	for comparison and checking, but...
#include <concepts>
#include <limits>
#include <cassert>
#include <locale>
#include <type_traits>
#include <cstddef>										//	for std::byte
#include <cstdint> 										// 	for uintptr_t
//	----------------------------------- concepts
template <typename T>
concept IntegralTypesOrByte 	= 	(std::is_integral_v<std::remove_cvref_t<T>> 
						|| 	std::is_same_v<std::remove_cvref_t<T>, std::byte>); 

template <typename T>
concept ArithmeticTypes 	= 	(std::is_arithmetic_v<std::remove_cvref_t<T>> 
						|| 	std::is_same_v<std::remove_cvref_t<T>, std::byte>); 

template <auto BaseTag>
concept BaseTagRestrict 	= 	(BaseTag == 'D' || BaseTag == 'H' || BaseTag == 'O' || BaseTag == 'B');

//	------------------------------------ 	template <> 		std::string 	integ_bin_str()
template <IntegralTypesOrByte T = int>		
std::string 		to_bin_str_with_sep(T integr_param, char separator = '\'')
{
	if constexpr (std::same_as<T, bool>) {
        return integr_param ? "0b1" : "0b0";				//	result string for bool 
	}	

	using 			unsigned_t 	= 	std::conditional_t
									<	std::is_same_v<T, bool> || std::is_same_v<T, std::byte>
									,	std::type_identity<unsigned char> 	// 	Wrapper for bool (C++20)
									, 	std::make_unsigned<T>              	// 	Wrapper for another types
									>::type; 								// 	Type of only the selected branch
	constexpr 	
	std::size_t		bits_n		=	std::numeric_limits<unsigned_t>::digits;
		
	std::string		res_string{};											//	Result binary number string 
	
	res_string.reserve(bits_n + bits_n / 4 + 2);
	
	std::size_t		res_string_sz	= 	bits_n;
	
	res_string 						+= 	"0b";	
	
	unsigned_t 		value 			= 	static_cast<unsigned_t>(integr_param);
	
	for (std::size_t i = 0; i < bits_n; ++i)
    {
        std::size_t 		r_shift 	= 	bits_n - 1 - i;							//	Shift entire value to the right
		
        res_string 				+= 	((value >> r_shift) & 1u) ? '1' : '0';

        if ((i + 1) % 4 == 0 && i + 1 != bits_n)								//	It needn't ending separator
            res_string += separator;
    }

    return res_string;
}
//	--------------------------------------- 	template <>		class set_group_and_separator
//	'D' - decimal, 'H' - hexdecimal, 'O' - octal; Digits Group Separator, default = apostrophe 
template	<char BaseTag = 'D', 	char Separator	= '\''>
	requires (BaseTag == 'D' || BaseTag == 'H' || BaseTag == 'O')
class ScopedLocaleGrouping final
{
 private:	
	static constexpr char 	group_sz =  (BaseTag == 'H') ? 4 : 3;	//	4 or 3 means repeat grouping of 4 or 3 digits
	std::locale 				old_locale;
	std::ostream& 			out_stream;			

	//	 Custom numpunct facet for digit grouping
	struct custom_numpunct : std::numpunct<char> 	
	{
		//	Define digit grouping separator symbol (e.g., apostrophe -  '\'')
		char				do_thousands_sep()	const override 	{ return Separator; }  
		
		//	Define digit grouping pattern (e.g., group_sz for repeat grouping)
		std::string 		do_grouping() const override  { return std::string(1, group_sz); }	
	};
 public:			
	//	CTOR and DETOR
	explicit ScopedLocaleGrouping(std::ostream& 	os)
        : 	out_stream(os)
		,   old_locale(os.imbue(std::locale(os.getloc(), new custom_numpunct)))	//	save old locale and set new one
    {}

    ~ScopedLocaleGrouping()    { out_stream.imbue(old_locale); }	//	restore old locale
	
	//	Objects of this class must not be copied or moved
	ScopedLocaleGrouping(const ScopedLocaleGrouping&) 				= delete;
	ScopedLocaleGrouping& operator=(const ScopedLocaleGrouping&) 	= delete;
	
	ScopedLocaleGrouping(ScopedLocaleGrouping&&) 					= delete;
	ScopedLocaleGrouping& operator=(ScopedLocaleGrouping&&) 		= delete;
};
//	----------------------------------- void 	template <typename T> print_bin()
template <IntegralTypesOrByte T>
void 	print_bin(T number_par, char Separator = '\'', std::ostream& os = std::cout){
	
		os << "Number: " << to_bin_str_with_sep<T>(number_par, Separator)	<< '\n'; 
}
//	----------------------------------- void 	print_dec_oct_hex()	
template		<typename T,	auto BaseTag = 'D', char Separator = '\''>
	requires BaseTagRestrict<BaseTag>
void 	print_dec_oct_hex(T number_par, std::ostream& os = std::cout)
{
	ScopedLocaleGrouping<BaseTag, Separator> 		slg(os);		//	imbuing stream with modified locale (std::numpunct)
	
	if 	constexpr (BaseTag == 'D')
        os << std::dec;
	else if constexpr (BaseTag == 'O')
        os << std::oct;
    else if constexpr (BaseTag == 'H')
        os << std::hex;

	os << "Number: " << std::uppercase << std::showbase << +number_par << '\n';		//	unary +:  trich for std::byte
}
//	--------------------------------------------	void 	print_with_separator()
template <ArithmeticTypes T = int, auto BaseTag = 'D', char Separator = '\''>
	requires BaseTagRestrict<BaseTag>
void 	print_with_separator(T 	number_par, std::ostream& os = std::cout)
{
	if constexpr (BaseTag == 'B') {						//	For binary literal and bool variable
		print_bin<T>								(number_par, Separator, os);
	} 	else 	{										//	For rest types variables and literals
		print_dec_oct_hex<T, BaseTag, Separator>	(number_par, os);
	}
}

//  	---------------------------------------------		main()
int		main_test()
{
	std::cout << "-------------- I'm " << __PRETTY_FUNCTION__ << '\n';
	
	print_with_separator<int>(1'777'789);
	print_with_separator<int>(-1'777'789);
	
	print_with_separator<int, 'D', ','>(1'777'789);
	
	print_with_separator<int, 'O'>(01'777'751);
	
	print_with_separator<long long, 'H'>(0x1'ABC7'DEF7);

	print_with_separator<double>(1'777.789, std::cerr);
	
	print_with_separator<int, 'B'>(0b1101'1101, std::cerr);
	print_with_separator<short, 'B'>(0b1101'1101);
	
	const std::byte	byte_var 	= std::byte{0b1101'1101};
	print_with_separator<std::byte, 'B'>(byte_var);
	
	print_with_separator<unsigned char, 'D'>(static_cast<unsigned char>(byte_var));
	print_with_separator<char, 'D'>(static_cast<char>(byte_var));

	print_with_separator<unsigned char, 'B'>(static_cast<unsigned char>(byte_var));
	print_with_separator<char, 'B'>(static_cast<char>(byte_var));
	
	print_with_separator<bool, 'B'>(true);
	print_with_separator<bool, 'B'>(false);
	
	print_with_separator<int, 'D'>(221);
	print_with_separator<int, 'D'>(-35);
	
	print_with_separator<int, 'B'>(221);
	print_with_separator<int, 'B'>(-35);

	int 		int_var{};
	int*		int_var_ptr = &int_var;
	print_with_separator<long long, 'H'>(reinterpret_cast<uintptr_t>(int_var_ptr));
	
	return 0;
}

