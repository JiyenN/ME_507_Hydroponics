/* USER CODE BEGIN Header */
// comment for gh
/**
  ******************************************************************************
  * @file           : main.cpp
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
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
using std::string;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//Interfacing ESP8266 Wi-Fi with STM32F401CCUx
//NOTE: Serial is serial monitor with baud rate(9600)
//NOTE: Serial2 (TX2, RX2)is connected with ESP8266(RX,TX)respectively with baud rate (9600)
string webpage="";                                   //String variable to store characters
int i=0,k=0,x=0;                                         //integer variables
string readString;                                   //using readString feature to read characters

bool No_IP=false;                                 //bool variables
string IP="";                                         //String variable to store data
char temp1='0';                                      //character variable

string name="<p>Manodeep's Server</p><p>Server created using ESP01 and STM32F401CCUx</p>";   //String with html notations
string data="<p>Data Received Successfully.....</p>";     //String with html

/* Static message buffers placed here to avoid string literals inline */
static const char BUF_AT_CIPSEND[] = "AT+CIPSEND=0,";
static const char BUF_AT_CIPCLOSE[] = "AT+CIPCLOSE=0";
static const char BUF_AT[] = "AT";
static const char BUF_CWMODE[] = "AT+CWMODE=3";
static const char BUF_CWQAP[] = "AT+CWQAP";
static const char BUF_RST[] = "AT+RST";
static const char BUF_CWJAP[] = "AT+CWJAP=\"Jiyen's iPhone\",\"loquat1515\"";
static const char BUF_CIPMUX[] = "AT+CIPMUX=1";
static const char BUF_CIPSERVER[] = "AT+CIPSERVER=1,80";
static const char BUF_CIFSR[] = "AT+CIFSR";

static const char BUF_IP_ADDRESS[] = "IP Address:";
static const char BUF_PORT[] = "Port:";
static const char BUF_CONNECTING_WIFI[] = "Connecting Wifi....";
static const char BUF_WIFI_CONNECTED[] = "Wifi Connected";
static const char BUF_SYSTEM_READY[] = "System Ready..";
static const char BUF_REFRESH[] = "Please Refresh your Page";
static const char BUF_START_PRINTING[] = "Start Printing";
static const char BUF_DONE_PRINTING[] = "Done Printing";
static const char BUF_OK[] = "OK";
static const char BUF_ERROR[] = "Error";

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Simple software RX buffers for polling reads (used to emulate Serial.available/find/read) */
#define SW_RX_BUF_SIZE 512
static uint8_t sw_rx2[SW_RX_BUF_SIZE];
static uint16_t sw_rx2_head = 0;
static uint16_t sw_rx2_tail = 0;

static uint8_t sw_rx1[SW_RX_BUF_SIZE/4];
static uint16_t sw_rx1_head = 0;
static uint16_t sw_rx1_tail = 0;

static inline uint16_t sw_count(uint16_t head, uint16_t tail, uint16_t size) {
  return (head + size - tail) % size;
}

static void sw_push(uint8_t *buf, uint16_t &head, uint16_t &tail, uint16_t size, uint8_t b) {
  buf[head] = b;
  head = (head + 1) % size;
  if (head == tail) { // overflow, advance tail
    tail = (tail + 1) % size;
  }
}

static int sw_pop(uint8_t *buf, uint16_t &head, uint16_t &tail, uint16_t size) {
  if (head == tail) return -1;
  uint8_t v = buf[tail];
  tail = (tail + 1) % size;
  return v;
}

// fill buffer from UART (non-blocking-ish): repeatedly try to receive bytes with 1ms timeout
static void sw_fill_from_uart(UART_HandleTypeDef *huart) {
  uint8_t b;
  HAL_StatusTypeDef st;
  // try to pull as many bytes as available (with tiny timeout)
  do {
    st = HAL_UART_Receive(huart, &b, 1, 1);
    if (st == HAL_OK) {
      if (huart == &huart2) sw_push(sw_rx2, sw_rx2_head, sw_rx2_tail, SW_RX_BUF_SIZE, b);
      else sw_push(sw_rx1, sw_rx1_head, sw_rx1_tail, SW_RX_BUF_SIZE/4, b);
    }
  } while (st == HAL_OK);
}

