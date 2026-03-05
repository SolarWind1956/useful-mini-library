// StopwatchSEH.hpp
//Copyright Ingo Proff 2017.
// https://github.com/CrikeeIP/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)
// https://github.com/ManWithABike/Stopwatch/blob/master/include/stopwatch/Stopwatch.hpp

#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <locale>
#include <thread>
#include <iostream>
using std::cout; using std::endl;
/*
	Секундомер StopwatchSEH может фиксировать	
	длительности следующих элементов:
	
	-	одноразовую продолжительность промежутка времени от момента старта 
		до момента останова в текущий момент времени;
	
	-	подолжительности временных кругов с формированием вектора длительностей кругов
		от старта предыдущего круга до старта очередного круга в текущий момент времени;
		
	-	подолжительности временных дистанций с формированием вектора длительностей дистанций
		от момента первичного старта до момента завершения очередной дистанции;
		
	Единицы продолжительности периодов задаются как параметры шаблонных функций старта/останова
	и могут составлять наносекунды, микросекунды, миллисекунды и секунды - это на момент разработки.
	
	Этот списко может быть расширен минутами, часами и так далее.
	
	Так как речь идет в том числе и о наносекундах, то в реализации используются
	часы с высоким разрешением std::chrono::high_resolution_clock.
	
	Количество заданных параметром шаблона единиц продолжительности возвращается функцией 
	std::chrono::duration::count().
	По умолчанию возвращаются миллисекунды.
*/
namespace stopwatchSEH
{

class StopwatchSEH{
public:
	typedef 	std::chrono::time_point<std::chrono::high_resolution_clock> 	time_pnt;
	
