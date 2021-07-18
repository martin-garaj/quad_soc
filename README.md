# TL;DR
Skip to **How to use this project** to see how to navigate through the files and go directly to files that are relevant for you.

# Quad_SoC
Quadcopter controller based on Altera Cyclone-V. The motivation of multi-processor system is fault tolerance, from sensor failures (the project gets data from up to 3 IMUs) to tolerance in processing errors (e.g. radio communication is handled by separate processor, such that main processor makes decisions when the signal is bad, without the need to handle UART communication and correction sum calculation).

## Purpose of the repo
This repo does not include 1.5 GB Quartus II project! Instead, this repo contains the hardware description files (VHDL) and software (C/C++) to teach how to deploy multi-processor system on Cyclone V. Therefore, this repo is brief and well documented and should be of use to anyone that uses Cyclone V (not just quadcopters/drones) to leverage the processing power of embedded platforms. Although, the Quartus II project, which is based on freerly available Golden Hardware Reference Design (GHRD) for Cyclone-V, can be shared via link, once contacted at garaj.martin@gmail.com.

## Hardware
The repo is intended to run on Atlas-SoC board with Cyclone-V ARM-FPGA chip. Furthermore, there is a custom PCB of a Quad Support Board which holds Radio Flysky FS-iA10B and up to 4x Inertial Measurement Units MPU 6500. The hardware components are depicted below, so one can check whether, for instance, the newer version of the product has compatible pinout (especially the MPU 6500).
###### Atlas-SoC board (top)
![./Documentation/Board-Top.jpg](./Documentation/Board-Top.jpg?raw=true "Atlas-SoC board (top)")

[//]: <> ###### Atlas-SoC board (bottom)
[//]: <> ![./Documentation/Board-Bottom.jpg](./Documentation/Board-Bottom.jpg?raw=true "Atlas-SoC board (bottom)")

###### Custom support board to house IMUs and Radio
![./Quad_support_board/pcb.png](./Quad_support_board/pcb.png?raw=true "Quad Support Board")
###### Inertial measurement unit (IMU) MPU 6500
![./Documentation/PMU6500.png](./Documentation/PMU6500.png?raw=true "MPU 6500")
###### Radio Flysky FS-iA10B
The pins for UART communication are highligted (UART TX (cyan) and RX (green))
![./Documentation/Flysky_FS-IA10B.png](./Documentation/Flysky_FS-IA10B.png?raw=true "Flysky FS-iA10B")

## Cyclone-V
Cyclone-V from Altera, or Zinq-7000 Xilinx (the only competitor at the moment), is a hybrid architecture integrating ARM core(s) and FPGA fabric coupled with Interconnect on single System-on-Chip (SoC) device. The internal architerure is depicted below.

###### Cyclone-V internal structure
![./Documentation/Altera-Cyclone-V-SoC.png](./Documentation/Altera-Cyclone-V-SoC.png?raw=true "Cyclone-V internal structure")


# Quad SoC on Cyclone-V
The Quad SoC uses both the ARM and FPGA to implement **multi-processor** system. 
- ARM
  - executes high level functions provided the data from Nios II co-processors
  - control software is running in Linux with underlying custom communication layer _control software -> Linux -> FPGA -> Nios II co-processors_ and vice-versa
- FPGA
  - AUX radio
    - Nios II co-processor responsible for handling communication with external radio transciever through UART
  - AUX imu
    - Nios II co-processor responsible for managing and collecting data from up to 4 IMUs through I2C
The content of FPGA, connections in FPGA fabric, GPIO connections and external components are shown below.

###### Cyclone-V internal structure
![./Documentation/Quad_SoC_hardware_software.png](./Documentation/Quad_SoC_hardware_software.png?raw=true "Quad SoC structure and components")


# How to use this project
For **TL;DR**, the Quartus II project has 1.5 GB and is based on default Golden Hardware Reference Design (GHRD) for Cyclone-V. Therefore, this repo includes only the custom files, that make the magic happen as well as handy documentation and solution to issues that I faced when developing this project. Tonavigate through this repo, use the diagram below and link to the files to get what you need. The code within the files is well documented (_grey round boxes_) and should be sufficient to understand the main aspects of the code together with documentation files (_blue round boxes_).

###### Cyclone-V internal structure
![./Documentation/Quad_SoC_hardware_software_v2.png](./Documentation/Quad_SoC_hardware_software_v2.png?raw=true "Quad SoC structure and components")


###### Files
[Qsys.png](./Documentation/Qsys.png)
