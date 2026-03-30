// 	printMatDiagnostics.hpp 
// 	D:/PolygonTESTS/OpenCVBooks/srcKaehler_Bradski_Learning_OpenCV/include/printMatDiagnostics.hpp
/*
	Данный код является учебным конспектом по книге Брэдски и Кэлера "Learning OpenCV 3". 
	
	Все права на оригинальные алгоритмы принадлежат авторам и издательству O'Reilly. 
	
	Код предназначен исключительно для ознакомления и личного обучения.
	
	Created on Sunday Mar 24 2026

	author: SergejHeckelDesktop
			with insightful support from Gemini (AI)
	
	"Дорогу осилит идущий, а вдвоем идти спорее".	
*/
#ifndef INCLUDE_GUARDS_CHAPTER_4_printMatDiagnostics_HPP
#define INCLUDE_GUARDS_CHAPTER_4_printMatDiagnostics_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <map>
#include "color_output.hpp"
using namespace color_output;

#include <opencv2/core/core.hpp>

//	--------------------------------------------------------------------------------------------------------
/*
	Рекомендуемая Gemini терминология, 
	в рамках которой будут наблюдаться результаты диагностики
	
	Иерархия данных в cv::Mat (от Атома к Вселенной)
	
	Предложена следующая структура, основанная на системных параметрах OpenCV:
	
	-	Канальный Атом (Depth)
	
		Что это: 		Минимальная неделимая единица данных (например, один `uchar` или один `float`).
		
		В коде: mat.depth() возвращает идентификатор (например, CV_8U), а mat.elemSize1() - 
		размер одного такого «атома» в байтах.
		
	-	Элемент Большого Массива (Element / Pixel)
	
		Что это: 		Совокупность всех каналов в одной точке пространства.
		
		В коде: mat.channels() показывает количество «атомов» в элементе. 
		
		mat.elemSize() 	- 	это общий вес элемента (например, 3 байта для BGR).
		
		Важно: 		В трехканальном Массиве один элемент - это вектор из 3-х байт.
		
	-	Измерение (Dimension / Axis)
	
		Что это: 		Направление, вдоль которого считаются элементы Большого Массива .
		
		В коде: 		mat.dims. 
		
					Для обычного фото их 2 (X и Y), для 3D-тензора -  (X, Y, Z).
					
	-	Размерность (Size / Shape)
	
		Что это: 		Протяженность Большого Массива по каждому измерению.
		
		В коде: 		Массив mat.size. 
		
					Теперь мы знаем, что выводить его нужно циклом по `dims`, 
					чтобы не потерять «глубину» тензора.
					
	-	Общий объем (Total)
	
		Что это: 		Общее количество Элементов во всем Большом Массиве.
		
		В коде: 		mat.total(). 
*/
/*
	В уважаемой книге Брэдски и Кэлера "Learning OpenCV 3" 
	термин `матрица` очень часто применяется к большим массивам,
	например, cv::Mat, независимо от из размерностей.
	
	Для меня (и возможно для других пользователей) матрица - это массив двумерный.

	По этому я предлагаю в контексте данной функции использовать термин `Большой Массив`.
*/
/*
	Для этой библиотечной функции диагностики многомерного массива cv::Mat, 
	лучше использовать еще функцию-оболочку Mat_Diagnostics(cv::Mat mat)
*/	
#if 0	
inline void Mat_Diagnostics(cv::Mat mat){
	separate_number_with_apostrophe snwa;			
	//  	По умолчанию число разбивается апострофом на группы по три разряда
	snwa.set_separator();
	printMatDiagnostics(mat);
	snwa.locale_restore();	//	Восстанавливаем установленные правила разделения на группы
}
#endif	
/*	
	Её целеособразность связна с тем, что 
	функция диагностики выводит величины количества байтов в массивах,
	и, для лучшей визуализации, 
	эти числа предпочтительно представлять с разбиением на группы десятичных разрядов.
	
	Класс, обеспечивающий такое разбиение,
	`separate_number_with_apostrophe`
	также имеется в моей библиотеке, а именно в заголовочном файле
	separate_number_with_apostrophe.hpp
*/

//	Целесообразность таблиц обусловлена тем, что функции depth() и type() выводят не инфрмативные целые числа
inline const
std::map<int, std::string> elem_types		{ 	{CV_8UC1, 	"CV_8UC1" }, 	{CV_8SC1, 	"CV_8SC1" }
										, 	{CV_8UC2, 	"CV_8UC2" }, 	{CV_8SC2, 	"CV_8SC2" }
										,	{CV_8UC3, 	"CV_8UC3" },		{CV_8SC3, 	"CV_8SC3" }
										,	{CV_8UC4, 	"CV_8UC4" },		{CV_8SC4, 	"CV_8SC4" }
										
										,	{CV_16UC1, 	"CV_16UC1" }
										, 	{CV_16UC2, 	"CV_16UC2" }
										,	{CV_16UC3, 	"CV_16UC3" }
										,	{CV_16UC4, 	"CV_16UC4" }
										
										, 	{CV_16SC1, 	"CV_16SC1" }, 	{CV_32SC1, 	"CV_32SC1" } 	
										, 	{CV_16SC2, 	"CV_16SC2" }, 	{CV_32SC2, 	"CV_32SC2" }
										,	{CV_16SC3, 	"CV_16SC3" },	{CV_32SC3, 	"CV_32SC3" }
										,	{CV_16SC4, 	"CV_16SC4" },	{CV_32SC4, 	"CV_32SC4" }

										,	{CV_16FC1, 	"CV_16FC1" }, 	{CV_32FC1, 	"CV_32FC1" }
										, 	{CV_16FC2, 	"CV_16FC2" }, 	{CV_32FC2, 	"CV_32FC2" }
										,	{CV_16FC3, 	"CV_16FC3" },	{CV_32FC3, 	"CV_32FC3" }
										,	{CV_16FC4, 	"CV_16FC4" },	{CV_32FC4, 	"CV_32FC4" }	
										
										,	{CV_64FC1, 	"CV_64FC1" }
										, 	{CV_64FC2, 	"CV_64FC2" }
										,	{CV_64FC3, 	"CV_64FC3" }
										,	{CV_64FC4, 	"CV_64FC4" }
										};
