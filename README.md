# TL;DR
Skip to **How to use this project** to see how to get to the relevant files.

# Quad_SoC
Quadcopter controller based on Altera Cyclone-V. The motivation of multi-processor system is fault tolerance, from sensor failures (the project gets data from up to 3 IMUs) to tolerance in processing errors (e.g. radio communication is handled by separate processor, such that main processor makes decisions when the signal is bad, without the need to handle UART communication and correction sum calculation).

# Purpose of the repo
This repo does not include 1.5 GB Quartus II project! Instead, this repo contains the hardware description files (VHDL) and software (C/C++) to teach how to deploy multi-processor system on Cyclone-V. Therefore, this repo is brief and well documented and should be of use to anyone that uses Cyclone-V (not just quadcopters/drones) to leverage the processing power of embedded platforms. Although, the Quartus II project, which is based on freerly available Golden Hardware Reference Design (GHRD) for Cyclone-V, can be shared via link, once contacted at garaj.martin@gmail.com.



# How to use this project
For **TL;DR**, to navigate through this repo, use the diagram below and link to the files to get what you need. The code within the files is well documented (_grey round boxes_) and should be sufficient to understand the main aspects of the code together with documentation files (_blue round boxes_).

###### Cyclone-V internal structure with documentation
![./Documentation/Quad_SoC_hardware_software_v2.png](./Documentation/Quad_SoC_hardware_software_v2.png?raw=true "Quad SoC structure and components")

###### Source files
[Quad_SoC_v1_0/src/main.cpp](./HPS/Quad_SoC_v1_0/src/main.cpp): main file with main loop running in ARM under Linux
  - initialize FPGA components and then hand-off the control to Nios II co-processors
  - handles 2-way communication with co-processors through FPGA components _aux_radio_0_mem_ and _aux_imu_0_mem_
    - sets co-processor control registers
    - polls co-processor registers
    - reads information processed by co-processors from memory
  - performs high level data processing (navigation, PID control, etc)

[API_Interconnect.hpp](./HPS/Quad_SoC_v1_0/src/FPGA_Peripherals/API_Interconnect/API_Interconnect.hpp): object for mapping virtual address space provided by Linux, to hardware address space of FPGA components (hardware addresses are assigned in Qsys)
  - software representation of L3 Interconnect -> HPS-to-FPGA Bridge (HPS is a master to this communication)
  - good example of properly handling virtual address conversion to FPGA address space
  - NOTE: FPGA-to-HPS (FPGA is master) is not utilized, since this is handled by interrupts, which are notoriously difficult to handle in Linux

[hps_0.h](./HPS/Quad_SoC_v1_0/src/system_headers/hps_0.h): file containing physical address space of FPGA components
  - generated by _winfo2header_ utility from [soc_system.sopcinfo](./FPGA/GHRD/soc_system.sopcinfo)

[AUX_RADIO_constants.h](./FPGA/GHRD/software/aux_radio_0/src/AUX_RADIO_constants.h): well documented memory map of FPGA component _aux_radio_0_mem_

[GHRD/software/aux_radio_0/src/main.c](./FPGA/GHRD/software/aux_radio_0/src/main.c): main loop running on Nios II co-processor _aux_radio_0_

[AUX_IMU_constants.h](./FPGA/GHRD/software/aux_imu_0/src/AUX_IMU_constants.h): well documented memory map of FPGA component _aux_imu_0_mem_

[GHRD/software/aux_imu_0/src/main.c](./FPGA/GHRD/software/aux_imu_0/src/main.c): main loop running on Nios II co-processor _aux_imu_0_

[API_PWM_module.hpp](./HPS/Quad_SoC_v1_0/src/FPGA_Peripherals/API_PWM_module/API_PWM_module.hpp): object for controlling PWM FPGA component from Linux

[API_UART_module.hpp](./HPS/Quad_SoC_v1_0/src/FPGA_Peripherals/API_UART_module/API_UART_module.hpp): object for controlling UART FPGA component from Linux

[API_I2C_module.cpp](./HPS/Quad_SoC_v1_0/src/FPGA_Peripherals/API_I2C_module/API_I2C_module.cpp): object for controlling I2C FPGA component from Linux

[MPU6050_DMP_arbiter.c](./FPGA/GHRD/software/aux_imu_0/src/MPU6050_DMP_arbiter.c): Nios II co-processor functions to handle the communication with MPU6050 

###### Documentation files
[QUADCOPTER_documentation.docx](./Documentation/QUADCOPTER_documentation.docx): the main file documenting the development of this project

