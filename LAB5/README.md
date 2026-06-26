# LAB 5: FreeRTOS & Sensor Filtering Algorithm

**OBJECTIVES**
- Configure and implement FreeRTOS on RPi Pico.
- Utilize FreeRTOS functionalities such as Task Creation and Message Buffers.
- Implement a filtering algorithm for processing sensor data.


**EQUIPMENT** 
1.	A laptop that has the Pico C/C++ SDK installed
2.	Raspberry Pico W
3.	Micro-USB Cable

> [NOTE]
> Only students wearing fully covered shoes are allowed in the SR6A lab due to safety.

## **INTRODUCTION** 

Real-Time Operating Systems (RTOS), are operating systems designed to meet the requirements of real-time systems, which need to process inputs and provide outputs without buffering delays. FreeRTOS is a popular open-sourced RTOS that offers straightforward functionality to enable easy, robust, and optimizable design in embedded systems like the Raspberry Pi Pico. FreeRTOS offers many functionalities such as Task Management to manage the tasks and Message Buffers to help inter-task communications and data transfer.

In this lab, we'll focus on implementing FreeRTOS on Raspberry Pi Pico and explore its features by writing tasks that use Message Buffers. We’ll also cover how to process sensor data using a simple filtering algorithm.

## **SETTING UP FREERTOS ON RPi PICO** 

When integrating FreeRTOS with a project, such as in the case of developing applications for the Raspberry Pi Pico, it is essential to configure the build system correctly, to compile the application code together with the FreeRTOS kernel code. CMake is a widely used build system that can be configured to build your project and the FreeRTOS kernel together. This involves including the FreeRTOS source files and setting the necessary environment settings.

We will be using the [Ping example that uses FreeRTOS](https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/freertos/ping/picow_freertos_ping.c). However, we will need to take a few steps to enable it. Currently, you should **only be able to see** it in Explorer and **not be able to see** it in CMake. To set up FreeRTOS on Raspberry Pi Pico, download the [FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) and unzip it onto your computer. Do take note of where the folder is located.

> [NOTE]
> Please note that there seems to be an issue with this example in version Pico-v1.5.0. Therefore, we will be using using Pico-v1.5.1. Windows users can just reinstall the SDK and it will create a new folder or you may go a git pull.

<img src="/img/freertosfolder.png" width=100% height=100%>