// returns number of bytes available in buffer (will attempt to read from UART first)
static int serial_available_uart(UART_HandleTypeDef *huart) {
  sw_fill_from_uart(huart);
  if (huart == &huart2) return sw_count(sw_rx2_head, sw_rx2_tail, SW_RX_BUF_SIZE);
  return sw_count(sw_rx1_head, sw_rx1_tail, SW_RX_BUF_SIZE/4);
}

// read one byte from buffer, returns -1 if none
static int serial_read_byte_uart(UART_HandleTypeDef *huart) {
  if (huart == &huart2) return sw_pop(sw_rx2, sw_rx2_head, sw_rx2_tail, SW_RX_BUF_SIZE);
  return sw_pop(sw_rx1, sw_rx1_head, sw_rx1_tail, SW_RX_BUF_SIZE/4);
}

// find pattern in buffer, reading from UART until timeout_ms expires. If found, advance tail to just after pattern and return 1.
static int serial_find_uart(UART_HandleTypeDef *huart, const char *pat, uint32_t timeout_ms) {
  uint32_t start = HAL_GetTick();
  size_t patlen = strlen(pat);
  if (patlen == 0) return 0;
  while (HAL_GetTick() - start <= timeout_ms) {
    sw_fill_from_uart(huart);
    // search buffer
    uint16_t head = (huart == &huart2) ? sw_rx2_head : sw_rx1_head;
    uint16_t tail = (huart == &huart2) ? sw_rx2_tail : sw_rx1_tail;
    uint16_t size = (huart == &huart2) ? SW_RX_BUF_SIZE : SW_RX_BUF_SIZE/4;
    uint16_t count = sw_count(head, tail, size);
    if (count >= (int)patlen) {
      // scan possible start positions
      for (uint16_t i = 0; i <= count - patlen; ++i) {
        bool ok = true;
        for (size_t j = 0; j < patlen; ++j) {
          uint16_t idx = (tail + i + j) % size;
          uint8_t c = (huart == &huart2) ? sw_rx2[idx] : sw_rx1[idx];
          if (c != (uint8_t)pat[j]) { ok = false; break; }
        }
        if (ok) {
          // advance tail to after pattern
          uint16_t newtail = (tail + i + patlen) % size;
          if (huart == &huart2) sw_rx2_tail = newtail; else sw_rx1_tail = newtail;
          return 1;
        }
      }
    }
    // small delay to wait for more bytes
    HAL_Delay(1);
  }
  return 0;
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* small helper wrappers for HAL UART transmit */
static void send_uart1(const char *msg)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, (uint16_t)strlen(msg), HAL_MAX_DELAY);
}
static void send_uart1_ln(const char *msg)
{
  send_uart1(msg);
  send_uart1("\r\n");
}
static void send_uart1_str(const string &s)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)s.c_str(), (uint16_t)s.length(), HAL_MAX_DELAY);
  send_uart1("\r\n");
}
static void send_uart2(const char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), HAL_MAX_DELAY);
}
static void send_uart2_ln(const char *msg)
{
  send_uart2(msg);
  send_uart2("\r\n");
}
static void send_uart2_str(const string &s)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)s.c_str(), (uint16_t)s.length(), HAL_MAX_DELAY);
  send_uart2("\r\n");
}
void sendwebdata(string webPage)                          //This function is used to send webpage datas to the localserver
{
    int ii=0;
     while(1)
     {
      unsigned int l=webPage.length();
      /* send AT+CIPSEND=0,<len> to both serial and esp */
      send_uart1(BUF_AT_CIPSEND);
      send_uart2(BUF_AT_CIPSEND);
      char numbuf[16];
      snprintf(numbuf, sizeof(numbuf), "%u", l+2);
      send_uart1_ln(numbuf);
      send_uart2_ln(numbuf);
      HAL_Delay(100);
      /* send webpage to monitor and to ESP (with CRLF) */
      send_uart1_str(webPage);
      send_uart2_str(webPage);
      if (serial_find_uart(&huart2, "OK", 200))
      {
        ii = 11;
      }
      if(ii==11)
      break;
      HAL_Delay(100);
     }
}
void Send()                                        //This function contains data to be sent to local server
{
      webpage = "<h1>Welcome to Manodeep's server</h1><body bgcolor=f0f0f0>";
      sendwebdata(webpage);
      webpage=name;
      sendwebdata(webpage);
      HAL_Delay(1000);
      webpage = "<a href=\"https://www.instructables.com/member/Manodeep/instructables/\"";
      webpage+="\">Click Here to see Manodeep's projects on Instructables</a>";
      webpage+=data;
      sendwebdata(webpage);
      send_uart2_ln(BUF_AT_CIPCLOSE);                  //Closes the server connection
}
void check4IP(int t1)                                     //A function to check ip of ESP8266
{
  uint32_t t2 = HAL_GetTick();
  while (t2 + (uint32_t)t1 > HAL_GetTick())
  {
    if (serial_find_uart(&huart2, "WIFI GOT IP", 10)) {
      No_IP = true;
      break;
    }
  }
}

