///*
// * main_cpp.cpp
// *
// *  Created on: May 27, 2026
// *      Author: micha
// */
//#include "main_cpp.h"
//#include "main.h" // Gives access to HAL drivers & pin definitions
//#include <iostream>
//#include <string>
//
//using std::string;
//
//// You can now use classes, namespaces, and standard C++ here!
//string webpage="";                                   //string variable to store characters
//int i=0,k=0,x=0;                                         //integer variables
//string readstring;                                   //using readstring feature to read characters
//
//bool No_IP=false;                                 //bool variables
//string IP="";                                         //string variable to store data
//char temp1='0';         			                             //character variable
//
//string name="<p>Manodeep's Server</p><p>Server created using ESP01 and STM32F401CCUx</p>";   //string with html notations
//string data="<p>Data Received Successfully.....</p>";     //string with html
//
//
///* Private user code ---------------------------------------------------------*/
///* USER CODE BEGIN 0 */
//void sendwebdata(string webPage)                          //This function is used to send webpage datas to the localserver
//{
//    int ii=0;
//     while(1)
//     {
//      unsigned int l=webPage.length();
//      Serial.print("AT+CIPSEND=0,");
//      Serial2.print("AT+CIPSEND=0,");
//      Serial.println(l+2);
//      Serial2.println(l+2);
//      HAL_Delay(100);
//      Serial.println(webPage);                        //sends webpage data to serial monitor
//      Serial2.println(webPage);                       //sends webpage data to serial2 ESP8266
//      while(Serial2.available())
//      {
//
//        if(Serial2.find("OK"))
//        {
//          ii=11;
//          break;
//        }
//      }
//      if(ii==11)
//      break;
//      HAL_Delay(100);
//     }
//}
//
//
//void data(string webPage)                          //This function is used to send webpage datas to the localserver
//{
//    int ii=0;
//     while(1)
//     {
//      unsigned int l=webPage.length();
//      MX_USART1_UART_Init();
//      Serial.print("AT+CIPSEND=0,");
//      HAL_UART_Transmit(&huart1, )
//      Serial2.print("AT+CIPSEND=0,");
//      Serial.println(l+2);
//      Serial2.println(l+2);
//      HAL_Delay(100);
//      Serial.println(webPage);                        //sends webpage data to serial monitor
//      Serial2.println(webPage);                       //sends webpage data to serial2 ESP8266
//      while(Serial2.available())
//      {
//
//        if(Serial2.find("OK"))
//        {
//          ii=11;
//          break;
//        }
//      }
//      if(ii==11)
//      break;
//      HAL_Delay(100);
//     }
//}
//
//void Send()                                        //This function contains data to be sent to local server
//{
//      webpage = "<h1>Welcome to Manodeep's server</h1><body bgcolor=f0f0f0>";
//      sendwebdata(webpage);
//      webpage=name;
//      sendwebdata(webpage);
//      HAL_Delay(1000);
//      webpage = "<a href=\"https://www.instructables.com/member/Manodeep/instructables/\"";
//      webpage+="\">Click Here to see Manodeep's projects on Instructables</a>";
//      webpage+=data;
//      sendwebdata(webpage);
//      Serial2.println("AT+CIPCLOSE=0");                  //Closes the server connection
//}
//void check4IP(int t1)                                     //A function to check ip of ESP8266
//{
//  int t2=millis();
//  while(t2+t1>millis())
//  {
//    while(Serial2.available()>0)
//    {
//      if(Serial2.find("WIFI GOT IP"))
//      {
//        No_IP=true;
//      }
//    }
//  }
//}
//
//void get_ip()                                           //After cheacking ip ,this is a function to get IP address
//{
//  IP="";
//  char ch=0;
//  while(1)
//  {
//    Serial2.println("AT+CIFSR");                   //GET IP AT COMMAND
//    while(Serial2.available()>0)
//    {
//      if(Serial2.find("STAIP,"))                   //This finds the STAIP that is the STATIC IP ADDRESS of ESP8266
//      {
//        HAL_Delay(1000);
//        Serial.print("IP Address:");
//        while(Serial2.available()>0)
//        {
//          ch=Serial2.read();                      //Serial2 reads from ESP8266
//          if(ch=='+')
//          break;
//          IP+=ch;
//        }
//      }
//      if(ch=='+')
//      break;
//    }
//    if(ch=='+')
//    break;
//    HAL_Delay(1000);
//  }
//  Serial.print(IP);                                //prints IP address in Serial monitor
//  Serial.print("Port:");
//  Serial.println(80);
//}
//
//void connect_wifi(string cmd, int t)                  //This function is for connecting ESP8266 with wifi network by using AT commands
//{
//  int temp=0,i=0;
//  while(1)
//  {
//    Serial.println(cmd);                  //Sends to serial monitor
//    Serial2.println(cmd);                 //sends to ESP8266 via serial communication
//    while(Serial2.available())
//    {
//      if(Serial2.find("OK"))
//      i=8;
//    }
//    HAL_Delay(t);
//    if(i>5)
//    break;
//    i++;
//  }
//  if(i==8)
//  Serial.println("OK");
//  else
//  Serial.println("Error");
//}
//
//void wifi_init()                                //This function contains AT commands that passes to connect_wifi()
//{
//      connect_wifi("AT",100);                   //Sends AT command with time(Command for Achknowledgement)
//      connect_wifi("AT+CWMODE=3",100);          //Sends AT command with time (For setting mode of Wifi)
//      connect_wifi("AT+CWQAP",100);            //Sends AT command with time (for Quit AP)
//      connect_wifi("AT+RST",5000);             //Sends AT command with time (For RESETTING WIFI)
//      check4IP(5000);
//      if(!No_IP)
//
//        Serial.println("Connecting Wifi....");
//        connect_wifi("AT+CWJAP=\"Jiyen's iPhone\",\"loquat1515\"",7000);         //provide your WiFi username and password here
//
//      }
//      else
//        {
//        }
//      Serial.println("Wifi Connected");
//      get_ip();
//
//      connect_wifi("AT+CIPMUX=1",100);                          //Sends AT command with time (For creating multiple connections)
//      connect_wifi("AT+CIPSERVER=1,80",100);                    //Sends AT command with time (For setting up server with port 80)
//}
//
//
//
//void main_cpp(void) {
//    while(1) {
//    	  /* USER CODE BEGIN 1 */
//    	   Serial.begin(9600);                //begins serial monitor with baud rate 9600
//    	   Serial2.begin(115200);               //begins serial communication with esp8266 with baud rate 9600 (Change according to your esp8266 module)
//    	   wifi_init();
//    	   Serial.println("System Ready..");
//    	  /* USER CODE END 1 */
//
//    	  /* MCU Configuration--------------------------------------------------------*/
//
//    	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//    	  HAL_Init();
//
//    	  /* USER CODE BEGIN Init */
//
//    	  /* USER CODE END Init */
//
//    	  /* Configure the system clock */
//    	  SystemClock_Config();
//
//    	  /* USER CODE BEGIN SysInit */
//
//    	  /* USER CODE END SysInit */
//
//    	  /* Initialize all configured peripherals */
//    	  MX_GPIO_Init();
//    	  MX_USART1_UART_Init();
//    	  MX_USART2_UART_Init();
//    	  /* USER CODE BEGIN 2 */
//
//    	  /* USER CODE END 2 */
//
//    	  /* Infinite loop */
//    	  /* USER CODE BEGIN WHILE */
//    	  while (1)
//    	  {
//    	    /* USER CODE END WHILE */
//    		  k=0;
//    		  Serial.println("Please Refresh your Page");
//    		  while(k<1000)
//    		  {
//    		    k++;
//    		   while(Serial2.available())
//    		   {
//    		    if(Serial2.find("0,CONNECT"))
//    		    {
//    		      Serial.println("Start Printing");
//    		      Send();
//    		      Serial.println("Done Printing");
//    		      HAL_Delay(1000);
//    		    }
//    		  }
//    		  HAL_Delay(1);
//    		 }
//    	    /* USER CODE BEGIN 3 */
//    	  }
//    	  /* USER CODE END 3 */
//    }
//}
