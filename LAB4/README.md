# LAB 4: PWM & ADCs

**OBJECTIVES**
-	Configure and implement Pulse Width Modulation (PWM) on RPi Pico
-	Configure and implement an Analog-to-Digital Converter (ADC) on RPi Pico
-	Understand how to configure the (line following) IR-based sensor & L298N Motor controller


**EQUIPMENT** 
1.	A laptop that has the Pico C/C++ SDK installed
2.	Raspberry Pico W
3.	Micro-USB Cable
4.	IR-based Line Sensor
5.	Motor Controller & DC Motor

> [NOTE]
> Only students wearing fully covered shoes are allowed in the SR6A lab due to safety.

## **INTRODUCTION** 

In the previous lab session, we learned that we can use interrupts to detect and react to requests from the GPIO and Timer. For GPIO, we can react when the GPIO pins change while for the Timer, we can invoke callback functions once the timer has lapsed. A timer can also be used as a counter that keeps track of the number of times a particular event or process occurred regarding a clock signal. It is used to count the events happening outside the microcontroller. However, this counter can also be configured to produce a pulse with a specific duty cycle known as pulse-width modulation (PWM). It is particularly useful for tasks like motor control, LED dimming, and generating analog-like signals. PWM works by varying the duty cycle (percentage of time the signal is ON) of a square wave signal while keeping its frequency constant. 

Typically, GPIO when configured as input is able to accept data as high, low, or edge-based signals. However, the world we live in doesn't always offer discrete data. Thus, we would also be looking at Analog-to-Digital Converter (ADC), which is an essential part of any embedded system. It helps to obtain **Analog** data (i.e. temperature, humidity, etc) that can’t be directly read on any microcontroller as they are in analog format. To read this type of analog data, we have to convert this analog data into a digital format via an ADC inside the microcontroller.

Lastly, we will understand how we can use PWM to adjust the speed of the motor via the motor controller and incorporate ADC for the IR-based sensor in measuring contrast on the surface.

## **PULSE-WIDTH MODULATION** 

The Raspberry Pi Pico, with its GPIO pins and the Pico C SDK, provides a convenient way to generate PWM signals for various applications. Configuring PWM on the Raspberry Pi Pico using the Pico C SDK involves several steps, including initializing the PWM hardware, setting the frequency, configuring channels, and controlling the duty cycle. PWM on the Raspberry Pi Pico is achieved through PWM slices. To understand how PWM works on the Pico, it's essential to understand the concept of PWM slices and how they generate PWM signals.

**What are PWM Slices?**

PWM slices are hardware modules on the Raspberry Pi Pico designed to generate PWM signals. These slices offer a flexible and efficient way to produce PWM waveforms with precise control over parameters like frequency, duty cycle, and polarity. The Pico has multiple PWM slices, and each slice can generate one or more PWM channels.

**Key Features and Concepts:**

The RP2040 PWM block has eight identical slices, where each slice can drive two PWM output signals or measure the frequency or duty cycle of an input signal. This gives a total of up to 16 controllable PWM outputs. All 30 GPIO pins can be driven by the PWM block, as shown below.

<img src="/img/pwmgpio.png" width=100% height=100%>

**How PWM Slices Work:**

1. **Initialization:** Before using PWM, initialize the PWM system using the `pwm_init()` function. This initializes all PWM slices and channels.
2. **Configuring Channels:** You configure the GPIO pins for PWM operation by setting their functions to `GPIO_FUNC_PWM` using `gpio_set_function()`. This allocates the pins to specific PWM channels.
3. **Frequency Setup:** You set the desired PWM frequency using the `pwm_set_wrap()` function. This determines how fast the PWM signal oscillates between high and low.
4. **Duty Cycle:** To control the duty cycle, you use the `pwm_set_chan_level()` function to specify when the signal transitions between high and low within each PWM cycle.
5. **Enabling PWM:** Once you've configured the PWM slices and channels, you enable the PWM signal using `pwm_set_enabled()`.
7. **Updating Parameters:** If needed, you can update the PWM parameters, such as frequency or duty cycle, at runtime to modify the PWM signal.
8. **Disabling PWM:** When you're done with PWM, disable it using `pwm_set_enabled()`.