	enum 		TimeFormat{ NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS };
	//	************* Конструкторы	и деструктор	**************************************************
	StopwatchSEH()	:	 
						start_time_point	()
					, 	laps_time_pnts		({})
					, 	distances_time_pnts	({})
					,	isStarted			{false}
					,	isLapsStarted		{false}
					,	isDistancesStarted	{false}
	{
		// Блок для введения символа апострофа как разделителя десятичных разрядов 
		// при выводе результатов тайминга
		#if defined (__clang__) || defined (_MSC_VER) || defined (_MSC_FULL_VER) 
			std::locale::global(std::locale("ru_RU.utf8"));
		#elif defined (__GNUC__) 	|| 	defined (__GNUG__) 	&& 	not defined (__clang__) 
			std::locale::global(std::locale(""));
		#endif
		
		cout << "Экземпляр секундомера StopwatchSEH() создан и готов к работе! :)" << endl;
		{
			struct space_out : std::numpunct<char> {
				char		do_thousands_sep()		const { return 0x27; }  // separate with apostrophe
				std::string	do_grouping()		const { return "\3"; }	// groups of 1 digit
			};
			old_locale = cout.imbue(std::locale(std::cout.getloc(), new space_out));
		}
	}
	~StopwatchSEH()
	{
		// Вернем локаль в первоначальное состояние в отношении разделителя десятичных разрядов
		cout.imbue(old_locale);
		cout << "Экземпляр секундомера StopwatchSEH() удален деструктором :)" << endl;
		#if defined (__clang__) || defined (_MSC_VER) || defined (_MSC_FULL_VER) 
			std::locale::global(std::locale("ru_RU.utf8"));
		#elif defined (__GNUC__) 	|| 	defined (__GNUG__) 	&& 	not defined (__clang__) 
			std::locale::global(std::locale(""));
		#endif
	}
	// 	Методы 
	//	-------------------------------------------------------------------------------------------------
	//  Режим фиксации промежутка времени от старта до останова - 
	//  длительность между двумя моментами времени
	void start(){
		if (!isStarted){
			isStarted			= true;
			cout	<< 	"------------------- StopwatchSEH is starting...  :)"	<< endl;
			start_time_point 	= std::chrono::high_resolution_clock::now();
			return;
		}	
		else {
			cout	<< 	"------------------- StopwatchSEH is working! :( ... First stop it!"	<< endl;
			return;
		}		
	}
	void stop(){
		// Сначала зафиксируем момент времени, а потом будем разбираться с запуском и остановом
		auto 	end_time_point = std::chrono::high_resolution_clock::now();
		if (isStarted){
			cout	<< 	"---------------- StopwatchSEH is stoping and printing results... :)"	<< endl;
			
			//	Get elapsed time..
			// .. in nanoseconds
			uint64_t 	elapsed_ns 	= ticks<TimeFormat::NANOSECONDS>(start_time_point, end_time_point);
			// .. in microseconds
			uint64_t 	elapsed_mus = ticks<TimeFormat::MICROSECONDS>(start_time_point, end_time_point);
			// .. in milliseconds 
			uint64_t 	elapsed_ms 	= ticks<TimeFormat::MILLISECONDS>(start_time_point, end_time_point);
			// .. in seconds
			uint64_t 	elapsed_s 	= ticks<TimeFormat::SECONDS>(start_time_point, end_time_point);

			//Print to screen
			cout << elapsed_ns 	<< " ns" 	<< endl;
			cout << elapsed_mus << " mus" 	<< endl;
			cout << elapsed_ms 	<< " ms" 	<< endl;
			cout << elapsed_s 	<< " s" 	<< endl;
			cout << "---------------" 		<< endl;
			
			isStarted	=	false;
			return;
		}	
		else {
			cout	<< 	"-------------- StopwatchSEH hasn't been worked :( First start it! :)"	<< endl;
		}	
	}
	//	*************************************************************************************************
	//	----  Круги - Laps -----------------------------------------------------------------------------
	//  Режим фиксации промежутка времени длительности кругов от старта до останова - 
	//  длительность между двумя моментами времени
	//	------------------------------------------------------------------------------------------------
	// Добавляет текущий (натоящий) момент времени в вектор кругов секундомера
	void 	lap_start()
	{
		static 	int		first_start	=	1;		// Флаг первого входа
		if (first_start != 1){
			// Добавляем текущий (настоящий) момент времени очередного круга
			laps_time_pnts.push_back(std::chrono::high_resolution_clock::now());
		} else {			
			first_start++;
			cout	<< 	"------------------- StopwatchSEH::Laps is starting... :)"	<< endl;
			isLapsStarted	=	true;
		}	
		return;	
	}
	void	laps_stop()
	{
		if(isLapsStarted){
			isLapsStarted	=	false;
			lap_start();		// Возможно не полный круг
			cout	<< 	"----------- StopwatchSEH::Laps is stoping and printing results... :)"	<< endl;
		
			auto 		laps_time_ticks 	= 	laps_elapsed_time_ticks<TimeFormat::NANOSECONDS>();
			cout << "Laps Total, ns: " << laps_time_ticks.first 	<< " ; ";
			auto 		laps_times 			= 	laps_time_ticks.second;
			cout << "Laps Times, ns: " ; 	show_times(laps_times);
			
			laps_time_ticks 				= 	laps_elapsed_time_ticks<TimeFormat::MICROSECONDS>();
			cout << "Laps Total, mus: " << laps_time_ticks.first 	<< " ; ";
			laps_times 						= 	laps_time_ticks.second;
			cout << "Laps Times, mus: " ; 	show_times(laps_times);
						
			laps_time_ticks 				= 	laps_elapsed_time_ticks<TimeFormat::MILLISECONDS>();
			cout << "Laps Total, ms: " << laps_time_ticks.first 	<< " ; ";
			laps_times 						= 	laps_time_ticks.second;
			cout << "Laps Times, ms: " ; 	show_times(laps_times);			
			
			laps_time_ticks 				= 	laps_elapsed_time_ticks<TimeFormat::SECONDS>();
			cout << "Laps Total, s: " << laps_time_ticks.first 	<< " ; ";
			laps_times 						= 	laps_time_ticks.second;
			cout << "Laps Times, s: " ; 	show_times(laps_times);			
			
			cout << "\n---------------" 		<< endl;
			// Очистим массив кругов
			laps_time_pnts.erase(laps_time_pnts.begin());
			return;
		}	else {	
			cout	<< 	"-------------- StopwatchSEH::Laps wasn't started! First start it! :)"	<< endl;
			return;
		}	
	}
	//	------------------------------------------------------------------------------------------------
	// Метод формирования счетчика тактов для кругов секундомера в отдельности и всех вместе
	template	<	TimeFormat time_fmt_total 			= TimeFormat::MILLISECONDS
				, 	TimeFormat time_fmt_lap_time_pnt 	= time_fmt_total
				>
	std::pair<std::uint64_t, std::vector<std::uint64_t>> 	laps_elapsed_time_ticks()
	{
		// Вспомогательный вектор счетчиков тактов кругов секундомера
		std::vector<std::uint64_t> 	laps_time_ticks;
		// Размер вектора с декрементом
		std::size_t 				ltp_size	=	laps_time_pnts.size() - 1;	
		// В соответствии с текущим размером вектора моментов времени кругов
		laps_time_ticks.reserve(ltp_size);
		// Цикл заполнения вспомогательного вектора счетчиками тактов кругов секундомера
		--ltp_size;			//	Декремент в порядке верхней границы индекса предвычисления для цикла
		for(std::size_t idx = 0; idx <= ltp_size; idx++){
			const auto lap_end_time_pnt 		= laps_time_pnts[idx+1];
			const auto lap_start_time_pnt 		= laps_time_pnts[idx];
	
			laps_time_ticks.push_back(ticks<time_fmt_lap_time_pnt>	(	lap_start_time_pnt
																	, 	lap_end_time_pnt));
		}

		return  {	ticks<time_fmt_total>	(	start_time_point
											, 	laps_time_pnts.back()
											)
											, 	laps_time_ticks
				};
	}
	//	***********************************************************************************************
	//	----  Дистанции - Distances -------------------------------------------------------------------
	//  Режим фиксации промежутка времени длительности дистанций от старта до останова - 
	//  длительности между двумя моментами времени
	//	------------------------------------------------------------------------------------------------
	// Добавляет текущий (натоящий) момент времени в вектор моментов времени дистанций секундомера
	// и возвращает счетчик тактов предыдущей дистанции по отношению к настоящему моменту времени
	void 	distance_start()
	{
		static 	int		first_start		=	1;		// Флаг первого старта
		if (first_start != 1){
			// Добавляем текущий (настоящий) момент времени очередной дистанции
			distances_time_pnts.push_back(std::chrono::high_resolution_clock::now());
		} else {
			cout	<< 	"------------------- StopwatchSEH::Distances is starting... :) "	<< endl;
			first_start++;
			isDistancesStarted	=	true;
		}
	}
	void	distances_stop()
	{
		if(isDistancesStarted){
			isDistancesStarted	=	false;
			cout	<< 	"------- StopwatchSEH::Distances is stoping and printing results... :)"	<< endl;
		
			auto 	distances_time_ticks 	= 	distances_elapsed_time_ticks<TimeFormat::NANOSECONDS>();
			cout << "Laps Total, ns: " << distances_time_ticks.first 	<< " ; ";
			auto 	distances_times 		= 	distances_time_ticks.second;
			cout << "Laps Times, ns: " ; 	show_times(distances_times);
			
			distances_time_ticks 			= 	distances_elapsed_time_ticks<TimeFormat::MICROSECONDS>();
			cout << "Laps Total, mus: " << distances_time_ticks.first 	<< " ; ";
			distances_times 				= 	distances_time_ticks.second;
			cout << "Laps Times, mus: " ; 	show_times(distances_times);
						
			distances_time_ticks 			= 	distances_elapsed_time_ticks<TimeFormat::MILLISECONDS>();
			cout << "Laps Total, ms: " << distances_time_ticks.first 	<< " ; ";
			distances_times 				= 	distances_time_ticks.second;
			cout << "Laps Times, ms: " ; 	show_times(distances_times);			
			
			distances_time_ticks 			= 	distances_elapsed_time_ticks<TimeFormat::SECONDS>();
			cout << "Laps Total, s: " << distances_time_ticks.first 	<< " ; ";
			distances_times 				= 	distances_time_ticks.second;
			cout << "Laps Times, s: " ; 	show_times(distances_times);			
			
			cout << "\n---------------" 		<< endl;
			distances_time_pnts.erase(distances_time_pnts.begin());	// Сбросим вектор дистанций
		}	else	{
			cout	<< 	"--------- StopwatchSEH::Distances wasn't started! First start it! :)"	<< endl;
			return;
		}		
	}
	//	-----------------------------------------------------------------------------------------------
	// Метод формирования счетчика тактов для дистанций секундомера в отдельности и всех вместе
	template	<	TimeFormat time_fmt_total 			= TimeFormat::MILLISECONDS
				, 	TimeFormat time_fmt_lap_time_pnt 	= time_fmt_total
				>
	std::pair<std::uint64_t, std::vector<std::uint64_t>> 	distances_elapsed_time_ticks()
	{
		// Вспомогательный вектор счетчиков тактов дистанций секундомера
		std::vector<std::uint64_t> 	distances_time_ticks;
		// Размер вектора с декрементом 
		std::size_t 				dtp_size	=	distances_time_pnts.size() - 1;	
		// В соответствии с текущим размером вектора моментов времени дистанций
		distances_time_ticks.reserve(dtp_size);
		// Цикл заполнения вспомогательного вектора счетчиками тактов дистанций секундомера
		for(std::size_t idx = 0; idx <= dtp_size; idx++){
			distances_time_ticks.push_back(ticks<time_fmt_lap_time_pnt>	(	start_time_point
																		, 	distances_time_pnts[idx]));
		}

		return  {	ticks<time_fmt_total>	(	start_time_point
											, 	distances_time_pnts.back()
											)
											, 	distances_time_ticks
				};
	}
	//	**********************************************************************************************
	//	----------------------------------------------------------------------------------------------
private:
	time_pnt 				start_time_point;	// Момент времени для часов с высоким разрешением
	std::vector<time_pnt> 	laps_time_pnts;		// Вектор моментов времени для начал кругов секундомера
	std::vector<time_pnt> 	distances_time_pnts;// Вектор моментов времени 
												// для завершений дистанций секундомера
	// Флаги запуска соответствующих секундомеров
	bool					isStarted{false};
	bool					isLapsStarted{false};
	bool					isDistancesStarted{false};
	std::locale 			old_locale;			// Для отключения апострофа 
												// как разделителя десятичных разрядов
	//	-----------------------------------------------------------------------------------------------
	// 	Возвращает счетчик тактов в заданных периодах между двумя моментами времени
	template<TimeFormat 	time_fmt = TimeFormat::MILLISECONDS>
	static std::uint64_t 	ticks(const time_pnt& 	start_time_point, const time_pnt& 	end_time_point)
	{
		// Разность моментов времени - суть длительность в терминах библионтеки std::chrono
		const auto 				duration = end_time_point - start_time_point; 		// В миллисекундах
		// Теперь приведем длительность в наносекунды и сосчитаем такты, очевидно в предположении
		// один такт равен одной наносекунде для современного процессора настольного компьютера
		const std::uint64_t 	ns_count = 
								std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

		switch(time_fmt){
			case TimeFormat::NANOSECONDS:
			{
				return ns_count;
			}
			case TimeFormat::MICROSECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100)%10 >= 5) ? 1 : 0;	// Разряд округления
				const auto 		mus_count 	= ( ns_count / 1'000) + up;
				return 			mus_count;
			}
			case TimeFormat::MILLISECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100'000)%10 >= 5) ? 1 : 0;// Разряд округления
				const auto 		ms_count 	= ( ns_count / 1'000'000) + up;
				return 			ms_count;
			}
			case TimeFormat::SECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100'000'000)%10 >= 5) ? 1 : 0;	
																					// Разряд округления
				const auto 		s_count 	= ( ns_count / 1'000'000'000) + up;
				return s_count;
			}
		}	// End of switch
    }
	// Выводит на консоль длительности кругов и дистанций
	//	---------------------------------------------------------------------------------------------
	void 	show_times(const std::vector<std::uint64_t>& time_ticks)
	{
		cout << "{";
		for(const auto& ticks : time_ticks){
			cout << ticks << ", ";
		}
		cout << "}";
		
		cout << endl;
	}
};