void get_ip()                                           //After cheacking ip ,this is a function to get IP address
{
  IP="";
  char ch=0;
  while(1)
  {
    send_uart2_ln(BUF_CIFSR);                   //GET IP AT COMMAND
    if (serial_find_uart(&huart2, "STAIP,", 1000))                   //This finds the STAIP that is the STATIC IP ADDRESS of ESP8266
    {
      HAL_Delay(1000);
      /* print IPAddress:IP Port:80 on uart1 without extra newlines */
      send_uart1(BUF_IP_ADDRESS);
      int r;
      while ((r = serial_read_byte_uart(&huart2)) >= 0) {
        ch = (char)r;                      // read from sw buffer
        if (ch == '+') break;
        IP += ch;
      }
    }
    if(ch=='+')
    break;
    HAL_Delay(1000);
  }
  /* transmit IP and port on uart1 */
  if(!IP.empty())
    HAL_UART_Transmit(&huart1, (uint8_t*)IP.c_str(), (uint16_t)IP.length(), HAL_MAX_DELAY);
  send_uart1(BUF_PORT);
  char portbuf[8];
  snprintf(portbuf, sizeof(portbuf), "%d", 80);
  send_uart1_ln(portbuf);
}

void connect_wifi(string cmd, int t)                  //This function is for connecting ESP8266 with wifi network by using AT commands
{
  int temp=0,i=0;
  while(1)
  {
    /* send command to both uarts */
    send_uart1_str(cmd);
    send_uart2_str(cmd);
    if (serial_find_uart(&huart2, "OK", t))
      i = 8;
    HAL_Delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  send_uart1_ln(BUF_OK);
  else
  send_uart1_ln(BUF_ERROR);
}

void wifi_init()                                //This function contains AT commands that passes to connect_wifi()
{
      connect_wifi(BUF_AT,100);                   //Sends AT command with time(Command for Achknowledgement)
      connect_wifi(BUF_CWMODE,100);          //Sends AT command with time (For setting mode of Wifi)
      connect_wifi(BUF_CWQAP,100);            //Sends AT command with time (for Quit AP)
      connect_wifi(BUF_RST,5000);             //Sends AT command with time (For RESETTING WIFI)
      check4IP(5000);
      if(!No_IP)
      {
        send_uart1_ln(BUF_CONNECTING_WIFI);
        connect_wifi(BUF_CWJAP,7000);         //provide your WiFi username and password here
      }
      send_uart1_ln(BUF_WIFI_CONNECTED);
      get_ip();

      connect_wifi(BUF_CIPMUX,100);                          //Sends AT command with time (For creating multiple connections)
      connect_wifi(BUF_CIPSERVER,100);                    //Sends AT command with time (For setting up server with port 80)
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* moved Serial/UART initialization and wifi_init after HAL and MX_UART init so HAL UART APIs are safe to call */
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* initialize any Serial wrappers (if present) and start wifi */
  /* HAL UARTs are initialized by MX_USARTx_UART_Init(); Serial wrappers (if any) are not required.
     Removed Serial.begin/Serial2.begin to use HAL_UART_Transmit directly. */
  wifi_init();
  /* indicate ready */
  send_uart1_ln(BUF_SYSTEM_READY);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  k=0;
    send_uart1_ln(BUF_REFRESH);
	  while(k<1000)
	  {
	    k++;
       if (serial_find_uart(&huart2, "0,CONNECT", 50))
       {
            send_uart1_ln(BUF_START_PRINTING);
          Send();
            send_uart1_ln(BUF_DONE_PRINTING);
          HAL_Delay(1000);
       }
	  HAL_Delay(1);
	 }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
