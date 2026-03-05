// 	convolutions.hpp 
#ifndef INCLUDE_GUARDS_convolutions_HPP
#define INCLUDE_GUARDS_convolutions_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <random>
#include <limits>
#include <cmath>
#include <cfloat>
#include <cstddef>
#include <type_traits>
#include <cmath>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
/*
	----------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
//	
//using	RAIt		=	typename RandomAccessIterator;
template<typename RAIt>
void		Convolution	(	RAIt			begin_of_input_signal
					,	RAIt			end_of_input_signal
					,	RAIt			begin_of_impulse_response
					,	RAIt			end_of_impulse_response
					,	RAIt			begin_of_output_signal
					,	RAIt			end_of_output_signal
					) {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		
	const	int	IN_SIGNAL_DIM			=	std::distance	(	begin_of_input_signal
															,	end_of_input_signal
															);
	const	int	IMPULSE_RESPONSE_DIM		=	std::distance	(	begin_of_impulse_response
															,	end_of_impulse_response
															);							
	const	int	OUT_SIGNAL_DIM			=	std::distance	(	begin_of_output_signal
															,	end_of_output_signal
															);
	
	if (OUT_SIGNAL_DIM	<	(IN_SIGNAL_DIM + IMPULSE_RESPONSE_DIM - 1)){
		cout		<< "Итераторы сигналов и импульного отклика указаны неверно. Конец работы" << endl;
	};														
	//	-------------------------------------------------------------------------------------------------------------------------------------------------------
	//	Метод обратной свертки
	RAIt in_it			=	begin_of_input_signal;
	RAIt out_it			=	begin_of_output_signal;
	RAIt imp_resp_it		=	begin_of_impulse_response;
	
	//	Цикл по отсчетам вЫходного сигнала
	for (int t{0}; t < OUT_SIGNAL_DIM; ++t)	{
		//	Инициализируем текущий отсчет выходного сигнала для накопления
		out_it[t] = 0.0;				
		/*
		  	Цикл по отсчетам сигнала 'импульсного отклика'	- 'свертка' 
			начинается с конца массива 'импульсного отклика'
			и сворачивается с таким же количеством отсчетов исходного сигнала  
			и также в обратном порядке 
		*/	
		for (int n{IMPULSE_RESPONSE_DIM - 1}; n >= 0; --n) {
			if (n > t)
				continue;
			if (n < (t - IN_SIGNAL_DIM + 1)) 
				break;
			out_it[t] 	= 	out_it[t] 	
						+ 	imp_resp_it[n] * in_it[t - n];
		}	
	}
	#if 0	
	//	Цикл по отсчетам вЫходного сигнала
	for (int i{0}; i < OUT_SIGNAL_DIM; ++i)	{
		*(out_it + i) = 0.0;				//	Инициализируем текущий отсчет выходного сигнала для накопления
		//  	Цикл по отсчетам сигнала 'импульсного отклика'	- свертка начинается с конца массива 'импульсного отклика'
		//	и сворачивается с таким же количеством отсчетов исходного сигнала  и также в обратном порядке 
		for (int k{IMPULSE_RESPONSE_DIM - 1}; k >= 0; --k) {
			if (i < k)
				continue;
			if (i > (IN_SIGNAL_DIM -1 + k)) 
				break;
			*(out_it + i) 	= 	*(out_it + i) 	
							+ 	*(imp_resp_it + k) * (*(in_it + i - k));
		}	
	}	
	#endif	
	std::copy(begin_of_output_signal, end_of_output_signal, std::ostream_iterator<double>(std::cout, "     ")); cout << endl;
}	
//	*************************************************************************************************************************
static	void		Convolution_using_input_side_algorithm() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	const	int	IN_SIGNAL_DIM		{11};				//	Размер массива отсчетов входного сигнала
	const	int	IMPULSE_RESPONSE_DIM	{5};				//	Размер массива отсчетов импульсного отклика
	//	Размер массива отсчетов выходного сигнала с учетом свертки входного сигнала и импульсного отклика
	const	int	OUT_SIGNAL_DIM		{IN_SIGNAL_DIM + IMPULSE_RESPONSE_DIM - 1};
	
	double		input_signal		[IN_SIGNAL_DIM]	        {0.0, -1.0, -1.2, 2.0, 1.5, 1.5, 0.5, 0.0, -0.5, 3.14, 2.71};
	double		impulse_response	[IMPULSE_RESPONSE_DIM]	{1.0, -0.8, -0.4, -0.2, 0.7};
	
	double		output_signal			[OUT_SIGNAL_DIM]					{0.0};
	double		output_signal_components	[IN_SIGNAL_DIM][OUT_SIGNAL_DIM]	{0.0}; 
	
	//	-------------------------------------------------------------------------------------------------------------------------------------------------------
	//	1.	Способ с формированием промежуточного двумерного массива компонентов выходного сигнала
	
	//	Циклы формирования компонентов выходного сигнала
	//	Цикл по отсчетам входного сигнала
	for (int i{}; i < IN_SIGNAL_DIM; ++i)	 
		//  	Цикл по отсчетам сигнала импульсного отклика
		for (int k{}; k < IMPULSE_RESPONSE_DIM; ++k) 
			output_signal_components[i][i + k] = input_signal[i] * impulse_response[k];
		
	//	Циклы распечатки отсчетов компонетов выходного сигнала
	for (int i{}; i < IN_SIGNAL_DIM; ++i)	 {
		for (int j{}; j < OUT_SIGNAL_DIM; ++j) 
			cout << std::setw(6) << output_signal_components[i][j] << "   ";
		cout << endl;
	}
	
	//	Циклы суммирования компонетов выходного сигнала и получение самого выходного сигнала
	for (int i{}; i < IN_SIGNAL_DIM; ++i)	 
		for (int j{}; j < OUT_SIGNAL_DIM; ++j) 
			output_signal[j] = output_signal[j] + output_signal_components[i][j];
	
	//	Цикл распечатки отсчетоов выходного сигнала
	for (int j{}; j < OUT_SIGNAL_DIM; ++j) 
		cout << std::setw(6) << output_signal[j] << "   ";
	cout << endl;
	
	//	-------------------------------------------------------------------------------------------------------------------------------------------------------
	//	2.	Способ без формирования промежуточного двумерного массива компонентов выходного сигнала
	std::fill(std::begin(output_signal), std::begin(output_signal) + OUT_SIGNAL_DIM, 0.0);
	//	Циклы формирования выходного сигнала
	//	Цикл по отсчетам входного сигнала
	for (int i{}; i < IN_SIGNAL_DIM; ++i)	
		//  	Цикл по отсчетам сигнала 'импульсного отклика'
		for (int k{}; k < IMPULSE_RESPONSE_DIM; ++k) 
			output_signal[i + k] = output_signal[i + k] + input_signal[i] * impulse_response[k];

	//	Цикл распечатки отсчетоов выходного сигнала
	for (int j{}; j < OUT_SIGNAL_DIM; ++j) 
		cout << std::setw(6) << output_signal[j] << "   ";
	cout << endl;
	
	//	-------------------------------------------------------------------------------------------------------------------------------------------------------
	//	3. 	Способ обратной свертки
	
	//	Цикл по отсчетам вЫходного сигнала
	for (int i{}; i < OUT_SIGNAL_DIM; ++i)	{
		output_signal[i] = 0.0;				//	Инициализируем текущий отсчет выходного сигнала для накопления
		//  	Цикл по отсчетам сигнала 'импульсного отклика'	- свертка начинается с конца массива 'импульсного отклика'
		//	и сворачивается с таким же количеством отсчетов исходного сигнала  и также в обратном порядке 
		for (int k{IMPULSE_RESPONSE_DIM - 1}; k >= 0; --k) {
			if (i < k)
				continue;
			if (i > (IN_SIGNAL_DIM + k)) 
				break;
			output_signal[i] = output_signal[i] + impulse_response[k] * input_signal[i - k];
		}	
	}	
	//	Цикл распечатки отсчетоов выходного сигнала
	for (int j{}; j < OUT_SIGNAL_DIM; ++j) 
		cout << std::setw(6) << output_signal[j] << "   ";
	cout << endl;	
	
	//	------------------------------------------------------------------------------------------------------------------------------------------------------------
}	


//	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_convolutions_HPP