[Qsys.png](./Documentation/Qsys.png): a screenshot of Qsys (System integration tool) depicting the connections of FPGA components

[soc_system.html](./Documentation/soc_system.html): auto-generated documentation for FPGA layout/Qsys view

[HOW_TO_NiosII_boot_from_On-Chip_Ram.docx](./Documentation/HOW_TO/HOW_TO_NiosII_boot_from_On-Chip_Ram.docx): documents how to make the Linux configure the FPGA (load the FPGA components) during boot (_otherwise_, the FPGA part of SoC needs to be loaded through USB Blaster II every single time after hard-reset)

[HOW_TO_wishbone_interface.docx](./Documentation/Avalon_i2c_master/HOW_TO_wishbone_to_avalon_interface/_NOTES/HOW_TO_wishbone_interface.docx): documents how to go from _Wishbone_ interface (generic interface for OpenCores FPGA components) to proprietary _Avalon_ interface (Alteras HPS-to-FPGA Bridge interface)
  - it is very useful, when one can generate a VHDL wrapper, that handles the translation of _Wishbone_ to _Avalon_, since then one can create a trully integrated components using high-level tools like Qsys
  - the VHDL [wishbone-to-avalon wrapper](./FPGA/GHRD/source_files/avalon_i2c_master/avalon_i2c_master.vhd) for i2c FPGA component

[checksum_testing.docx](./Documentation/testing/iBus/checksum_testing.docx): guidline on how to calculate checksum for packets sent through Flysky iBus protocol
  - the checksum calculation is implemented in C and can be found in [aux_radio_0/src/main.c](FPGA/GHRD/software/aux_radio_0/src/main.c) [line 185](https://github.com/martin-garaj/quad_soc/blob/e8f651f6a8f62508dc0a7bcb314ce51ea4f3d151/FPGA/GHRD/software/aux_radio_0/src/main.c#L185)

[FS-iA10B_iBus_TX_data.txt](./Documentation/testing/iBus/FS-iA10B_iBus_TX_data.txt): matching the hex-values to radio channel values ontained from UART communication from [Flysky FS-iA10B](./Documentation/Flysky_FS-IA10B.png)




# Hardware
The repo is intended to run on Atlas-SoC board with Cyclone-V ARM-FPGA chip. Furthermore, there is a custom PCB of a Quad Support Board which holds Radio Flysky FS-iA10B and up to 4x Inertial Measurement Units MPU 6050. The hardware components are depicted below, so one can check whether, for instance, the newer version of the product has compatible pinout (especially the MPU 6050).
###### Atlas-SoC board (top)
![./Documentation/Board-Top.jpg](./Documentation/Board-Top.jpg?raw=true "Atlas-SoC board (top)")

###### Custom support board to house IMUs and Radio
Custom PCB board that fits on top of the Atlas-SoC board. It provides a simple way to hold all components firmly in place, which was found very usefull when debugging the UART and I2C communication, since the issues with improper connections is eliminated. 
![./Quad_support_board/pcb.png](./Quad_support_board/pcb.png?raw=true "Quad Support Board")

###### Inertial measurement unit (IMU) MPU 6050
NOTE: mind the pinout of the MPU 6050 board.
![./Documentation/PMU6050.png](./Documentation/PMU6500.png?raw=true "MPU 6050")

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
  - aux radio
    - Nios II co-processor responsible for handling communication with external radio transciever through UART
  - aux imu
    - Nios II co-processor responsible for managing and collecting data from up to 4 IMUs through I2C
The content of FPGA, connections in FPGA fabric, GPIO connections and external components are shown below.

###### Cyclone-V internal structure
![./Documentation/Quad_SoC_hardware_software.png](./Documentation/Quad_SoC_hardware_software.png?raw=true "Quad SoC structure and components")

[i2c_0](FPGA/GHRD/source_files/avalon_i2c_master/i2c_master_top.vhd): VHDL source code for i2c FPGA component (all credit goes to the author **Richard Herveille**)

[wishbone-to-avalon wrapper](./FPGA/GHRD/source_files/avalon_i2c_master/avalon_i2c_master.vhd): VHDL wrapper to translate _Wishbone_ to _Avalon_ interface

[pwm_0](./FPGA/GHRD/source_files/avalon_pwm/avalon_PWM.vhd): VHDL file of the PWM FPGA component

[package_common.vhd](FPGA/GHRD/source_files/package_common.vhd): useful functions for VHDL programming, for instance binary operations require _ceil_log2_ function


