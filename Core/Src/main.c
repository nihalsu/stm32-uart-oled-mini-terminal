/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define MAX_ROWS 20
#define MAX_COLS 18       // 128 piksel genişlik / 7 piksel font ~= 18 karakter
#define VISIBLE_ROWS 5

volatile uint8_t update_needed = 0;
char terminal_history[MAX_ROWS][MAX_COLS + 1];
int current_row = 0;
int current_col = 0;
int scroll_offset = 0;
uint8_t rx_byte;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// ANSI Kaçış Dizileri
const char* CLEAR_TERMINAL = "\033[2J\033[H";

void Refresh_All_Screens(void) {
    // 1. TERATERM GÜNCELLEME
    // Terminali temizle ve o anki görünür pencereyi (scroll_offset'ten itibaren) yazdır
    HAL_UART_Transmit(&huart2, (uint8_t*)CLEAR_TERMINAL, 7, 10);
    for (int i = 0; i < VISIBLE_ROWS; i++) {
        int line_idx = scroll_offset + i;
        if (line_idx < MAX_ROWS && terminal_history[line_idx][0] != '\0') {
            HAL_UART_Transmit(&huart2, (uint8_t*)terminal_history[line_idx], strlen(terminal_history[line_idx]), 10);
            HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 10);
        }
    }

    // 2. OLED GÜNCELLEME
    ssd1306_Fill(Black);
    for (int i = 0; i < VISIBLE_ROWS; i++) {
        int line_idx = scroll_offset + i;
        if (line_idx < MAX_ROWS) {
            ssd1306_SetCursor(0, i * 11);
            ssd1306_WriteString(terminal_history[line_idx], Font_7x10, White);
        }
    }
    ssd1306_UpdateScreen();
}

void Move_Last_Word_To_Next_Line(void) {
    if (current_row >= MAX_ROWS - 1) return;

    int i = current_col - 1;
    // Kelimenin başlangıcını bul (boşluğa kadar geri git)
    while (i >= 0 && terminal_history[current_row][i] != ' ') {
        i--;
    }
    i++; // Kelimenin ilk harfinin indeksi

    // Eğer satırın tamamı tek bir kelimeyse (hiç boşluk yoksa) bölmekten başka care yok
    if (i == 0) return;


    char temp_word[MAX_COLS];
    strcpy(temp_word, &terminal_history[current_row][i]);
    terminal_history[current_row][i] = '\0';


    current_row++;
    strcpy(terminal_history[current_row], temp_word);
    current_col = strlen(temp_word);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  memset(terminal_history, 0, sizeof(terminal_history));
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();

  HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (update_needed) {
	          update_needed = 0;
	          Refresh_All_Screens();
	      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        uint8_t is_processed = 1;

        // --- 1. SİLME İŞLEMİ (Backspace) ---
        if (rx_byte == 8 || rx_byte == 127) {
            if (current_col > 0) {
                current_col--;
                terminal_history[current_row][current_col] = '\0';
            }
            else if (current_row > 0) {
                current_row--;
                current_col = strlen(terminal_history[current_row]);
                if(current_col > 0) {
                    current_col--;
                    terminal_history[current_row][current_col] = '\0';
                }
            }
        }
        // --- 2. YENİ SATIR (Enter) ---
        else if (rx_byte == '\r') {
            if (current_row < MAX_ROWS - 1) {
                current_row++;
                current_col = 0;
            }
        }
        // --- 3. YUKARI KAYDIRMA (Komut: '+') ---
        else if (rx_byte == '+') {
            if (scroll_offset > 0) {
                scroll_offset--;
            }
        }
        // --- 4. AŞAĞI KAYDIRMA (Komut: '-') ---
        else if (rx_byte == '-') {
            if (scroll_offset < (MAX_ROWS - VISIBLE_ROWS) && (scroll_offset < current_row)) {
                scroll_offset++;
            }
        }
        // --- 5. METİN YAZMA (Word Wrap - Kelime Kaydırma) ---
        else if (rx_byte >= 32 && rx_byte <= 126) {
            // Eğer satır dolduysa
            if (current_col >= MAX_COLS) {
                if (rx_byte == ' ') {
                    // Boşluk geldiyse sadece alt satıra geç
                    if (current_row < MAX_ROWS - 1) {
                        current_row++;
                        current_col = 0;
                    }
                } else {
                    // Harf geldiyse kelimeyi aşağı kaydır
                    Move_Last_Word_To_Next_Line();

                    // Kaydırmadan sonra yeni karakteri güvenle ekle
                    if (current_col < MAX_COLS) {
                        terminal_history[current_row][current_col++] = (char)rx_byte;
                        terminal_history[current_row][current_col] = '\0';
                    }
                }
            } else {
                // Satır henüz dolmadıysa normal yaz
                terminal_history[current_row][current_col++] = (char)rx_byte;
                terminal_history[current_row][current_col] = '\0';
            }
        }
        else {
            is_processed = 0;
        }

        // --- OTOMATİK SCROLL (Yazarken Takip Et) ---
        if (rx_byte != '+' && rx_byte != '-' && is_processed) {
            if (current_row >= (scroll_offset + VISIBLE_ROWS)) {
                scroll_offset = current_row - VISIBLE_ROWS + 1;
            }
        }

        if (is_processed) {
            update_needed = 1;
        }


        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
