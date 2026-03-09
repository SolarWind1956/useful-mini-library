# -*- coding: utf-8 -*-
"""
Created on Fri Mar 14 14:32:40 2025

@author: SergejHeckelDesktop

Fourier_transforms.py
"""
import numpy as np
import math
import matplotlib.pyplot as plt


#  Фунция получения определенного интеграла как площади фигуры под кривой функции интегрирования


def plot_a_graph(abscissas, ordinates):
    plt.plot(abscissas, ordinates)
    plt.grid()
    plt.draw()


def definite_integral(abscissas, ordinates):
    integral = 0.0
    for idx in range(0, ordinates.size):
        integral += ordinates[idx] * abscissas[1]
    return integral


def plot_sin():
    sample_n = 100
    absciss = np.linspace(0.0, math.pi / 2.0, sample_n)

    ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = math.sin(absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)


def plot_cos():
    sample_n = 100
    absciss = np.linspace(0.0, math.pi / 2.0, sample_n)

    ordinates = np.zeros(sample_n)

    for smpl_ind in range(0, sample_n):
        ordinates[smpl_ind] = math.cos(absciss[smpl_ind])

    plot_a_graph(absciss, ordinates)


# from definite_integral import definite_integral


def Fourier_coefficients(absciss,
                         ordinates,
                         harmonics_quantity):

    sample_n = absciss.size

    #  Определение коэффициентов Фурье an n=1, 2, ...
    auxil_coeff = np.zeros(sample_n)

    # Коэффициенты косинусов
    an = np.zeros(harmonics_quantity)
    for an_ind in range(0, harmonics_quantity):
        # Удалим все ортогональные функции; останется только cos(x)**2
        for smpl_ind in range(0, sample_n):  # Внимательно нужно следить за индексами
            auxil_coeff[smpl_ind] = ordinates[smpl_ind] * \
                math.cos(an_ind * absciss[smpl_ind])

        integral = definite_integral(absciss, auxil_coeff)

        an[an_ind] = integral / math.pi

    # Коэффициенты синусов
    bn = np.zeros(harmonics_quantity)
    for bn_ind in range(0, harmonics_quantity):
        # Удалим все ортогональные функции; останется только sin(x)**2
        for smpl_ind in range(0, sample_n):  # Внимательно нужно следить за индексами
            auxil_coeff[smpl_ind] = ordinates[smpl_ind] * \
                math.sin(bn_ind * absciss[smpl_ind])

        integral = definite_integral(absciss, auxil_coeff)

        bn[bn_ind] = integral / math.pi

    return (an, bn)
