#include "hal.h"
#include <unistd.h>

/**
 * LED
 * SW
 * */
uint64_t leds_num [] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
uint64_t sw_num [] = {GPIO_PIN_4,GPIO_PIN_8,GPIO_PIN_10, GPIO_PIN_12};

/**
 * Time
 * */
const uint64_t BASIC_TIME = 500;
const uint64_t CUSTOM_TIME = 50;

//State SW
GPIO_PinState state_arr[4];

/*
 * Чтение состояния SW
 * */
void read_state(){
    for (size_t i = 0; i < 4;i++){
        state_arr[i] = HAL_GPIO_ReadPin(GPIOE,sw_num[i]);
    }
}

/*
 * Подсчет значения SW
 * */
uint64_t real_value_of_state(){
    return state_arr[0] + state_arr[1] * 2 + state_arr[2] * 4 + state_arr[3] * 8;
}

/*
 * Рассчет
 * */
uint64_t register_timer_params(uint64_t value){
    return BASIC_TIME + (value * CUSTOM_TIME);
}


/*
 * Анимация
 * */
void animate(size_t j){
        switch (j) {
            case 1:
                HAL_GPIO_WritePin(GPIOD, leds_num[3],GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, leds_num[4],GPIO_PIN_SET);
                break;
            case 2:
                HAL_GPIO_WritePin(GPIOD, leds_num[2],GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, leds_num[5],GPIO_PIN_SET);
                break;
            case 3:
                HAL_GPIO_WritePin(GPIOD, leds_num[1],GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, leds_num[6],GPIO_PIN_SET);
                break;
            case 4:
                HAL_GPIO_WritePin(GPIOD, leds_num[0],GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, leds_num[7],GPIO_PIN_SET);
                break;
            case 5:
                HAL_GPIO_WritePin(GPIOD, leds_num[0],GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, leds_num[7],GPIO_PIN_RESET);
                break;
            case 6:
                HAL_GPIO_WritePin(GPIOD, leds_num[1],GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, leds_num[6],GPIO_PIN_RESET);
                break;
            case 7:
                HAL_GPIO_WritePin(GPIOD, leds_num[2],GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, leds_num[5],GPIO_PIN_RESET);
                break;
            case 8:
                HAL_GPIO_WritePin(GPIOD, leds_num[3],GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, leds_num[4],GPIO_PIN_RESET);
                break;
            case 9:
                break;
        }
}

/*
 * Обработчик таймера
 * */
void tim6_handler(){
    static uint64_t j = 0;
    if (j >= 9){
        j = 0;
    }
    animate(j);
    ++j;
}

int umain() {
    registerTIM6_IRQHandler(tim6_handler);
    __enable_irq();

    WRITE_REG(TIM6_ARR,BASIC_TIME);
    WRITE_REG(TIM6_DIER,TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC,0);

    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);


    while (1){
        read_state();
        uint64_t value = real_value_of_state();
        WRITE_REG(TIM6_ARR,register_timer_params(value));
    }

    __disable_irq();
    return 0;
}





