all: libpaho-mqtt3cs.so.1.0


libpaho-mqtt3cs.so.1.0: paho.mqtt.c-master
	export CC='gcc -Wno-deprecated-declarations -Wno-implicit-function-declaration -Wformat=0 '; export prefix=$(shell pwd); make -C paho.mqtt.c-master
	mv paho.mqtt.c-master/build/output/*.so* .
	
paho.mqtt.c-master: paho.mqtt.c-master.zip
	unzip $^

clean:
	-rm -R *.so*
	-rm -Rf paho.mqtt.c-master
