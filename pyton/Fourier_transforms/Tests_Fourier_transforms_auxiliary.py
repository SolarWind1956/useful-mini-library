# -*- coding: utf-8 -*-
"""
Tests_Fourier_transforms_auxiliary.py

Upgrated on Mon Mar 9 10:27:05 2026

@author: SergejHeckelDesktop
"""

import matplotlib.pyplot as plt
import numpy as np
import math

from Fourier_transforms import Fourier_coefficients
from Fourier_transforms import definite_integral

# %matplotlib inline


'''
    --------------------------------------------------------------------------------------
'''


def plot_a_graph(abscissas, ordinates):

    plt.plot(abscissas, ordinates)
    plt.grid()
    plt.draw()


def plot_a_few_graph():
    abscissas = np.linspace(0, 10, 11)

    ordinates = [25, 32, 34, 27, 25, 37, 34, 40, 45, 42, 50]
    plot_a_graph(abscissas, ordinates)

    running_mean = np.linspace(0, 10, 11)
    running_mean[0] = ordinates[0]

    # Определение скользящего среднего - moving average - усреднение с нарастающим итогом
    for index in range(1, 11):
        running_mean[index] = (running_mean[index - 1]
                               * index + ordinates[index]) / (index + 1)
        print(running_mean[index])

    plot_a_graph(abscissas, running_mean)

    # Сглаживание - flattening
    flattening_curve = ordinates
    for index in range(2, 9):
        flattening_curve[index] = (flattening_curve[index - 2] + flattening_curve[index - 1]
                                   + flattening_curve[index]
                                   + flattening_curve[index + 1] +
                                   flattening_curve[index + 2]
                                   ) / 5

        print(flattening_curve[index])

    plot_a_graph(abscissas, flattening_curve)

    flattening_curve = ordinates
    for index in range(2, 11):
        flattening_curve[index] = (flattening_curve[index - 2] + flattening_curve[index - 1]
                                   + flattening_curve[index]
                                   ) / 3

        print(flattening_curve[index])

    plot_a_graph(abscissas, flattening_curve)


'''
    --------------------------------------------------------------------------------------
'''


def build_and_plot_x():

    sample_n = 100
    abscissas = np.linspace(-2.0 * math.pi, 2.0 * math.pi, sample_n)
    # x and y must have same first dimension
    ordinates = np.zeros(sample_n)

    fourier_cn = 500

    for sample_ind in range(0, sample_n):

        sum = 0.0

        for ind in range(1, fourier_cn):
            sum += (-1.0)**(ind - 1.0) * 2.0 / ind * \
                math.sin(ind * abscissas[sample_ind])

        ordinates[sample_ind] = sum

    plt.plot(abscissas, ordinates)
    plt.grid()
    plt.draw()


def build_and_plot_sawtooth():

    sample_n = 50
    abscissas = np.linspace(-2.0 * math.pi, 2.0 * math.pi, sample_n)
    ordinates = np.zeros(sample_n)

    for sample_ind in range(0, sample_n):

        sum = 0.0
        fourier_cn = 300
        for ind in range(1, fourier_cn):
            sum += (-1.0)**(ind + 1.0) * 2.0 / ind * \
                math.sin(ind * abscissas[sample_ind])

        ordinates[sample_ind] = sum

    plt.plot(abscissas, ordinates)
    plt.grid()
    plt.draw()


def build_and_plot_thriangle():

    sample_n = 500
    absciss = np.linspace(-2.0 * math.pi, 2.0 * math.pi, sample_n)

    fourier_cn = 500
    ordinat = np.linspace(0.0, math.pi, sample_n)

    for sample_ind in range(0, sample_n):

        sum = math.pi / 2.0

        for ind in range(1, fourier_cn):
            sum -= 4.0 / math.pi * \
                math.cos((2.0 * ind - 1.0) *
                         absciss[sample_ind]) / (2.0 * ind - 1.0)**2.0

        ordinat[sample_ind] = sum

    plt.plot(absciss, ordinat)
    plt.grid()
    plt.draw()

    # fig, ax = plt.subplots()
    # plt.subplots()


'''
    5.4.3. Пример разложения в комплексный ряд Фурье 

    ω₀ = 2π/Т - круговая частота. Сделаем период, равным 2π, тогда ω₀ = 1
'''


