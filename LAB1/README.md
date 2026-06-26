# LAB 1: Microcontroller and its Development Environment

**OBJECTIVES**
- To setup and get you familiarised with the development environment for Raspberry Pi Pico W.
- To get you familiarised with the Raspberry Pi Pico development board.

**EQUIPMENT** 
1.	A laptop that has the Pico C/C++ SDK installed
2.	Raspberry Pi Pico or Raspberry Pi Pico W
3.	Micro-USB Cable 

> [NOTE]
> Only students wearing fully covered shoes are allowed in the lab due to safety. 

## **INTRODUCTION** 

Programming an embedded system requires a deep-level understanding of the embedded processor architecture, the development environment tool, and the hardware interfaced with the embedded system. In this laboratory session, you will be introduced to the basic tools and how they can be used to aid your understanding of software development and, more importantly, to help you build your software. Programming for an embedded system differs significantly from programming on a desktop computer. The main differences are the limited resource constraints in terms of the program code and RA,M and the overall computation performance of an embedded system. For instance, our RP2040 microcontroller only provides 256KB of SRAM memory space. A 16MB Flash memory is added to the board and externally connected to the RP2040. Also, the various software components discussed in the lecture will be observed in  this session. At this point, you should  be familiar with  the fundamentals of C programming. You are encouraged to review it if you still need to. Do revise ALL the fundamentals of C programming covered in the following [site](https://www.cprogramming.com/tutorial/c-tutorial.html). This will prepare you for the subsequent lab sessions and make this subject more enjoyable. In addition, please brush up on the following:
- “Numeral Systems” (e.g. binary, decimal, hexadecimal, etc)
- “Ordering Consideration” (e.g. endianness, MSB, LSB, etc)

## **RASPBERRY PI PICO** 

In this lab session, we learned about the Raspberry Pi Pico development platform and the Pico C/C++ SDK, which is used throughout all lab sessions. We will also familiarise you with concepts like direct register access, polling, and serial communication. The following introduction provides a broad overview of the working environment; however, it is not necessary to understand all the details to complete this laboratory. 

The Raspberry Pi Pico is an affordable microcontroller board developed by the Raspberry Pi Foundation, ideal for electronics projects. It features a dual-core ARM Cortex-M0+ processor, 26 GPIO pins, and supports multiple programming languages. However, it lacks built-in wireless connectivity. The Raspberry Pi Pico Wireless (Pico W) is an enhanced version of the Raspberry Pi Pico, featuring built-in Wi-Fi and Bluetooth. This makes it suitable for IoT and wireless communication projects while maintaining compatibility with Pico's programming languages and GPIO pins.

The Raspberry Pi Pico family currently consists of four boards: Raspberry Pi Pico, Pico H, Pico W, and Pico WH.

The following is the pinout for the Raspberry Pi Pico
![Screenshot of a Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/images/pico-pinout.svg)

The following is the pinout for the Raspberry Pi Pico W
![Screenshot of a Raspberry Pi Pico W](https://www.raspberrypi.com/documentation/microcontrollers/images/picow-pinout.svg)

The most important documents of an embedded system are, among others, datasheets, user guides, technical reference manuals, application notes, errata and schematics. Therefore, every embedded system comes with many documentation files. All the necessary files for this lab and subsequent labs can be found on the Raspberry Foundation site and supplementary documents will be provided on the course xSITe website. It is essential to have access to all parts of the documentation to use the functionality of an embedded system to its fullest extent. Details of the hardware can be found [here](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html).

---

## **Step 1: Updating Dependencies**

### **Raspberry Pi OS and Windows**
No additional dependencies are required.

### **Linux**
Most Linux distributions come with the necessary dependencies preinstalled. However, if required, install the following:
- Python 3.9 or later
- Git
- Tar
- A native C and C++ compiler (GCC supported)

Run the following command to install missing dependencies:
```bash
sudo apt install python3 git tar build-essential
```

### **macOS**
*The steps described for macOS are extracted from this [guide](https://www.eldelto.net/articles/raspberry-pi-pico-setup-for-macos).*

To install the required dependencies, run:
```bash
xcode-select --install
```
This installs:
- Git
- Tar
- A native C and C++ compiler (GCC and Clang supported)

---

## **Step 2: Setting Up the Pico SDK**
Ensure your system has the following tools installed:
- **CMake**: For generating build files.
- **GNU Make**: For compiling.
- **ARM GCC Toolchain**: For cross-compilation.

Install the Pico SDK and necessary tools:
```bash
sudo apt install cmake
sudo apt install make
sudo apt install gcc-arm-none-eabi
```

Clone and initialize the Pico SDK & Pico Examples:
```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git
```

---

## **Step 3: Compiling and Running Your Files**
Once the Pico SDK is installed, follow these steps to compile and flash your files to the Pico W.

1. **Open a terminal.**
2. **Set the environment path for the Pico SDK**
   ```bash
   export PICO_SDK_PATH=/home/username/pico_path/pico-sdk
   ```
3. **Navigate to the directory** where your project files are located.
4. **Create a build directory:**
   ```bash
   mkdir build
   cd build
   ```
5. **Generate build files and compile:**
   ```bash
   cmake -DPICO_BOARD=pico_w ..
   make
   ```
6. **Flash your compiled file to the Pico W:**
   ```bash
   cp build/your_file.uf2 /media/your_username/RPI-RP2
   ```
   Alternatively, you can **drag and drop** the UF2 file onto the Pico W drive in your file manager.

---

## **Optional: Using Visual Studio Code Extension**
You can also use the **Visual Studio Code Pico W extension** to build and flash your files.

### **Steps:**
1. **Open Visual Studio Code.**
2. Ensure the [**Pico W extension**](/img/raspberry_pico_ext.png) is installed.
3. Click on the [**Pico W icon**](/img/ext_icon.png) in the sidebar.
4. Click on [**"New Project From Example"**](/img/project_example.png) and select blinky as your first project follow the setup instructions.
5. Click on the **"Build"** button to compile your files.
6. Click on the **"Flash"** button to upload your files to the Pico W.

> **Note:** The initial project build may take some time, but subsequent builds will be much faster.

---

## **Alternative: Using Visual Studio Code**

There are various [methods](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) (see chapters 2 and 9) to setup the development environment for the pico in C using the Pico SDK, depending on what OS you are using on your PC/laptop. 

**Windows OS**

1. Download and install [this](https://github.com/raspberrypi/pico-setup-windows/releases/latest/download/pico-setup-windows-x64-standalone.exe) tool.

2. Launch Pico Visual Studio Code: use the Windows search and type **"pico"** to locate the application.

3. Open pico_examples folder: Go to **"File->Open Folder"**, locate the folder. ie. "C:/Users/<YourName>/Documents/Pico-v1.5.0/pico_examples"

Visual Studio Code will ask if you want to configure the pico-examples project when it is first opened; click *Yes* on that prompt to proceed. You will then be prompted to select a kit -- select the *Pico ARM GCC - Pico SDK Toolchain with GCC arm-none-eabi* entry. 

> [NOTE]
> Please restart your PC/laptop (multiple times) after installing the SDK. This resolved many first-time compilation error issues that were brought to my attention. :)

**macOS**

1. Download "Visual Studio Code.app" from [here](https://code.visualstudio.com/download). *Drag "Visual Studio Code.app" into the Applications folder.*

2. Install all required dependencies:
   ```bash
   brew install gcc-arm-embedded libusb make cmake git
   ```
   
3. Create a new folder called **pico** to contain all related tools:
   ```bash
   mkdir pico
   ```

4. Checkout the **pico-sdk** from Github:
   ```bash
   cd pico
   git clone https://github.com/raspberrypi/pico-sdk.git
   ``` 

5. Fetch all referenced submodules:
   ```bash
   cd pico-sdk
   git submodule update --init
   ```  

6. Checkout the **pico-examples** from Github:
   ```bash
   cd ..
   git clone https://github.com/raspberrypi/pico-examples.git
   ```

7. Delete the **build** folder inside pico-examples (skip this step if this is a fresh setup).:
   ```bash
   cd pico_examples
   rm -rf build
   ```

8. Set two environment variables that are required by the build process
   ```bash
   export PICO_SDK_PATH=/users/<YourName>/pico/pico-sdk
   export PICO_BOARD=pico_w
   ```

9. Launch Pico Visual Studio Code from terminal (to inherit terminal environment variables):
   ```bash
   /Applications/Visual Studio Code.app/Contents/Resources/app/bin/code
   ```

10. Ensures environment variables are automatically applied in new terminal sessions.
   ```bash
   echo 'PICO_SDK_PATH=/users/<YourName>/pico/pico-sdk' >> ~/.zshrc
   echo 'PICO_BOARD=pico_w' >> ~/.zshrc
   ```

11. Open pico_examples folder: Go to **"File->Open Folder"**, locate the folder. ie. "/users/username/pico/pico-examples".
You will then be prompted to select a kit -- select the *arm-none-eabi-gcc* entry.

## **BUILDING AN EXAMPLE**

Ensure you select the right application when starting Visual Studio Code, as two variations might be installed on your laptop. The icon should look as follows:

![Screenshot of Pico - Visual Studio Code](/img/pico_vsc.png)

Once "Pico - Visual Studio Code" (VSCode) is started, click the [CMake](/img/cmake.png) icon and select the sample code you want to work on. In this example, we will use the [Hello World](https://github.com/raspberrypi/pico-examples/tree/master/hello_world/usb) example. The following [video](https://www.youtube.com/watch?v=NPwoflT_bB0) demonstrates how you get started with VSCode. Note that we are using the hello_usb version of the code. This allows the USB connection between the pico and the PC/laptop to become a virtual UART connection, which can be used together with printf (for debugging purposes).

Now, try to compile and run the [blink](https://github.com/raspberrypi/pico-examples/tree/master/blink) example.

If you are using the Pico W boards, you must make a small amendment to the CMakeLists.txt file. Include "set(PICO_BOARD pico_w)" to line #11. The following [video](https://www.youtube.com/watch?v=sTNtLkoHN58) demonstrates how to make the changes and build a [blink](https://github.com/raspberrypi/pico-examples/tree/master/pico_w/wifi/blink) example for the Pico W. 

![Screenshot of Pico - Visual Studio Code](/img/picow_support.png)

> [NOTE]
> The normal blink example will only work on a standard Pico (without wireless). This is because the Pico W LED is connected to the WiFi SoC and not directly to the RP2040.

## **DOWNLOADING FIRMWARE INTO THE PICO**

Depending on your preferences and requirements, several methods are available to upload firmware to a Raspberry Pi Pico microcontroller board. Here is a brief overview of two of the most common methods:

<img width="835" height="625" alt="image" src="https://github.com/user-attachments/assets/3d1be0db-e686-44a9-ab1c-2460425ecf4b" />


1. **Drag and Drop (Mass Storage Device):**
   - The Raspberry Pi Pico has a built-in feature that makes it appear as a mass storage device when connected to a computer via USB.
   - To get the board in bootloader mode ready for the firmware update, hold down the BOOTSEL button while plugging the board into USB. You can only release the button once you connect the pico to the PC/laptop properly.
   - Drag and drop a UF2 file onto Pico's virtual drive to upload firmware using this method.
   - This is a beginner-friendly method and doesn't require any additional software.

3. **Using JTAG/SWD (For Advanced Users):**
   - Advanced users and developers may opt for JTAG/SWD debugging and programming tools to upload firmware.
   - This method offers greater control and debugging capabilities but requires additional hardware and expertise.
   - You may configure another pico as a SWD Debugger called PicoProbe. See [Appendix A](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).
   - Here is a video of someone configuring and using the [PicoProbe](https://www.youtube.com/watch?v=0i2gLeBal9Y).

**In this lab, we will use method #1 (Drag and Drop).**

## **Optional:**
You can also use a VM which has everything preinstalled and configured. It is available in the repo with the installer and also the VM image. Please ensure that you have 40 GB of free space on your hard drive. Vmware fusion is for mac users and vmware workstation for windows users, download the correct version for your system. 

Once u have installed the VMware software, place the image in the virtual machines folder and click on scan to get it to recognise it. Then, click on the image and select 'Import'. Once the VM is imported, click on the VM and click on Power On. The password is password.

## **Troubleshotting:**
If you encounter a 'build not found' error, ensure that the Pico SDK is correctly cloned and initialised. If the issue persists, check the path in the CMakeLists.txt file.


## **THE BIG PICTURE**
The figure below illustrates the entire procedure.
<img width="1946" height="1071" alt="image" src="https://github.com/user-attachments/assets/22ae2238-e2c2-44bd-8dfe-8d28bafa4e48" />


## **TASK**

In this [basic code](basic.c) example, we embark on a journey to understand various types of operators in the C programming language. By executing this code, we gain insights into arithmetic, relational, logical, and bitwise operators, each playing a distinct role in manipulating and evaluating data.

## **EXERCISE**

This [blinky code](blinky.c) is supposed to blink an LED connected to the GPIO pin. The LED blinks at a rate determined by the "a" variable, which starts at 1 ms and __doubles__ with each iteration of the loop. When variable "a" reaches 2048ms, it resets to 1, creating an odd repeating LED blink pattern. The LED blink pattern must turn on and off with the same delay at __each loop iteration__. Could you identify where the errors are and make the necessary changes so that the code works as intended?

> [!IMPORTANT]
> Include a printf statement to monitor the variable "a". You might need to modify the CMake file to allow printf to work on the blink example. Refer to the CMake file in the Hello_World example for insights into this.