/*
	Глубина цвета/изображения (Bit Depth): 		Это количество бит, 
											выделяемое для хранения информации о цвете одного пикселя 
											(или одного канала пикселя).
											
		Примеры: 			8 бит (256 оттенков), 16 бит (65 536 оттенков), 32 бита.
	
		На что влияет: 		Качество градиентов, точность цветопередачи.
*/										
inline  const 										
std::map<int, std::string> channel_depth 	{	{CV_8U,		"CV_8U"  }   
										,	{CV_8S,		"CV_8S"  }   
										,	{CV_16U,		"CV_16U" }  
										,	{CV_16S,		"CV_16S" }  
										,	{CV_32S,		"CV_32S" }  
										,	{CV_32F,		"CV_32F" }  
										,	{CV_64F,		"CV_64F" }  
										,	{CV_16F,		"CV_16F" }  
										};
					
inline void		printMatDiagnostics(const cv::Mat& mat)
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	cout << bright_yellow << "--- [Big Mat Diagnostics] ---" << reset << endl;
	
	if (mat.empty()) {
        cout << red << "Большой Массив пуст! Диагностика остановлена. Выход!" << reset << endl;
        return;
    }
	
	// 	Безопасный поиск типа данных (вместо рискованного [])
    auto 		it_type 		= 	elem_types.find(mat.type());
    std::string 	typeName 	= 	(it_type != elem_types.end()) 
							? 	it_type->second 
							: 	"UNKNOWN_TYPE_" + std::to_string(mat.type());

    auto 		it_depth 	= 	channel_depth.find(mat.depth());
    std::string 	depthName 	= 	(it_depth != channel_depth.end()) 
							? 	it_depth->second 
							: 	"UNKNOWN_DEPTH_" + std::to_string(mat.depth());

	//	Базовые параметры
	cout << "Каналов: " << mat.channels() << " | Глубина: " << depthName << endl;
    cout << "Всего элементов: " << mat.total() << endl;
	cout << "Тип элемента (ID): " << typeName << " (" << mat.type() << ")" << endl;
	
	// 	Память и структура
	size_t memSize = mat.total() * mat.elemSize();
    cout << "Память: " << memSize << " байт" << endl;
		
	/*
		cv::Mat::size (которое выводится через cout) - это специальный объект типа cv::MatSize. 
		
		При попытке вывести его напрямую в поток для многомерного Большого массива 
		(где измерений больше двух), стандартный оператор вывода OpenCV 
		по привычке показывает вам только первые два измерения (плоскость), 
		игнорируя остальные.
		
		В чем именно «секрет»?
		
		Хранение данных: 
		
			Объект cv::MatSize: 	Этот объект внутри cv::Mat хранит массив всех размерностей. 
		
		Но когда нужно 
		
		cout << mat.size(), 
		
		срабатывает оператор, оптимизированный для классических 2D-изображений. 
		
		Для него всё, что больше двух измерений - это «экзотика», 
		которую он не умеет красиво печатать «из коробки».
		
		Поле `dims`: 	Для многомерных Больших массивов 
		критически важно проверять количество измерений через `mat.dims`.
	*/	
		
	// 	Вместо простого cout << mat.size()
	cout << "Размерность (dims): " << mat.dims << endl;
	cout << "Форма (shape) : [";
	for (int i = 0; i < mat.dims; ++i) {
		cout << mat.size[i] << (i == mat.dims - 1 ? "" : " x ");
	}
	cout << "]" << endl;
	
    // 	Признаки структуры
    cout << "Непрерывный: " << (mat.isContinuous() ? "Yes" : "No") << endl;
    cout << "Субматрица (ROI): " << (mat.isSubmatrix() ? "Yes" : "No") << endl;

    // 	Анализ диапазона (только для подходящих типов)
    if (mat.depth() == CV_8U || mat.depth() == CV_32F) {
        if (mat.channels() == 1) { 	// 	minMaxLoc работает только для 1 канала
            double minVal, maxVal;
            cv::minMaxLoc(mat, &minVal, &maxVal);
            cout << "Диапазон значений: [" << minVal << " ... " << maxVal << "]" << endl;
        } else {
            cout << "Диапазон: (многоканальный массив, требуется split)" << endl;
        }
    }
}
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_CHAPTER_4_printMatDiagnostics_HPP
