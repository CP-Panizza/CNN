SRCS = main.cpp load_data.hpp matrix.hpp utils.cpp utils.h conv.hpp conv_relu.hpp pooling.hpp pooling_affine.hpp relu.hpp affine.hpp softmax.hpp cnn.hpp adam.hpp pgmer.hpp

target:$(SRCS)
	g++ --std=c++11 -c $(SRCS)