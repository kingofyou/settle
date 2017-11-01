all:
	make -f BPlat.mak
	make -f BCommon.mak
	make -f cJSON.mak
clean release debug:
	make -f BPlat.mak $@
	make -f BCommon.mak $@
	make -f cJSON.mak $@
