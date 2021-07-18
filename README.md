# Quad_SoC
Quadcopter controller based on Altera Cyclone-V. The motivation of multi-processor system is fault tolerance, from sensor failures (the project gets data from up to 3 IMUs) to tolerance in processing errors (e.g. radio communication is handled by separate processor, such that main processor makes decisions when the signal is bad, without the need to handle UART communication and correction sum calculation).

# Purpose of the repo
This repo does not include 1.5 GB Quartus II project! Instead, this repo contains the hardware description files (VHDL) and software (C/C++) to teach how to deploy multi-processor system on Cyclone V. Therefore, this repo is brief and well documented and should be of use to anyone that uses Cyclone V (not just quadcopters/drones) to leverage the processing power of embedded platforms. Although, the Quartus II project, which is based on freerly available Golden Hardware Reference Design (GHRD) for Cyclone-V, can be shared via link, once contacted at garaj.martin@gmail.com.

# Hardware
The repo is intended to run on Atlas-SoC board with Cyclone-V ARM-FPGA chip. The board is depicted below.
###### Atlas-SoC board (top)
![./Documentation/Board-Top.jpg](./Documentation/Board-Top.jpg?raw=true "Atlas-SoC board (top)")
###### Atlas-SoC board (bottom)
![./Documentation/Board-Bottom.jpg](./Documentation/Board-Bottom.jpg?raw=true "Atlas-SoC board (bottom)")

# Cyclone-V
Cyclone-V from Altera, or Zinq-7000 Xilinx (the only competitor at the moment), is a hybrid architecture integrating ARM core(s) and FPGA fabric coupled with Interconnect on single System-on-Chip (SoC) device. The internal architerure is depicted below.