The following code [hello_pwm](https://github.com/raspberrypi/pico-examples/blob/master/pwm/hello_pwm/hello_pwm.c) illustrates a simple example of how a PWM can be configured on the Raspberry Pi Pico. You could connect GP0 and GP1 to an oscilloscope to observe the signal. Alternatively, you may observe the effects of PWM on the motor controller by connecting, as described in the next section.

### Key PWM Concepts:
- **PWM (Pulse Width Modulation)**: A technique to control the voltage output by rapidly switching the output between high and low, effectively varying the average voltage over time. It is widely used in motor control, LED brightness control, and other applications where analog-like behavior is desired from digital signals.
- **Duty Cycle**: The percentage of time the signal is in a high (on) state within a single PWM period. For example, a 50% duty cycle means the signal is high for half the time and low for the other half.
- **Frequency**: The number of times the PWM cycle repeats per second. For motor control, a typical PWM frequency might be in the range of 100Hz to a few kHz.


## **UNDERSTANDING THE L298N MOTOR CONTROLLER - hello_pwm example**

The L298N module is a high-power motor driver module for driving DC and stepper motors. This module comprises an L298 motor driver IC and a 78M05 5V regulator. This module can control up to two DC motors with directional and speed control. The module's datasheet can be found at the following [link](https://components101.com/modules/l293n-motor-driver-module). 

<img src="/img/l298n_pinout.png" width=30% height=30%>

HiBit. (2023). *L298N motor driver pinout diagram* Retrieved from [here](https://components101.com/modules/l293n-motor-driver-module) 

https://cdn.hibit.dev/images/posts/2023/schemas/l298n_pinout.png

There are two ways for this module to control the speed of the motor. An easier way would be to use a jumper across ENA and ENB to fix the voltage to the maximum. However, removing the jumper and connecting the pin to a PWM source would facilitate controlling the motor’s speed. PWM is a widely used technique to control the speed of DC motors, including those used in robotics and other applications.

<img src="/img/l298npico.png" width=100% height=100%>

Again, we can use the [hello_pwm](https://github.com/raspberrypi/pico-examples/blob/master/pwm/hello_pwm/hello_pwm.c) code with some changes to demonstrate how PWM can control the motor speed using the L298N motor controller. Connect the motor controller as follows and observe how fast the motor turns.

An alternative image of connecting them can be seen [here](/img/motorconnection.jpg).

The following changes are required:
1. change line #16: `gpio_set_function(0, GPIO_FUNC_PWM);` --> `gpio_set_function(2, GPIO_FUNC_PWM);`
1. include this in line #21: `pwm_set_clkdiv(slice_num, 100);`
2. change line #23: `pwm_set_wrap(slice_num, 3);` --> `pwm_set_wrap(slice_num, 12500);`
3. change line #25: `pwm_set_chan_level(slice_num, PWM_CHAN_A, 1);` --> `pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500/2);`  
4. comment off line #27: `pwm_set_chan_level(slice_num, PWM_CHAN_B, 3);` --> `\\pwm_set_chan_level(slice_num, PWM_CHAN_B, 3);`

This modified code snippet configures the Raspberry Pi Pico to generate PWM (Pulse Width Modulation) signals on GP2 via the gpio_set_function function. It then obtains the PWM slice number associated with GP0 and sets the clock divisor to 100, reducing the main clock from 125Mhz to 1.25Mhz frequency. The pwm_set_wrap function sets the PWM wrap value, essentially determining the period of the PWM signal. Here, it's set to 12500, which corresponds to a period of 10ms. This is obtained as follows: ```((1 / 1.25Mhz)*12500) ```. pwm_set_chan_level sets the duty cycle of the PWM signal on channel A of the specified PWM slice to 50% (12500/2). Finally, pwm_set_enabled enables the PWM output on the specified slice. In summary, this code initializes PWM on GP2 with a 100Hz frequency and a 50% duty cycle, effectively generating a square wave output.

The L298N motor controller's N3 and N4 pins control the motor's turning direction. In this example, we connect N3 to GP0 and N4 to GP1. By setting GP0 and GP1 to different combinations of High and Low, the motor can be controlled to rotate clockwise, counterclockwise, or stop. For example:

- Setting GP0 High and GP1 Low will rotate the motor clockwise.
- Setting GP0 Low and GP1 High will rotate the motor counterclockwise.
- Setting both GP0 and GP1 either High or Low will stop the motor."

## **UNDERSTANDING THE L298N MOTOR CONTROLLER - pwm_set_gpio_level example**

The provided alternative C [code](https://github.com/sirfonzie/INF2004_LAB4/blob/main/l298n.c) demonstrates how to configure a Raspberry Pi Pico to use Pulse Width Modulation (PWM) on GP2 and control motor direction using an L298N motor driver via GP0 and GP1. Let's break down the essence of each part of the PWM-related functions and other elements in the [code](https://github.com/sirfonzie/INF2004_LAB4/blob/main/l298n.c).

### Key PWM-related Code Snippets Explained:

1. **`setup_pwm` Function:**
   This function is the heart of the PWM configuration, setting the frequency and duty cycle for a specified GPIO pin.
   
   - **Set the GPIO function to PWM:**
     ```c
     gpio_set_function(gpio, GPIO_FUNC_PWM);
     ```
     This configures the specified GPIO pin (e.g., `PWM_PIN`) to function as a PWM output.

   - **Find the PWM slice:**
     ```c
     uint slice_num = pwm_gpio_to_slice_num(gpio);
     ```
     Each GPIO pin has an associated **PWM slice** (hardware block) in the Raspberry Pi Pico's PWM peripheral. This function retrieves the correct slice number for the specified GPIO pin. This allows us to configure PWM parameters for that particular slice.

   - **Calculate the clock divider and set the PWM frequency:**
     ```c
     float clock_freq = 125000000.0f;  // Default clock frequency of the Pico in Hz
     uint32_t divider = clock_freq / (freq * 65536);  // Compute clock divider
     pwm_set_clkdiv(slice_num, divider);
     ```
     The Raspberry Pi Pico operates with a clock frequency of 125 MHz. The divider is calculated based on the target frequency (`freq`) and the resolution (16 bits, or 65536 steps for PWM), ensuring the PWM frequency is set to the desired value.

   - **Set the PWM wrap value:**
     ```c
     pwm_set_wrap(slice_num, 65535);  // 16-bit resolution (0-65535)
     ```
     The **wrap value** defines the maximum count of the PWM cycle. A value of `65535` corresponds to a 16-bit resolution (0 to 65535 steps), which means the PWM can modulate with a precision of 1/65536.

   - **Set the duty cycle:**
     ```c
     pwm_set_gpio_level(gpio, (uint16_t)(duty_cycle * 65536));
     ```
     The `pwm_set_gpio_level` function sets the output level for the given GPIO pin based on the duty cycle. A duty cycle of 50% corresponds to setting the output level to half the wrap value, ensuring the PWM signal is high 50% of the time.

   - **Enable the PWM:**
     ```c
     pwm_set_enabled(slice_num, true);
     ```
     This enables the PWM for the specified slice.

2. **`main` Function:**
   This is the main loop of the program that controls the motor direction using GPIO pins connected to the L298N motor driver.

   - **Set up the PWM on GP2:**
     ```c
     setup_pwm(PWM_PIN, 100.0f, 0.5f);
     ```
     This calls the `setup_pwm` function to configure GP2 with a 100Hz PWM frequency and a 50% duty cycle.

   - **Control motor direction in an infinite loop:**
     ```c
     while (true) {
         gpio_put(DIR_PIN1, 1);  // Forward direction
         gpio_put(DIR_PIN2, 0);
         sleep_ms(2000);         // Wait for 2 seconds

         gpio_put(DIR_PIN1, 0);  // Reverse direction
         gpio_put(DIR_PIN2, 1);
         sleep_ms(2000);         // Wait for 2 seconds
     }
     ```
     Inside the loop:
     - It first sets the direction of the motor to forward by setting GP0 high and GP1 low.
     - After 2 seconds, it reverses the direction by toggling GP0 and GP1.
     - This loop continuously switches the motor between forward and reverse every 2 seconds.

### Summary of PWM Functions:
- **`setup_pwm()`**: Configures the PWM output on a given GPIO pin with a specified frequency and duty cycle. It handles the setup of the PWM slice, clock divider, wrap value, and output level.
- **`pwm_gpio_to_slice_num()`**: Retrieves the PWM slice associated with a GPIO pin.
- **`pwm_set_wrap()`**: Sets the maximum count for the PWM cycle (i.e., the resolution).
- **`pwm_set_gpio_level()`**: Sets the duty cycle of the PWM signal on a specific GPIO pin.
- **`pwm_set_enabled()`**: Enables the PWM for the given slice.


## **ANALOG-TO-DIGITAL CONVERTER**

An Analog-to-Digital Converter (ADC) on the Raspberry Pi Pico functions by converting continuous analog signals, such as voltage levels from sensors, into discrete digital values that the Pico can process. It does this through a multi-step process. First, the ADC captures the analog voltage from a sensor input pin and samples it at regular intervals. Then, it quantizes the sampled voltage, assigning it a digital value based on its magnitude. The resolution of the ADC determines the number of discrete values it can represent. The Raspberry Pi Pico's ADC typically provides a 12-bit resolution, allowing for 4,096 possible values. This digitized data can then be used in software applications, enabling the Pico to process and respond to analog inputs from the physical world.

The following code [adc_console](https://github.com/raspberrypi/pico-examples/blob/master/adc/adc_console/adc_console.c) illustrates a simple example of how an ADC can be configured on the Raspberry Pi Pico. This code creates a console interface for interacting with the Raspberry Pi Pico's ADC, allowing users to select channels, sample analog data, capture multiple samples, and perform GPIO operations for testing and experimentation.

The code you provided deals with the ADC (Analog-to-Digital Converter) functionality on the Raspberry Pi Pico. Let’s break down the sections that affect the ADC operations.

### 1. **ADC Initialization (`adc_init()`)**
   ```c
   adc_init();
   ```
   - This initializes the ADC peripheral. It sets up the ADC for further operations, including reading analog values. The `adc_init()` function prepares the ADC for subsequent operations like enabling the temperature sensor and selecting input channels.

### 2. **Enable Temperature Sensor (`adc_set_temp_sensor_enabled(true)`)**
   ```c
   adc_set_temp_sensor_enabled(true);
   ```
   - This enables the internal temperature sensor as an ADC input. The temperature sensor is connected to one of the internal ADC channels, allowing you to read temperature data. 

### 3. **Selecting the ADC Input Channel (`adc_select_input(c - '0')`)**
   ```c
   adc_select_input(c - '0');
   ```
   - This section allows you to select an ADC channel based on user input. The code converts the ASCII value of the input (`c`) into a number (`c - '0'`) to select the corresponding ADC input channel (0–7).
   - The function `adc_select_input()` configures the ADC to sample from the specified input channel.

### 4. **Single ADC Sample (`adc_read()`)**
   ```c
   uint32_t result = adc_read();
   const float conversion_factor = 3.3f / (1 << 12);
   printf("\n0x%03x -> %f V\n", result, result * conversion_factor);
   ```
   - This block reads a single sample from the selected ADC channel.
   - The `adc_read()` function returns the raw ADC value (a 12-bit number). 
   - The **conversion factor** converts the ADC reading to a voltage value. Since the ADC has 12-bit resolution (i.e., it can output values from 0 to 4095), the reading is mapped to a voltage range (0 to 3.3V).
   - The result is then printed, both as a hexadecimal value and as a voltage.

### 5. **Capturing Multiple ADC Samples (`adc_capture(sample_buf, N_SAMPLES)`)**
   ```c
   adc_capture(sample_buf, N_SAMPLES);
   ```
   - This function captures multiple ADC samples and stores them in the `sample_buf` buffer. The number of samples to capture is defined by the constant `N_SAMPLES`.
   - The function `adc_fifo_get_blocking()` is used inside `adc_capture()` to retrieve each ADC sample from the FIFO (First In, First Out) buffer and store it in the buffer array.

   ### Key elements inside `adc_capture()`:
   - **FIFO Setup (`adc_fifo_setup()`)**: 
     ```c
     adc_fifo_setup(true, false, 0, false, false);
     ```
     This function configures the ADC's FIFO buffer. It ensures that the FIFO is enabled and ready to store ADC data.
   
   - **Enable ADC and Capture Samples**:
     ```c
     adc_run(true);
     for (size_t i = 0; i < count; i = i + 1)
         buf[i] = adc_fifo_get_blocking();
     adc_run(false);
     ```
     The ADC is started with `adc_run(true)`, and the loop reads multiple samples from the FIFO buffer using `adc_fifo_get_blocking()`. Once all samples are captured, the ADC is stopped with `adc_run(false)`.

   - **Drain the FIFO (`adc_fifo_drain()`)**: 
     ```c
     adc_fifo_drain();
     ```
     This function clears the FIFO after the sampling operation to ensure no leftover data remains in the buffer.

### 6. **Displaying Multiple Samples:**
   ```c
   for (int i = 0; i < N_SAMPLES; i = i + 1)
       printf("%03x\n", sample_buf[i]);
   ```
   - After capturing the ADC samples, the program iterates through the buffer and prints each sample in hexadecimal format. The `sample_buf[]` array holds the raw ADC values from the multi-sample capture.

### 7. **User Commands for ADC:**
   - The program provides a simple console-based interface to interact with the ADC. The relevant commands for the ADC are:
     - **`c[n]`**: Switch ADC input to channel `n` (0–7).
     - **`s`**: Sample the ADC once and display the result.
     - **`S`**: Capture multiple samples (as defined by `N_SAMPLES`) and print the results.

### Summary:
- **Initialization**: The ADC is initialized, and the temperature sensor is enabled.
- **Channel Selection**: The user can select an ADC input channel (0–7).
- **Single Sample**: The code reads a single ADC sample, converts it to voltage, and prints it.
- **Multiple Samples**: The code can capture multiple ADC samples and print them after storing them in a buffer.
- **FIFO Management**: FIFO is used to handle the multiple samples, with functions to start, stop, and clear the FIFO as needed.


## **CONFIGURING THE IR-BASED LINE SENSOR**

The IR line sensor is a reflective sensor that includes an infrared emitter and phototransistor in a leaded package which blocks visible light. The infrared emission diode constantly emits infrared rays. When the intensity of the reflection is not received or is not large enough, the light-activated diode will be in the off state. However, if the sensor detects an object within the field of view, infrared light is reflected back and activates the diode.

The IR sensor depicted below features both analog and digital outputs. The digital output alternates between high and low based on the infrared light's reflection, with a threshold set via a potentiometer determining the state. However, in this lab, we will exclusively utilize the Analog output (A), which provides a continuous signal proportional to the intensity of the reflected IR light. Subsequently, we will employ the ADC to convert this analog signal into a digital value, enabling precise measurement and offering a more comprehensive range of information compared to the binary high-low output of the digital mode.

<img src="/img/linepico.png" width=100% height=100%>

Again, we can re-use the adc_console code to demonstrate how ADC can be used to obtain the analog signal from the IR-based sensor and convert it into a digital format. Connect the IR sensor as follows and observe the data received via the serial monitor.

## **EXERCISE**

To configure a PWM signal at 20 Hz with a 50% duty cycle on GP0 and feed it into an ADC at GP26 while sampling the ADC every 25 ms, you must use the Raspberry Pi Pico and its Pico C SDK. You will need to use a jumper wire to connect GP0 to GP26. You may use a timer interrupt. The output to look as follows:

<img src="/img/ex4.png" width=100% height=100%>