constexpr StopwatchSEH::TimeFormat ns 				= StopwatchSEH::TimeFormat::NANOSECONDS;
constexpr StopwatchSEH::TimeFormat mus 			= StopwatchSEH::TimeFormat::MICROSECONDS;
constexpr StopwatchSEH::TimeFormat ms 				= StopwatchSEH::TimeFormat::MILLISECONDS;
constexpr StopwatchSEH::TimeFormat s 				= StopwatchSEH::TimeFormat::SECONDS;

constexpr StopwatchSEH::TimeFormat nanoseconds 	= StopwatchSEH::TimeFormat::NANOSECONDS;
constexpr StopwatchSEH::TimeFormat microseconds 	= StopwatchSEH::TimeFormat::MICROSECONDS;
constexpr StopwatchSEH::TimeFormat milliseconds 	= StopwatchSEH::TimeFormat::MILLISECONDS;
constexpr StopwatchSEH::TimeFormat seconds 		= StopwatchSEH::TimeFormat::SECONDS;

}	// namespace stopwatchSEH

void stopwatchSEH_test()
{
	cout << "void stopwatchSEH_test()" << endl;
	
	//Namespace alias
	namespace sw = stopwatchSEH;

	//Create and start a stopwatch
	sw::StopwatchSEH 		my_watch;
	
	//	Секундомер общий
	my_watch.start();

	//	Секундомер кругов
	my_watch.lap_start();
	
	//	Секундомер дистанций
	my_watch.distance_start();
	
	for(std::size_t i = 1; i <= 4'000'000'000; i++){
		if( i % 800'000'000 == 0){
			my_watch.lap_start();
			my_watch.distance_start();
		}
	}
	
	my_watch.stop();	
	my_watch.distances_stop();
	std::this_thread::sleep_for(4'700ms); 	// Это чтобы получился еще один круг
	my_watch.laps_stop();
	
}