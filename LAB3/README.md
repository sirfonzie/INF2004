# LAB 3: Interrupts & Timers

**OBJECTIVES**
-	Learn the difference between polling and interrupts
-	Configure and implement hardware Interrupts
-	Configure and implement various Timer
-	Understand how to configure the HC020K IR Wheel Encoder & HCSR04P Ultrasonic sensor


**EQUIPMENT** 
1.	A laptop that has the Pico C/C++ SDK installed
2.	Raspberry Pico W
3.	Micro-USB Cable
4.	4-Wire Groove Cable

> [NOTE]
> Only students wearing fully covered shoes and long pants are allowed into the lab due to safety concerns.

## **INTRODUCTION** 

Embedded systems can interact with their surrounding environment in many ways. Whether sensing or actuating, there are two primary mechanisms initiating actions: event-triggered and time-triggered. In this lab, we will first learn how to use interrupts, the building blocks for event-triggered functionality. Lastly, we will build code that uses interrupts that the IR sensor can use as a wheel encoder.

Subsequently, we shall learn how to use time-triggered events known as Timers. A timer is a specialised type of clock used to measure time intervals. A timer that counts from zero upwards for measuring the time elapsed is often called a stopwatch. It is a device that counts down from a specified time interval and generates a time delay. A timer can also be used as a counter that keeps track of the number of times a particular event or process occurred regarding a clock signal. It is used to count the events happening outside the microcontroller. 

## **HARDWARE INTERRUPTS** 

A hardware interrupt is an electronic signal that alerts the microprocessor of an event. An interrupt can be triggered by either an internal peripheral (e.g. timer) or an external device (e.g. button).

In the previous lab session, polling was used to detect when a button was pressed. When polling is used, the microprocessor repeatedly checks whether the event has occurred. In the case of a button, the value of the GPIO pin is read to determine if it is high (unpressed) or low (pressed). Once the button is pressed, the microprocessor detects it quickly since it is always active and does nothing but check this single condition, as shown in Figure 1 (bottom). 

While polling is a simple way to check for state changes, there's a cost. If the checking interval is shorter, there can be a long lag between occurrence and Detection, and you may fail to see the change entirely if the state changes back before you check. A shorter interval will get faster and more reliable Detection and consume more processing time and power since many more checks will return negative.

An alternative is configuring an interrupt on the button's GPIO pin so that an interrupt is generated when a pre-configured trigger condition is met. With this approach, the microprocessor can enter a low-power sleep state and be woken up with the interrupt. The Pico can detect signal changes (e.g. rising or falling edges) to generate an interrupt. If a GPIO pin is configured to be pulled up, a falling edge will occur when the button is pressed, pulling the pin to the ground. Interrupts are thus better suited to handle asynchronous events.

<img src="https://www.renesas.com/sites/default/files/inline-images/fig1-interrupts-vs-polling-en.jpg" width=80% height=80%>

A dedicated or grouped interrupt is triggered, depending on the source of the interrupt. For peripherals like GPIO ports, multiple pins could produce the same interrupt. In these cases, it is necessary to query the pin's interrupt vector register to identify the interrupt's exact source. Typically, this is done inside the ISR. Once an ISR identifies the source of the interrupt, it can react accordingly. Typically, ISRs execute in a privileged mode that can mask other interrupts. Hence, ISR should be as short as possible and only set application-specific flags to indicate to the microprocessor's main thread to execute the corresponding task in response to an interrupt.

## **GPIO INTERRUPT REQUEST**

