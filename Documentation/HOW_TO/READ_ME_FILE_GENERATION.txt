
	1) generate hps_0.h

		For some reason the "sopc-create-header-files" the command is not found. The command is in "/cygdrive/c/PROGRAMS/Quartus/quartus/sopc_builder/bin/".
		The path can be included by running the "Altera Embedded Command Shell" with following command :

			mgaraj2@DESKTOP-BQTK0L1 /cygdrive/c/PROJECTS/Quad_SoC/GHRD_quad_v0_7/q_GHRD
			$ export PATH=$PATH:/cygdrive/c/PROGRAMS/Quartus/quartus/sopc_builder/bin/

		Only then the "./generate_hps_qsys_header.sh" can be run :

			mgaraj2@DESKTOP-BQTK0L1 /cygdrive/c/PROJECTS/Quad_SoC/GHRD_quad_v0_7/q_GHRD
			$ ./generate_hps_qsys_header.sh

		Output upon success :

			swinfo2header: Creating macro file 'hps_0.h' for module 'hps_0'

	2) update the <project_name>_bsp path
		
		Navigate to <Quartus_project_path>/software/nios_0_bsp/settings.bsp . Open the settings.bsp, edit :
		<BspGeneratedLocation><Quartus_project_path>\software\q_nios_0_bsp</BspGeneratedLocation> XML node.

	3) generate nios memory initialization .HEX

			Run "Altera Nios2 Command Shell". 
			Make sure the <project_name>_bsp is up-to-date,	by right-clicking it, Nios II->Generate BSP.
			Navigate to the /cygdrive/c/<Quartus_project_path>/software/nios_0 .
			Generate initialization Nios memoty files for the Quartus II project.
		
				$ make mem_init_install

			Repeat the same process for every Nios requiring memory initialization.

			Output upon success :
			
				Post-processing to create mem_init/hdl_sim/soc_system_<qsys_memory_component_name>.sym...
				nios2_elf_nm -n <nios_ii_processor_name>.elf > mem_init/hdl_sim/soc_system_<qsys_memory_component_name>.sym

			Check the Qsys memory components to point to the gemerated .HEX files in :
			<Quartus_project_path>/software/<nios_ii_processor_name>/mem_init/soc_system_<qsys_memory_component_name>.hex .
			Run "Compile Design" in Quartus II.

		ALTERNATIVE OPTION

			Run "Nios II 16.0 Software Build Tools for Eclipse". 
			Make sure the <project_name>_bsp is up-to-date,	by right-clicking it, Nios II->Generate BSP.

			Right-click the <project_name> folder, Make Targets->Build...->"select mem_init_generate"->"hit [Build] button"

			Check the Qsys memory components to point to the gemerated .HEX files in :
			<Quartus_project_path>/software/<nios_ii_processor_name>/mem_init/soc_system_<qsys_memory_component_name>.hex .
			Run "Compile Design" in Quartus II.

		ALTERNATIVE OPTION

			Generate the initialization files, in of the the 2 above described ways. Copy the files from 
			<Quartus_project_path>/software/<nios_ii_processor_name>/mem_init/ to <Quartus_project_path>/ .
			Then in Quartus II -> Processing -> Update Memory Initialization File . 

			ADVANTAGE

			The Qsys doesnt need to "point" to specific location. Instead of including the path to the initialization file, 
			the component in Qsys has the name of the hex file, in format <qsys_memory_component_name>.hex .
			This means there is no absolute path pointing to a memory initialization file.

	4) GENERATE fpga initialization file .rbf

		Quartus II->File->Convert Programming Files ...
		
		Window:
			Output programming file:
				Programming file type: Raw Binary File (.rbf)
				Mode: Passive Parallel x8
				File name: de_nano_soc.rbf
			Input files to convert:
				SOF Data->Add File...->"Select soc_system.sof"
				SOF Data->Properties->"tick Compression"

		Hit [Generate] button.
		Copy the file to the SD card.
			