Then add three items into the Cmake: Environment
- FREERTOS_KERNEL_PATH: C:\FreeRTOS-Kernel-main
- WIFI_SSID: "your mobile hotspot SSID" (SIT's WiFi would not work)
- WIFI_PASSWORD: "superduperpassword"

> [NOTE]: Please use your own hotspot for the WIFI_SSID and WIFI_PASSWORD, as SIT's WiFi security doesn't work well with Pico.


There are various methods to do it, but in this example, we will include the path into the CMake environment, not the Windows environment. The images below guide you on how you can include the three items.
<img src="/img/CMakeTools.png" width=100% height=100%>
<img src="/img/CmakeEnvironment.png" width=100% height=100%>

Next, include `pico_enable_stdio_usb(picow_freertos_ping_sys 1)` into the CMakeLists.txt file for "picow_freertos_ping_sys" to allow serial monitoring via USB.
Do remember to re-select the "Pico ARM GCC" compiler to kick-start the configuration process.

<img src="/img/NoSMP.PNG" width=100% height=100%>

Finally, before you can start compiling your code, make the following changes (lines #107 & #110) to the FreeRTOS configuration. The configuration file is located at "pico_w\wifi\freertos\ping\FreeRTOSConfig.h". This disables the SMP and ensures that there will only be a single core used in this example.

> [NOTE]
> Symmetric Multiprocessing (SMP) support in the FreeRTOS Kernel enables one instance of the FreeRTOS kernel to schedule tasks across multiple identical processor cores. The core architectures must be identical and share the same memory.

```
#if FREE_RTOS_KERNEL_SMP // set by the RP2040 SMP port of FreeRTOS
/* SMP port only */
#define configNUM_CORES                         1
#define configTICK_CORE                         0
#define configRUN_MULTIPLE_PRIORITIES           1
#define configUSE_CORE_AFFINITY                 0
#endif
```

At this point, if everything is done correctly, you should be able to see the project under CMake.
<img src="/img/CMakePing.png" width=100% height=100%>


You should try building the project and downloading the code to the RPi Pico. You should be able to see some output on the Serial Monitor.

<img src="/img/task1.png" width=100% height=100%>

## **Create FreeRTOS Tasks**
Currently, we are operating a single task that directs its focus on pinging a specified IP address. Nonetheless, given that we are utilizing a Real-Time Operating System (RTOS), our system is inherently designed to execute multiple tasks concurrently. Consequently, our subsequent task will logically involve constructing an additional task—a task designated to manage the blinking of an LED.

**Create a Blinking LED Task**

<img src="/img/task2.png" width=100% height=100%>

Include the following header files to allow printf and to use the GPIO for the LED.
```
#include <stdio.h>
#include "hardware/gpio.h"
```

The following is the function for blinking the LED. Notice that each task is a forever loop. In this example, the LED will turn on for 2000 ticks (approx 2 seconds) and turn off for 2000 ticks.
```
void led_task(__unused void *params) {
    while(true) {
        vTaskDelay(2000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(2000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }
}
```

Include the following in the `vLaunch` function. This will create the instance of the task for execution. TaskHandle is used for managing the task, i.e. deleting, changing priority, etc.
```
    TaskHandle_t ledtask;
    xTaskCreate(led_task, "TestLedThread", configMINIMAL_STACK_SIZE, NULL, 5, &ledtask);
```

Build and download the project to the RPi Pico. You should now see the ping message appearing on the serial monitor **AND** the LED blinking as intended.

**Create a Temperature Sensor Task**

The RP2040 microcontroller, embedded in the Raspberry Pi Pico, comes with an inbuilt temperature sensor, allowing developers to easily access and measure the ambient temperature. This sensor is part of the RP2040’s ADC (Analog to Digital Converter) system, which means it converts the analog temperature value to a digital representation that can be processed and read by the MCU. In this section, we’ll discuss how to access this built-in temperature sensor to retrieve temperature data. To access the temperature sensor data, we use the ADC to read the analog voltage and then convert this analog value to a temperature reading in degrees Celsius. Here is a simple step-by-step guide to achieving this.

<img src="/img/task3.png" width=100% height=100%>

Include the following header. This is to digitize the data from the inbuilt temperature sensor within RP2040.
```
#include "hardware/adc.h"
```

In addition, you will need to add in `hardware_adc` in CMakeLists.txt under as shown below:

```
    target_link_libraries(picow_freertos_ping_sys
            hardware_adc
            pico_cyw43_arch_lwip_sys_freertos
            pico_stdlib
            pico_lwip_iperf
            FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
            )
```


The following are the functions to obtain the temperature from the RP2040 and print it out via the serial. In the `temp_task` function, we initialize the ADC before reading the sensor data, it is essential to initialize the ADC subsystem. This prepares the ADC to start taking readings. The ADC system can read from multiple channels, so we need to select the temperature sensor as the input channel to the ADC (the temperature sensor is on input 4). Once the temperature sensor is selected, we can read the analog value from the ADC. The adc_read() function returns a 12-bit value representing the analog voltage reading from the temperature sensor. The obtained analog value needs to be converted to a temperature reading. You can use the conversion formula provided in the RP2040 datasheet or SDK to convert the ADC reading to a temperature value in degrees Celsius as shown in `read_onboard_temperature()`.
```
float read_onboard_temperature() {
    
    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}

void temp_task(__unused void *params) {
    float temperature = 0.0;
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);                 // Temperature sensor is on input 4

    while(true) {
        vTaskDelay(1000);
        temperature = read_onboard_temperature();
        printf("Onboard temperature = %.02f C\n", temperature);
    }
}
```

Include the following in the `vLaunch` function. This will create the instance of the task for execution. 
```
    TaskHandle_t temptask;
    xTaskCreate(temp_task, "TestTempThread", configMINIMAL_STACK_SIZE, NULL, 8, &temptask);
```

Build and download the project to the RPi Pico. You should continue seeing the ping message appearing on the serial monitor and the LED blinking. However, this time, you should also see an additional message appearing on the serial monitor which is the temperature data.

**Create an Average Filtering Task**

Filtering sensor data is crucial in embedded systems and other applications where sensors are employed. Sensors, regardless of their type, are prone to noise and interference from external sources, such as electromagnetic fields. This noise can distort the sensor's readings, making the raw data unreliable. Filtering helps mitigate the impact of noise, providing more accurate and reliable sensor readings. In real-world applications, sensor readings can exhibit fluctuations due to environmental changes or other factors. Filtering helps in smoothing the data by averaging the values over time, which aids in identifying trends and patterns more clearly and avoiding reactionary responses to sudden, brief changes in readings. Some filtering methods help in reducing the amount of data that needs to be processed, stored, or transmitted, which is crucial in resource-constrained environments. By removing unnecessary data points or outliers, filtering can help in optimizing the use of system resources, such as memory and processing power.

In the context of our application, we utilize a message buffer to receive temperature data from the `temp_task`. Message buffers are particularly effective for passing data between tasks, ensuring data integrity and synchronization between different parts of the system. In this case, the `temp_task` sends the temperature readings to the `avg_task` via the message buffer. Upon receiving the temperature data in `avg_task`, a Simple Moving Window Averaging Algorithm is implemented to filter the incoming data. This algorithm works by maintaining a 'window' of the most recent readings and computing the average, thereby providing a smoother and more consistent data output. This smoothed data, derived from the averaging algorithm, is essential for rendering a more accurate representation of the temperature trends, filtering out transient fluctuations and potential noise in the sensor readings, thereby contributing to the enhanced reliability and accuracy of the system’s response to the environmental conditions.

<img src="/img/task4.png" width=100% height=100%>

We start by including the header file (.h) to inform the compiler that we intend to use messagebuffer library in this build.
```
#include "message_buffer.h"
```

This is followed by including the handler and #define label to manage the message buffer.
```
#define mbaTASK_MESSAGE_BUFFER_SIZE       ( 60 )
static MessageBufferHandle_t xControlMessageBuffer;
```

You will also need to include the following in the `vLaunch` function, preferably after all the xTaskCreate. This is to create the message buffer that can be used by the various tasks. Note that message buffers have an N-to-1 relationship; thus, if you need one sender task to send to multiple receiver tasks, you will need to create multiple message buffers or use other FreeRTOS services, i.e. queues, etc. In this case, `xControlMessageBuffer` is the handle created to reference a single message buffer, which acts like a "mailbox" for sending and receiving tasks. If you need more "mailboxes" for different tasks or communication channels, you can create additional message buffers by declaring and creating similar handles. Each handle would reference its own separate message buffer, allowing you to manage multiple independent communication channels between tasks or interrupts in your FreeRTOS application. For instance, you could have multiple message buffers like `xSensorMessageBuffer`, `xDataMessageBuffer`, and so on, each serving a specific purpose within the system.
```
xControlMessageBuffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
```

Modify the temp_task to send a message each time it obtains new temperature data from the sensor. In the `temp_task` function, a message buffer, `xControlMessageBuffer`, is used as a mechanism for inter-task communication to send temperature data between tasks. The task initializes and configures the ADC to read the onboard temperature sensor and then enters into an infinite loop. In each iteration of the loop, after a delay, the task reads the onboard temperature and prints it. Subsequently, the temperature data is sent to the message buffer using `xMessageBufferSend()`. This function takes the message buffer to write to, a pointer to the source of the data to send, the length of the data to send, and the block time as parameters. In this case, the source of the data is the address of the `temperature` variable, the length of the data is the size of a `float`, and the block time is set to `0`, meaning the task does not block if the message buffer is full. The data sent to the message buffer can be received by another task using the `xMessageBufferReceive()` function, allowing seamless sharing of data between different tasks in the FreeRTOS environment.
```
void temp_task(__unused void *params) {
    float temperature = 0.0;
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while(true) {
        vTaskDelay(1000);
        temperature = read_onboard_temperature();
        printf("Onboard temperature = %.02f C\n", temperature);
        xMessageBufferSend( /* The message buffer to write to. */
            xControlMessageBuffer,
            /* The source of the data to send. */
            (void *) &temperature,
            /* The length of the data to send. */
            sizeof( temperature ),
            /* The block time; 0 = no block */
            0 );
    }
}
```

The `avg_task` function is a FreeRTOS task designed to calculate the moving average of temperature data. It operates in an infinite loop, continuously receiving temperature data through a message buffer, `xControlMessageBuffer`. The task maintains a static buffer, `data`, to hold the most recent four elements, and uses a variable, `sum`, to maintain the sum of the elements in this buffer. As new data points are received, the task subtracts the oldest element from `sum`, replaces it with the new data point, and then adds the new data point to `sum`. The index is then updated in a circular manner to ensure that it always points to a valid position within the buffer. If the buffer has not been filled, the count is incremented until it reaches the buffer's size. Finally, the task calculates and prints the average temperature by dividing the sum by the count of received elements.
```
void avg_task(__unused void *params) {
    float fReceivedData;
    size_t xReceivedBytes;
    static float data[4] = {0};
    static int index = 0;
    static int count = 0;
    float sum = 0;

    while(true) {
        xReceivedBytes = xMessageBufferReceive( 
            xControlMessageBuffer,        /* The message buffer to receive from. */
            (void *) &fReceivedData,      /* Location to store received data. */
            sizeof( fReceivedData ),      /* Maximum number of bytes to receive. */
            portMAX_DELAY );              /* Wait indefinitely */

            sum -= data[index];            // Subtract the oldest element from sum
            data[index] = fReceivedData;   // Assign the new element to the data
            sum += data[index];            // Add the new element to sum
            index = (index + 1) % 4;       // Update the index- make it circular

            if (count < 4) count++;        // Increment count till it reaches 4

            printf("Average Temperature = %0.2f C\n", sum / count);
    }
}
```


Include the following in the `vLaunch` function. This will create the instance of the task for execution. 
```
    TaskHandle_t avgtask;
    xTaskCreate(avg_task, "TestAvgThread", configMINIMAL_STACK_SIZE, NULL, 9, &avgtask);
```

Build and download the project to the RPi Pico. You should continue seeing the ping message appearing on the serial monitor and the LED blinking. However, this time, you should also see an additional message appearing on the serial monitor which is the current temperature data **AND** the averaged data.

I have attached the [modified version of the ping code](https://github.com/sirfonzie/INF2004_LAB5/blob/main/picow_freertos_ping.c) that includes all 4 tasks for those who are struggling and could not get it to compile successfully.

## **EXERCISE**

Develop an application using FreeRTOS that contains a task that reads the temperature data from the RP2040's built-in temperature sensor and sends it to two tasks every 1 second. The **second task** will perform a moving average on a buffer of ten data points, and the **third task** will perform a simple averaging. Additionally, create a **fourth task** exclusively for executing all the `printf` statements. No `printf` statements are allowed in any other task.
