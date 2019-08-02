#include <mbed.h>
#include "stm32f4xx_hal.h"

/* GENERATED WITH STM32CUBEMX */

#define PWM_IN_Pin GPIO_PIN_0 // PA_0 / MOT1A_ENC
#define PWM_IN_GPIO_Port GPIOA

TIM_HandleTypeDef htim2;

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA0-WKUP     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = PWM_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(PWM_IN_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 83;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlaveConfig.TriggerFilter = 0;

    if (HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/* GENERATED WITH STM32CUBEMX */

#define TIMx_RESOLUTION_CYCLES 2000 // InputClock = 84Mhz (APB1) 
                                    // Prescaler = 83
                                    // Period = 1999
#define TIMx_PERIOD_MS 2 // 500Hz
#define TEST_PWM_PERIOD_MS 1 // 1000Hz

enum DutyCycle : uint16_t
{
    DUTY_0_PERCENT = 0x0000,
    DUTY_33_PERCENT = 0x0001,
    DUTY_66_PERCENT = 0x0003,
    DUTY_100_PERCENT = 0x0007,
};

PwmOut test_pwm(SERVO1_PWM);
InterruptIn button1(BUTTON1);
InterruptIn button2(BUTTON2);
BusOut indicator(LED1,LED2,LED3);

volatile float test_duty = 0.0f;

int main()
{
    // initialize TIM2
    MX_TIM2_Init();

    // start input compare on both channels
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);

    // all leds off
    indicator = DUTY_0_PERCENT;

    // setup pwm with 1000Hz period, 0% duty at the beginning
    test_pwm.period_ms(1);
    test_pwm = 0.0f;

    // setup interrupts on buttons
    button1.mode(PullUp);
    button2.mode(PullUp);

    // increase duty cycle in test pwm at each press
    button1.fall([](void) -> void {
        if (test_duty < 1.0f)
            test_duty += 0.1f;
    });

    // decrease duty cycle in test pwm at each press
    button2.fall([](void) -> void {
        if (test_duty > 0.0f)
            test_duty -= 0.1f;
    });

    volatile uint32_t input_pwm_period_cycles;
    volatile uint32_t input_pwm_duty_cycles;
    volatile float input_pwm_period_ms;
    volatile float input_pwm_duty_percent; 
    printf("PWM input functionality test.\r\n"
           "TIM2 CH1 (PA_0) used as input.\r\n"
           "TIM9 CH1 (PE_9) used as pwm output (1kHz frequency).\r\n"
    );
    while(1)
    {
        // set new duty cycle for test pwm
        test_pwm = test_duty;

        // calculate input signal period and duty cycle
        input_pwm_period_cycles = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1) + 1;
	    input_pwm_duty_cycles = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2) + 1;
        input_pwm_period_ms = (((float)TIMx_PERIOD_MS * input_pwm_period_cycles) / (float)TIMx_RESOLUTION_CYCLES);
	    input_pwm_duty_percent = ((float)input_pwm_duty_cycles * 100) / (float)(input_pwm_period_cycles);

        // set indicator
        if(input_pwm_duty_percent >= 90.0f)
            indicator = DUTY_100_PERCENT;
        else if(input_pwm_duty_percent >= 66.0f )
            indicator = DUTY_66_PERCENT;
        else if(input_pwm_duty_percent >= 33.0f )
            indicator = DUTY_33_PERCENT;
        else 
            indicator = DUTY_0_PERCENT;

        // printf values to serial
        printf("Test PWM duty cycle: %.2f\r\n"
               "Measured input PWM period: %.2f ms\r\n"
               "Measured input PWM duty: %.2f %% \r\n",
               test_pwm.read(),
               input_pwm_period_ms,
               input_pwm_duty_percent
        );
        wait_ms(500);
    }
}