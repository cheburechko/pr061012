CFLAGS = -g -Wall -std=c++0x

VIEWPATH =  ../../view/
MODELPATH = ../../model/

COBJS  = main.cpp
COBJS += $(MODELPATH)World/Object/Object.cpp
COBJS += $(MODELPATH)World/Action/Action.cpp
COBJS += $(MODELPATH)God/God.cpp
COBJS += $(MODELPATH)Utilities/Shape/Shape.cpp
COBJS += $(MODELPATH)World/Indexator/Indexator.cpp
COBJS += $(MODELPATH)World/IWorld.cpp
COBJS += $(MODELPATH)World/World.cpp
COBJS += $(MODELPATH)World/ObjectHeap/ObjectHeap.cpp
COBJS += $(MODELPATH)World/Object/Building/Building.cpp
COBJS += $(MODELPATH)World/Object/Weather/Weather.cpp
COBJS += $(MODELPATH)World/Object/Resource/Resource.cpp
COBJS += $(MODELPATH)World/Object/Tool/Tool.cpp
COBJS += $(MODELPATH)World/ParamArray/ParamArray.cpp
COBJS += $(MODELPATH)World/ParamArray/EParamArrayBadKey.cpp
COBJS += $(MODELPATH)World/ObjectFactory/ObjectFactory.cpp
COBJS += $(MODELPATH)Utilities/Point/Point.cpp
COBJS += $(VIEWPATH)WorldRenderer.cpp
LIBRARIES = ../View/


OBJECTS = $(COBJS:.cpp=.o)



all: $(COBJS)
	g++ $(CFLAGS) $(COBJS) -L"$(LIBRARIES)" -lGL -lGLU -lglfw -lSOIL -o test


clean:
	rm -f $(OBJECTS)
