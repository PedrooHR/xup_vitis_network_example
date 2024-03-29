# /*******************************************************************************
#  Copyright (C) 2021 Advanced Micro Devices, Inc
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
# *******************************************************************************/

DEVICE=xcu55c-fsvh2892-2L-e
IPS=m2s_send s2m_recv s2s_recv s2s_send matadd_hw matmul_hw matnorm_hw
TARGET=ip

all: $(IPS) 

m2s_send: build_sr.tcl src/m2s_send.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) m2s_send

s2m_recv: build_sr.tcl src/s2m_recv.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) s2m_recv

s2s_recv: build_sr.tcl src/s2s_recv.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) s2s_recv

s2s_send: build_sr.tcl src/s2s_send.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) s2s_send

matadd_hw: build_app.tcl application/matadd_hw.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) matadd_hw

matmul_hw: build_app.tcl application/matmul_hw.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) matmul_hw

matnorm_hw: build_app.tcl application/matnorm_hw.cpp
	vitis_hls $< -tclargs $(TARGET) $(DEVICE) matnorm_hw

clean:
	rm -rf *.xclbin*
	rm -rf _x
	rm -rf .ipcache
	rm -rf build_sr
	rm -rf *.xo
	rm -rf *.log
	rm -rf *.ltx
	rm -rf .Xil