We will be exploring the [hello_gpio_irq.c](https://github.com/raspberrypi/pico-examples/blob/master/gpio/hello_gpio_irq/hello_gpio_irq.c) sample code designed for the Pico W. In this session, we'll merge our knowledge of GPIO with the concept of interrupts. Instead of the previous lab's approach, where we continuously polled the GPIO pin status using the `while(true)` statement, we'll now integrate interrupts. This will allow us to trigger the interrupts based on the desired state, whether edge-triggered or level-triggered. In this example, edge-triggered has been chosen. How would you change it to level-triggered? 

To test the code, you must connect the GP02 pin to 3.3V while observing the output on the Serial Monitor. Can you identify the function that sets GP02? In the [CMakeLists.txt](https://github.com/raspberrypi/pico-examples/blob/master/gpio/hello_gpio_irq/CMakeLists.txt), __ensure__ that the following line has been added `pico_enable_stdio_usb(hello_gpio_irq 1)`. In addition, `stdio_init_all()` should also be included in your main() function.

> [NOTE]
> Switching to trigger at a low-level (GPIO_IRQ_LEVEL_LOW) could lead to the software crashing (not working). Why?

## **IR-BASED WHEEL ENCODER**

<img src="img/irwheelencoder.PNG" width=30% height=30%>

The working principle of the encoder (shown above) is illustrated in the image below. It uses a slotted wheel with a single LED and photodetector pair that generate pulses as the wheel turns, and the speed of an object can be calculated by measuring the pulse duration Δti  (i.e. elapsed time or period of a pulse) between successive pulses. It comprises three connections: GND, VCC and OUT. GND and VCC supply power to the module (in our case, via the Pico's GND and 3.3V pins), while OUT generates the square-pulse signal. Connecting the GP02 from the Pico to the OUT of the IR-Sensor will allow the Pico to detect when the wheel is turning.

<img src="img/encoder.png" width=50% height=50%>


You may now change the callback function to convert this code into the wheel encoder driver. To measure distance, count each time the notch has been detected (Hint: Use edge-triggered). To measure speed, you will need to measure the pulse width.

<img src="img/irpico.png" width=70% height=70%>

## **TIMERS**

The RP2040 found in the Pico features a flexible timer system that can be used for various applications. Here is a brief description of its timer system, focusing on timer modes, input capture, and output compare:

1. **Timer Modes:**
    - **Free Running Mode:** In this mode, the timer simply counts from 0 to its maximum value and then wraps around to start counting again.
    - **Periodic Mode:** In this mode, the timer counts up to a predefined value (ALARM0, for instance) and then restarts from zero. This can be used to generate periodic interrupts or events.
    - **One-Shot Mode:** In this mode, the timer counts up to a predefined value and then stops. It's useful for creating a single delay or measuring an event of a known duration.

2. **Input Capture:**
    - Input capture is used to measure the time duration of an external event. For example, it can measure the duration of a pulse on a pin.
    - When the event occurs (like a rising or falling edge on a pin), the current timer value is 'captured' and stored in a register.
    - By comparing consecutive captured values, you can determine the duration of the event or the period between events.
    - The RP2040 timer can be configured to generate an interrupt when such capture occurs, which allows the CPU to process the captured value or take action based on the event.

3. **Output Compare:**
    - Output compare generates an event (like toggling a pin) at a specific timer value.
    - You set a value in a compare register, and an action can be triggered when the timer counts up to that value.
    - The action can be as simple as generating an interrupt or as complex as toggling a GPIO pin, generating PWM signals, etc.
    - This feature can be very useful in applications like motor control, where precise timing of events is crucial.

It's also worth noting that the RP2040 timer system provides multiple alarm (compare) registers, allowing multiple compare values to be active simultaneously. This multi-alarm capability can be particularly useful in applications requiring various events to happen simultaneously without constant CPU intervention. To make full use of the timer capabilities, you should take a look at the RP2040 datasheet and SDK, which will provide more detailed information and examples on how to configure and use the timers.

## **Periodic vs Single-shot**

This example [hello_timer.c](https://github.com/raspberrypi/pico-examples/blob/master/timer/hello_timer/hello_timer.c) illustrates how to configure a single-shot and a periodic-based timer that is used to trigger an interrupt. This interrupt will then trigger a function to perform the user-defined code. Changing the `delay_ms` parameter in the `add_repeating_timer_ms` API call can trigger different behaviours. Observe the starting time at each call.

The image below illustrates two different configurations for the periodic timer used in the sample code above.
<img src="img/periodic.png" width=70% height=70%>

## **HOW DOES THE ULTRASONIC HC-SR04P WORK**

<img src="img/ultrasonic.png" width=50% height=50%>

1. **Initiation**: A short high pulse, typically around 10 microseconds in duration, is applied to the "Trigger" pin of the HC-SR04 module to initiate a measurement.
2. **Ultrasonic Pulse Emission**: Once triggered, the HC-SR04 responds by emitting a burst of eight ultrasonic pulses at approximately 40 kHz. These sound waves travel through the air, radiating outward from the module's transmitter.
3. **Reflection and Reception**: If an object is present within the sensor's detection range, the emitted ultrasonic waves will bounce off that object's surface and reflect toward the module. The module's ultrasonic receiver, or the "Echo" pin, detects these reflected sound waves. The duration for which the "Echo" pin stays high is directly proportional to the time it takes for the emitted ultrasonic waves to hit an object and return.
4. **Distance Derivation**: The time measured from the emission of the ultrasonic pulse to its reception (the round trip) can be used to derive the distance to the object. Given that, we know the speed of sound in air (approximately 343 meters per second or 1125.33 feet per second at room temperature), the formula to calculate this distance is: <img src="img/distance.png" width=25% height=25%> The division by two accounts for the round trip of the sound waves; we need the time for just one way to determine the distance to the object.

<img src="img/hcsr04pico.png" width=70% height=70%>

## **SAMPLE CODE FOR ULTRASONIC HC-SR04P**

The following [example](https://github.com/KleistRobotics/Pico-Ultrasonic/blob/main/ultrasonic/ultrasonic.c) uses simple GPIO and delays to achieve the trigger and echo calculation to obtain the distance. However, this code is **incomplete** and might contain syntax errors. Moreover, it is **inefficient** due to the use of block-waiting, which can lead to unresponsive behavior (lines #26 & #28).

> [NOTE]: HC-SR04P and HC-SR04+ both supports 3.3V microcontroller such as Pi Pico and ESP32. However, HC-SR04 is a 5V based sensor and can only be used on an Arduino Uno and **NOT** on a Pi Pico and ESP32.

## **EXERCISE**

The objective is to develop a simple stopwatch application. The stopwatch will be controlled by a single button on **GP21**. **Pressing and holding** this button starts the timer, and the elapsed time in seconds will be continuously displayed on the Serial Monitor. **Releasing** the button stops the timer and resets the displayed time to zero. To ensure a smooth user experience and accurate timekeeping, the **GP21** button input will be **debounced**, and **timer interrupts** must be employed.
