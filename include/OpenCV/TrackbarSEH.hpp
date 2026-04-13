// 	TrackbarSEH.hpp 
/*
	Created on Sunday April 12 2026

	author: SergejHeckelDesktop
			with insightful support from Gemini (AI)
	
	"Дорогу осилит идущий, а вдвоем идти спорее".	
*/
#ifndef INCLUDE_GUARDS_TrackbarSEH_HPP
#define INCLUDE_GUARDS_TrackbarSEH_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "color_output.hpp"
using namespace color_output;

//	--------------------------------------------------------------------------------------------------------
/*
	*********************************************************************************************************
	
*/
class TrackbarSEH 
{
 public:
    // 	Колбэк теперь принимает (src, dst, value)
    using 	FilterFunc = std::function<void(const cv::Mat&, cv::Mat&, int)>;

    TrackbarSEH	(	const std::string& 	_TrackbarName
				, 	const std::string& 	_WindowName
				,	const cv::Mat& 		_src
				, 	cv::Mat& 			_dst 
				, 	int 					_maxVal
				, 	FilterFunc 			func
				)
				: 	TrackbarName(_TrackbarName)
				,	WindowName(_WindowName)
				, 	src(_src)
				, 	dst(_dst)
				,	maxVal(_maxVal)
				, 	applyFilter(func)
    {
        // 	Передаем 'this' в userdata, чтобы статический метод нашел объект
        cv::createTrackbar	(	TrackbarName
							, 	WindowName
							, 	nullptr
							, 	maxVal
							, 	onChange
							, 	this
							);		
    }
	
    int 		getValue() const 	{ return cv::getTrackbarPos(TrackbarName, WindowName); }
    void 	setValue(int value) 	{ cv::setTrackbarPos(TrackbarName, WindowName, value); }

 private:
 
    // 	Статическая оболочка, которая вызывает метод конкретного экземпляра 
    static void onChange(int pos, void* userdata) {
        auto* self = static_cast<TrackbarSEH*>(userdata);
        
        // 	1. Применяем фильтр к локальному контексту
        self->applyFilter(self->src, self->dst, pos);
        
        // 	2. Сразу обновляем окно
        cv::imshow(self->WindowName, self->dst);
    }

	std::string 		TrackbarName 	= "";
	std::string 		WindowName 		= ""; 
	
    const cv::Mat& 	src;
    cv::Mat& 		dst;
	
	int				maxVal 			= 0;
    
	FilterFunc 		applyFilter{};
};
/*
	===================================================================
	ИИ
		Пример использования
	ИИ
	===================================================================	
*/
#if 0	

void MedianBlurAndTrackbarDemo_1() {
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	cout << bright_magenta;

	// 	1. Читаем исходное изображение
	cv::Mat		src = cv::imread("../../images/apple.jpg", cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        	cout << bright_red << "Ошибка загрузки ../../images/apple.jpg" << reset << endl; 
		return;
    }
		
	// 	Создаем окно и отображаем в нем оригинальное изображение
	const std::string	demoWindowName{"MedianBlur filter Demo"};
    cv::namedWindow(demoWindowName, cv::WINDOW_AUTOSIZE);
	cv::imshow(demoWindowName, src);
		
	int 					blur_value 		= 1; 	// 	Начальное значение ползунка
	const int 			max_value 		= 5; 	// 	Максимальное значение положения ползунка
												//	Размеры ядра будут пересчитываться - смотри ниже лябду
	cv::Mat 				dst;	
	
	// Пример 1: Размытие (Median Blur)
    TrackbarSEH 			medianBlurTrackbar	(	"Median Blur Trackbar"
											, 	demoWindowName
											, 	src
											, 	dst
											, 	max_value			//	Максимальное значение ползунка
											, 	[](const cv::Mat& src, cv::Mat& dst, int v) {
													int k = v * 4 + 1; // Только нечетные значения
													cv::medianBlur(src, dst, k);
												}
											);

    // 	Вызываем один раз вручную, чтобы показать картинку сразу
	medianBlurTrackbar.onChange(blur_value, &medianBlurTrackbar);

    cv::waitKey(0);
	
	cout << reset  << endl; 
}
#endif	
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_TrackbarSEH_HPP

