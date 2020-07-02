SRCS = main.cpp load_data.hpp matrix.hpp utils.cpp utils.h conv.hpp conv_relu.hpp pooling.hpp pooling_affine.hpp relu.hpp affine.hpp dropout.hpp conv_dropout.hpp deep_cnn.hpp sigmoid.hpp softmax.hpp cnn.hpp adam.hpp pgmer.hpp
target:$(SRCS)
	g++ -fpermissive --std=c++11 -w $(SRCS)