UVM_HOME=/comelec/softs/opt/mentor/modelsim/v10.5c/modeltech/verilog_src/uvm-1.1d
# need to set for ld error ->  LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
QUESTA_UVM_PKG=/comelec/softs/opt/mentor/modelsim/v10.4c/modeltech/verilog_src/questa_uvm_pkg-1.1d/src/
com:
	vlog +incdir+${UVM_HOME}/src dut_transaction.sv

wo_dpi:
	vlog  +define+QUESTA +incdir+${UVM_HOME}/src +define+UVM_NO_DPI +define+QUESTA +acc dut.sv tb.sv 

#when using uvm 1.2, quest_uvm pkg needs to be compiled separately
w_dpi:
	vlog  +define+QUESTA +incdir+${UVM_HOME}/src +acc avalon_m_if.sv dut.sv tb.sv
	mkdir -p lib
	g++ -m64 -fPIC -DQUESTA -g -W -shared -I/comelec/softs/opt/mentor/modelsim/v10.4c/modeltech/include/ ${UVM_HOME}/src/dpi/uvm_dpi.cc -o lib/uvm_dpi64.so
	#vlog +incdir+${UVM_HOME}/src +incdir+${QUESTA_UVM_PKG} ${QUESTA_UVM_PKG}/questa_uvm_pkg.sv
	#vsim -uvmcontrol=all   +uvm_set_config_int=*,recording_detail,400 +define+QUESTA -sv_seed random -solvefaildebug -novopt -sv_lib ./lib/uvm_dpi64  +UVM_TESTNAME=av_test tb
uvm11d:
	vlog  +define+QUESTA  +acc avalon_m_if.sv dut.sv tb.sv
sim:
	vsim -uvmcontrol=all   +uvm_set_config_int=*,recording_detail,400 +define+QUESTA -sv_seed random -solvefaildebug  +UVM_TESTNAME=av_test tb
qv:
	qverilog -uvmcontrol=all  +uvm_set_config_int=*,recording_detail,400 +define+QUESTA +UVM_TESTNAME=av_test avalon_m_if.sv dut.sv tb.sv
	#vsim -uvmcontrol=all  +uvm_set_config_int=*,recording_detail,400 +define+QUESTA +UVM_TESTNAME=av_test tb


build:
	vlog wb_slave_agent/wb_if.sv
	vlog +incdir+./wb_slave_agent  ./wb_slave_agent/wb_slave_agent_pkg.sv
	vlog ./av_master_agent/avalon_m_if.sv
	vlog +incdir+./av_master_agent  ./av_master_agent/av_test_pkg.sv
	vlog env_pkg.sv 
	vlog +acc ../RTL/av2wb1.sv tb.sv
sim:
	vsim \
	-c +uvm_set_config_int=\*,recording_detail,400 -uvmcontrol=all +define+QUESTA -sv_seed random \
	-solvefaildebug  \
	+UVM_VERBOSITY=UVM_DEBUG \
	tb \
	-sv_seed 3138956347 \
	-do "run -all"
	#fail 3053001672 \
	#pass -sv_seed seed 3138956347

clean:
	rm -rf *.wlf transcript work *.xml
