# Derek Schacht
#  2016 10 22
#  License : Give me credit where it is due.
#  Disclaimer : I try and cite code that I find on the internet but I am not 
#               perfect. If you find something that should be cited let me know
#               and I will update my code.
#  Warranty   : Absolutely None
#
#  This header also applies to all previous commits. But, I reserve the right 
#  to modify this in the future.
#

# Router Attempt.... (doesn't really work that well.. it runs but seems unstable)
#PATH=/home/dschacht/extraDrive/toolchains/toolchain-mipsel_4.1.1_BRCM24/bin:/usr/bin/

# RasPi
#PATH=$PATH:/home/dschacht/buildroot/buildroot-2016.05/output/host/usr/bin/

# RasPi3
#PATH=$PATH:/home/dschacht/buildroot2017toolchain/buildroot-2017.02.09/output/host/usr/bin/

CPP_FILES=data_manager.cpp \
core/receive_thread.cpp \
core/weather_thread.cpp \
core/status_thread.cpp \
utilities/property_reader.cpp \
utilities/udp_interface.cpp \
utilities/tcp_client.cpp \
utilities/system_utilities.cpp \
utilities/system_message.cpp \
utilities/numerical_statistics.cpp \
utilities/http_client.cpp \
file/csv_interface.cpp \
house/house_interface.cpp \
data/global_queue_list.cpp \
data/global_properties.cpp


LIBRARY_INC=-lrt -lpthread

#VPATH=numerical_statistics.cpp:../../libraries/Numerical_Statistics/
SEARCH=-I./ -I./core -I./data -I./file -I./house -I./types -I./utilities
#SEARCH=-I../../libraries/Numerical_Statistics/ \
#-I../Common

default: host

date:
	@echo "#ifndef BUILD_DATE_H\n\
#define BUILD_DATE_H\n\
static const char* BUILD_DATE = \"`date`\";\n\
#endif" > build_date.h

target: date
	#export PATH=$(PATH):/home/dschacht/buildroot/buildroot-2016.05/output/host/usr/bin/
	arm-raspi-linux-uclibcgnueabihf-g++ -std=c++0x -O2 -o weather-target3 $(SEARCH) -I../Common $(FOLDERS) $(CPP_FILES) $(LIBRARY_INC)

raspi3 : date
	#export PATH=$(PATH):/home/dschacht/buildroot2017toolchain/buildroot-2017.02.09/output/host/usr/bin/
	arm-buildroot-linux-gnueabihf-g++ -std=c++0x -O2 -o weather-target3 $(SEARCH) -I../Common $(FOLDERS) $(CPP_FILES) $(LIBRARY_INC)

host: date
	g++ -std=c++0x -O2 -o weather-host3 $(SEARCH) $(CPP_FILES) $(LIBRARY_INC)

install:
	cp weather-target3 ~/buildroot/overlay/root
