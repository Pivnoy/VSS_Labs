#include "hal.h"
#include <unistd.h>


/* WARNING
 * По условию лабораторной работы delay = 500 и должен простаивать только при переключении LED,
 * но программа содержит дополнительные простои для совместимости  с Virtual Lab
 * */
/*
 * LED
 * SW
 * nBTN
 * */
unsigned int leds_num [] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
unsigned int sw_num [] = {GPIO_PIN_4,GPIO_PIN_8,GPIO_PIN_10, GPIO_PIN_12};
unsigned int proc_num [] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};


// Базовое состоние SW
GPIO_PinState state_arr[4];


/*
 * Проверка состояния SW
 * */
bool check_sw() {
    for (size_t i = 0; i < 4;i++){
        state_arr[i] = HAL_GPIO_ReadPin(GPIOE,sw_num[i]);
    }
    if (state_arr[1] && state_arr[3] && (state_arr[0] == 0) && (state_arr[2] == 0)){
        return true;
    }
    return false;
}
/*
 * Реализация работы с LED и простоя в случае нажатия на nBTN
 * */
void pod_program() {
    int64_t delay = 500;
    int64_t point = 7;
    HAL_Delay(delay);
    for (size_t i = 0; i < 14; i++) {
        if (!check_sw()) {
            HAL_Delay(300);
            break;
        }
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOC, proc_num[2]);
        HAL_Delay(delay);
        while (state == GPIO_PIN_RESET){
            HAL_Delay(delay);
            HAL_GPIO_WritePin(GPIOD, proc_num[0],GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, proc_num[2],GPIO_PIN_SET);
            state = HAL_GPIO_ReadPin(GPIOC, proc_num[2]);
        }
        HAL_GPIO_WritePin(GPIOD, proc_num[0],GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, proc_num[2],GPIO_PIN_RESET);
        HAL_Delay(delay);
        switch (i) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
                HAL_Delay(delay);
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                HAL_GPIO_WritePin(GPIOD, leds_num[point--], GPIO_PIN_RESET);
                HAL_Delay(delay);
                break;
            default:
                break;
        }
    }

    for (size_t i = 0; i < 8;i++) {
        HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(GPIOD, proc_num[0], GPIO_PIN_RESET);
}

/*
 * Основная функция
 * Работа со стандартными SW и включение/выключение оранжевой лампочки
 * */

int umain() {
    GPIO_PinState state_arr[4];
    while (1) {
        for (size_t i = 0; i < 4;i++){
            state_arr[i] = HAL_GPIO_ReadPin(GPIOE,sw_num[i]);
        }
        if (state_arr[1] && state_arr[3] && (state_arr[0] == 0) && (state_arr[2] == 0)) {
            HAL_GPIO_WritePin(GPIOD, proc_num[1],GPIO_PIN_RESET);
            for (size_t i = 0; i < 4;i++) {
                HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
            }
            HAL_Delay(500);
            pod_program();
        } else {
            HAL_GPIO_WritePin(GPIOD, proc_num[1],GPIO_PIN_SET);
            for (size_t i = 0; i < 4;i++) {
                if (state_arr[i] == 1){
                    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
                } else {
                    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
                }
            }
            HAL_Delay(300);
        }
    }
    return 0;
}