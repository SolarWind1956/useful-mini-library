//	histogramm_builder.hpp
//
//  	Sergej Edgar Heckel 
//  	17.02.2026
//
// 	Demonstration program: build and print a histogram for a normal distribution
//	
//	Code refined after technical consultation with ChatGPT (OpenAI), 2026.
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
using std::cout; using std::endl;
#include <iterator>
#include <random>
#include <vector>
//	------------------------  «Nice Numbers for Graph Labels» - Paul Heckbert algorithm, Graphics Gems (1990) 
// 	Normalization behaviour was discussed and mathematically verified during technical review.
double 	nice_step(double diff)											//	narrow contract: diff > 0.0
{																		//	diff = max_smpl - min_smpl
    double 	exp 		= 	std::floor(std::log10(diff));	
	/*
		lim(t→0+) floor(log(10^n+t) =  n
		lim(t→0+) floor(log(10^n-t) =  n-1
	*/
    double 	base 	= 	diff / std::pow(10.0, exp);						// 	Normalized number from 1 to 10
	/*
		lim(t→0+) (10^n / 10^(floor(log(10^n+t)) = 10^n / 10^n = 1;		
		lim(t→0+) (10^n / 10^(floor(log(10^n-t)) = 10^n / 10^(n-1) = 10;
	*/
    double 	nice 	= 	(base < 1.5) ? 1 :
						(base < 3)   ? 2 :
						(base < 7)   ? 5 : 10;

    return 	nice * std::pow(10.0, exp);
}
//	-----------------------------------
void 	print_histogramm	(	const std::vector<int>& 	histogram
						,	double 					min_smpl				//	For histogram axis mark
						,	int						bins
						,	double 					step
						)	{
	//	What counter is max
    int 		max_count = *std::max_element(histogram.begin(), histogram.end());
    	
	int		max_bar_height	= 100;										//	Max chart bar height  (or width)
	double 	scale 			= double(max_bar_height) / max_count;			//	Scale to fit chart bars all on the screen

	for (int bin = 0; bin < bins; ++bin) 
	{
		double value = min_smpl + bin * step;								//	Value as histogram axis mark
		cout << std::setw(10) << std::setprecision(5) << value << "\t| ";
		
		std::fill_n	(	std::ostream_iterator<char>(std::cout)			//	Viewing chart bars
					,	int(histogram[bin] * scale)
					, 	'*'
					);
		cout << ' ' << histogram[bin] << '\n';							//	Chart bar numeric marker
	}	
}
//	----------------------------------- 	Build and print histogram for numeric forward range 
template<class It>
	requires std::random_access_iterator<It>								//	std::vector, std::deque, std::array - ✅, 
																		//	but not std::list, std::map, std::set -❌
void histogram(It first, 	It last)
{
	auto 	[min_it, max_it] 	= std::minmax_element(first, last);		//	Max and min samples ​​in signal
    double 			min_smpl		= *min_it;
	double 			max_smpl		= *max_it;

    const auto 		samples_n 	= std::distance(first, last);				//	Samples number in random signal
	 
	int 				bins 		= int(1 + 3.322 * std::log10(samples_n));	//	Number of bins (Sturges rule)
	if (max_smpl == min_smpl) {
		cout << "All samples are equal: ";
		cout << std::setw(10) << max_smpl << "\t| " << std::string(50, '*') << " " << samples_n << '\n';
		return;
	}
    double 			step 		= nice_step((max_smpl - min_smpl) / bins);//	To Paul Heckbert algorithm
		
    std::vector<int> histogram(bins, 0);									//	Histogram vector for display

	for (auto it = first; it != last; ++it) 								//	Distribution elem counters into histogram
	{
        int 	idx 	= 	int((*it - min_smpl) / step);						//	Conditions for the distribution element 
		idx 		= 	std::clamp(idx, 0, bins - 1);						//	to fall into particular bin   
		++histogram[idx];												//	Count numbers of distribution in each bin
    }
	
	print_histogramm	(histogram,	min_smpl	,	bins	,	step	);
}
//  	---------------------------------------------		main()
int main()
{
    constexpr int 					samples_n 		= 10'000;			//	Number of samples in random signal
    std::vector<double> 				signal(samples_n);			
	
	std::mt19937 					rng(std::random_device{}());			//	Mersenne-Twister would be better
	
	auto 							Mu{1.0};							//	Mean value
	auto 							Sigma{3.0};							//	Standard deviation
    std::normal_distribution<double> 	distr(Mu, Sigma);

	for (auto& v : signal) 												//	Generate random signal
		v = distr(rng);

    histogram(signal.begin(), signal.end());								//	Build and print histogramm
}