def build_and_plot_rectangle_wave():
    sample_n = 100
    #    absciss = np.linspace(-2.0 * math.pi, 2.0 * math.pi, sample_n)
    absciss = np.linspace(0.0, 2.0 * math.pi, sample_n)
    ordinates = np.zeros(sample_n)

    for sample_ind in range(0, sample_n):

        sum = 0.0
        fourier_cn = 1000
        for n_ind in range(1, fourier_cn):
            odd_ind = 2 * n_ind - 1
            sum += 1.0 / odd_ind * math.sin(odd_ind * absciss[sample_ind])

        ordinates[sample_ind] = sum

    plot_a_graph(absciss, ordinates)

    #  Проинтегрируем синтезированную кривую на интервале [0, π]
    integral = definite_integral(absciss, ordinates)
    print("Интеграл синтезированной кривой на интервале [0, π] ", integral)

    a0 = integral / math.pi * 2.0
    print("Нулевой коэффициент ряда Фурье  ", a0)

    harmonics_quantity = 10

    #  Определение коэффициентов Фурье an, bn n=1, 2, ...
    (an, bn) = Fourier_coefficients(absciss, ordinates, harmonics_quantity)

    #  Построение функции, полученной в результате анализа Фурье
    new_ordinates = np.zeros(sample_n)

    for smpl_ind in range(1, sample_n):
        sum = 0.0
        for coeff_ind in range(1, sample_n):
            sum = sum + an[coeff_ind] * math.cos(coeff_ind * absciss[smpl_ind]) + \
                bn[coeff_ind] * math.sin(coeff_ind * absciss[smpl_ind])
        print("Сумма без нулевого коэффициента", sum)

        new_ordinates[smpl_ind] = a0 + sum

    plot_a_graph(absciss, new_ordinates)


def build_and_plot_complex():
    T = 2.0 * math.pi
    # Круговая частота равна единице, так как принимаем период, равным 2π
    omega0 = 2.0 * math.pi / T
    C0 = 2.0 / T  # Нулевой коэффициент Фурье

    sample_n = 500
    absciss = np.linspace(-3.0 * T, 5.0 * T, sample_n)
    ordinat = np.linspace(0.0, 1.0, sample_n)

    fourier_cn = 500

    for sample_ind in range(0, sample_n):

        sum = C0

        for ind in range(1, fourier_cn):
            if (-1 + T * (ind - 1) <= absciss[sample_ind] and absciss[sample_ind] <= 1.0 + T * (ind - 1)):
                sum += 2.0 / T / omega0 / ind * math.sin(omega0 * ind) * 1.0

        ordinat[sample_ind] = sum

    plt.plot(absciss, ordinat)
    plt.grid()
    plt.draw()

    fig, ax = plt.subplots()

    spectr = np.linspace(0.0, 1.0, sample_n)

    for ind in range(1, sample_n):
        if (-1.0 <= absciss[ind] and absciss[ind] <= 1.0):
            spectr[ind] = 2.0 / T / omega0 / ind * math.sin(omega0 * ind) * 1.0
        else:
            spectr[ind] = 0.0

    ax.plot(absciss, spectr)
    plt.grid()
    plt.draw()


def build_and_plot_complex_sample_func():
    T = 2.0 * math.pi
    # Круговая частота равна единице, так как принимаем период, равным 2π
    omega0 = 2.0 * math.pi / T
    C0 = 2.0 / T  # Нулевой коэффициент Фурье

    sample_n = 1000
    absciss = np.linspace(-5.0 * T, 5.0 * T, sample_n)

    puls_n = 8  # Количество прямоугольных импульсов
    puls_wide = 10.0 * T / puls_n / 1.5  # Ширина импульса

    ordinat = np.zeros(sample_n)

    fourier_cn = 1000

    for puls_ind in range(0, puls_n):
        left = -5.0 * T + 1.5 * puls_wide * (puls_ind - 1)

        for absc_ind in range(0, sample_n):
            if (left <= absciss[absc_ind] and absciss[absc_ind] < left + puls_wide):

                sum = C0

                for ind in range(1, fourier_cn):

                    sum += 2.0 / T \
                        * math.sin(absciss[ind]) / absciss[ind] \
                        * math.sin(omega0 * ind) / omega0 / ind

                ordinat[absc_ind] = sum

    plt.plot(absciss, ordinat)
    plt.grid()
    plt.draw()

    fig, ax = plt.subplots()

    spectr = np.zeros(sample_n)

    for ind in range(1, sample_n):
        spectr[ind] = 2.0 / T * math.sin(absciss[ind]) / absciss[ind]

    ax.plot(absciss, spectr)
    plt.grid()
    plt.draw()

    fig, ax = plt.subplots()

    absciss = np.linspace(-5.0 * T, 5.0 * T, 50)
    spectr = np.zeros(50)

    for ind in range(1, 50):
        spectr[ind] = 2.0 / T * \
            np.abs(math.sin(absciss[ind])) / np.abs(absciss[ind])

    ax.stem(absciss, spectr)

    # Первый варинт амплитудного частотного спектра
    ax.set_xlabel('Частота в герцах [Гц]')
    ax.set_ylabel('Величина частотной области (или спектра)')
    # ax.plot(absciss, spectr)
    plt.grid()
    plt.draw()


def plot_3sinx_4sin2x_2sin3x():
    print("******************** def plot_3sinx_4sin2x_2sin3x():")
    sample_n = 100
    absciss = np.linspace(0.0, math.pi / 2.0, sample_n)

    ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = math.sin(absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 3.0*math.sin(absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 4.0*math.sin(2.0*absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 2.0*math.sin(3.0*absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 3.0*math.sin(absciss[smpl_ind]) \
            + 4.0*math.sin(2.0*absciss[smpl_ind]) \
            + 2.0*math.sin(3.0*absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    '''
        ----------------------------------------------------------------------------------
        Приступим к анализу Фурье
        ----------------------------------------------------------------------------------
    '''
    harmonics_quantity = 10     # Не должна быть больше половины частоты дискретизации сигнала
    # Теорема Найквиста, Шеннона, Котельникова
    # В нашем случае harmonics_quantity < 2 * sample_n

    #  Проинтегрируем синтезированную кривую на интервале [0, π / 2]
    integral = definite_integral(absciss, ordinates)

    a0 = integral / math.pi / 2.0

    #  Определение коэффициентов Фурье an, bn n=1, 2, ...
    (an, bn) = Fourier_coefficients(absciss, ordinates, harmonics_quantity)

    #  Построение функции, полученной в результате анализа Фурье
    new_ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        sum = 0.0
        # Внимательно нужно следить за индексами
        for coeff_ind in range(1, harmonics_quantity):
            sum = sum + an[coeff_ind] * math.cos(coeff_ind * absciss[smpl_ind]) + \
                bn[coeff_ind] * math.sin(coeff_ind * absciss[smpl_ind])

        new_ordinates[smpl_ind] = a0 + sum

    plot_a_graph(absciss, new_ordinates)

    '''
        -----------------------------------------------------------------------------------------
        Теперь попробую штурмовать спектр частот

        Исследуя спектр, необходимо рассматривать
        'не коэффициенты' каждой частотной составляющей,
        а её 'амплитуду'.

        r = √ (a² + b²)     n = 1, 2, 3, ...
    '''
    freq_axis_absciss = np.linspace(
        0, harmonics_quantity, harmonics_quantity, endpoint=False)

    ampl_spectum1 = np.zeros(harmonics_quantity)
    ampl_spectum2 = np.zeros(harmonics_quantity)
    phase_spectum1 = np.zeros(harmonics_quantity)

    for amp_sp_ind in range(0, harmonics_quantity):
        idx = int(freq_axis_absciss[amp_sp_ind])
        # Это, видимо-таки, и есть правильный спектр
        ampl_spectum1[amp_sp_ind] = math.sqrt(an[idx]**2 + bn[idx]**2)
        # Это, видимо-таки, НЕправильный спектр, мною придуманный :)
        ampl_spectum2[amp_sp_ind] = an[idx] * math.cos(an[idx] * absciss[idx]) + \
            bn[idx] * math.cos(an[idx] * absciss[idx])

        # Фазовый спектр - совсем для меня ночь
        phase_spectum1[amp_sp_ind] = math.atan2(bn[idx], an[idx])

    fig, ax = plt.subplots()

    # Stem-график.
    # Если нужно отобразить данные в виде отдельных точек,
    # соединенных линией с некоторым базовым уровнем,
    # можно воспользоваться функцией stem():
    ax.stem(freq_axis_absciss, ampl_spectum1)

    # Первый варинт амплитудного частотного спектра
    ax.set_xlabel('Частота в герцах [Гц]')
    ax.set_ylabel('Величина частотной области (или спектра)')
    # ax.set_xlim(0, harmonics_quantity)
    # ax.set_ylim(0, harmonics_quantity)
    plt.grid()
    plt.show()

    # Второй варинт амплитудного частотного спектра
    fig, ax = plt.subplots()
    ax.stem(freq_axis_absciss, np.abs(ampl_spectum2))

    ax.set_xlabel('Частота в герцах [Гц]')
    ax.set_ylabel('Величина частотной области (или спектра)')
    # ax.set_xlim(0, harmonics_quantity)
    # ax.set_ylim(0, harmonics_quantity)
    plt.grid()
    plt.show()

    '''
        Спектры получились похожими, интересено, какой из них более правильный...

        Наверное, все-таки первый
    '''
    '''
        ----------------------------------------------------------------------------------
        А что это за фазовый спектр
    '''
    fig, ax = plt.subplots()
    ax.stem(freq_axis_absciss, phase_spectum1)

    ax.set_xlabel('Частота в герцах [Гц]')
    ax.set_ylabel('Частотная область (или фазовый спектр)')
    # ax.set_xlim(0, harmonics_quantity)
    # ax.set_ylim(0, harmonics_quantity)
    plt.grid()
    plt.show()


def plot_3sinx_4sin2x_2sin3x_pi():
    print("******************** def plot_3sinx_4sin2x_2sin3x():")
    sample_n = 100
    absciss = np.linspace(0.0, math.pi, sample_n)

    ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = math.sin(absciss[smpl_ind])

    # plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 3.0*math.sin(absciss[smpl_ind])

    # plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 4.0*math.sin(2.0*absciss[smpl_ind])

    # plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 2.0*math.sin(3.0*absciss[smpl_ind])

    # plot_a_graph(absciss, ordinates)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = 3.0*math.sin(absciss[smpl_ind]) \
            + 4.0*math.sin(2.0*absciss[smpl_ind]) \
            + 2.0*math.sin(3.0*absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)

    '''
        ----------------------------------------------------------------------------------
        Приступим к анализу Фурье
        ----------------------------------------------------------------------------------
    '''
    harmonics_quantity = 20

    #  Проинтегрируем синтезированную кривую на интервале [0, π / 2]
    integral = definite_integral(absciss, ordinates)

    a0 = integral / math.pi / 2.0

    #  Определение коэффициентов Фурье an, bn n=1, 2, ...
    (an, bn) = Fourier_coefficients(absciss, ordinates, harmonics_quantity)

    #  Построение функции, полученной в результате анализа Фурье
    new_ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        sum = 0.0
        # Внимательно нужно следить за индексами
        for coeff_ind in range(1, harmonics_quantity):
            sum = sum + an[coeff_ind] * math.cos(coeff_ind * absciss[smpl_ind]) + \
                bn[coeff_ind] * math.sin(coeff_ind * absciss[smpl_ind])

        new_ordinates[smpl_ind] = a0 + sum

    plot_a_graph(absciss, new_ordinates)

    '''
        -----------------------------------------------------------------------------------------
        Теперь попробую штурмовать спектр частот

        Исследуя спектр, необходимо рассматривать
        'не коэффициенты' каждой частотной составляющей,
        а её 'амплитуду'.

        r = √ (a² + b²)     n = 1, 2, 3, ...
    '''
    freq_axis_absciss = np.linspace(
        0, harmonics_quantity, harmonics_quantity, endpoint=False)

    ampl_spectum = np.zeros(harmonics_quantity)

    for amp_sp_ind in range(0, harmonics_quantity):
        idx = int(freq_axis_absciss[amp_sp_ind])
        # Это, видимо-таки, и есть правильный спектр
        ampl_spectum[amp_sp_ind] = math.sqrt(an[idx]**2 + bn[idx]**2)

    fig, ax = plt.subplots()

    # Stem-график.
    # Если нужно отобразить данные в виде отдельных точек,
    # соединенных линией с некоторым базовым уровнем,
    # можно воспользоваться функцией stem():
    ax.stem(freq_axis_absciss, ampl_spectum)

    # Первый варинт амплитудного частотного спектра
    ax.set_xlabel('Частота в герцах [Гц]')
    ax.set_ylabel('Величина частотной области (или спектра)')
    # ax.set_xlim(0, harmonics_quantity)
    # ax.set_ylim(0, harmonics_quantity)
    plt.grid()
    plt.show()


'''
    ***************************************************************************************************
    def main():
    ***************************************************************************************************

    Тестовая часть для проверки всего выше разработанного кода
'''

'''
    *************************************************************************************************************
    if __name__ == "__main__":
    *************************************************************************************************************
    Когда интерпретатор Python читает исходный файл, он исполняет весь найденный в нем код.

    Перед тем, как начать выполнять команды,
    он определяет несколько специальных переменных.

    Например, если интерпретатор запускает некоторый модуль (исходный файл) как основную программу,
    он присваивает специальной переменной '__name__' значение "__main__".

    Если этот файл импортируется из другого модуля,
    переменной '__name__' будет присвоено "имя этого модуля, ИМПОРТИРУЮЩЕГО ДАННЫЙ".

    Предположим, что код исполняется как основная функция,
    например:

        python file_with_python_code.py

    После задания специальный переменных,
    интерпретатор выполнит инструкцию 'import'
    и загрузит указанные модули.

    Затем он прочтет инструкцию if,
    «поймёт», что '__name__' эквивалентен "__main__",
    и выполнит указанный блок.

    Одна из причин делать именно так – тот факт,
    что иногда вы пишете модуль (файл с расширением .py),
    предназначенный для непосредственного исполнения.

    Кроме того, он также может быть импортирован и использован из другого модуля.

    Производя подобную проверку, вы можете сделать так,
    что код будет исполняться только при условии,
    что данный модуль запущен как программа,
    и запретить исполнять его,
    если его хотят импортировать и использовать функции модуля отдельно.
'''

if __name__ == "__main__":
    main()
