DEFS 	  = -DOPENCV_SKIP
IFLAGS    = -I../OpenGRL/include
CXXFLAGS  = --std=c++17 -Wall -g -fopenmp -O3 $(DEFS) $(INCLUDE)
CXX       = g++
APP       = rdf_trainer
LFLAGS    = -lm -lopencv_core -pthread -lopencv_highgui -lopencv_imgproc
OBJ		  = RandomDecisionForest.o SegmentedDepthUtils.o
OBJ	     += main.o DecisionTree.o
THREADS   = 20

${APP}: ${OBJ}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LFLAGS}

main.o: main.cpp RandomDecisionForest.o
	${CXX} ${CXXFLAGS} -c $< -o $@

RandomDecisionForest.o: RandomDecisionForest.cpp RandomDecisionForest.h DecisionTree.o
	${CXX} ${CXXFLAGS} -c $< -o $@

DecisionTree.o: DecisionTree.cpp DecisionTree.h SegmentedDepthUtils.o
	${CXX} ${CXXFLAGS} -c $< -o $@

SegmentedDepthUtils.o: SegmentedDepthUtils.cpp SegmentedDepthUtils.h MathUtils.h
	${CXX} ${CXXFLAGS} -c $< -o $@

run:
	OMP_NUM_THREADS=${THREADS} ./rdf_trainer

clean:
	rm -f ${APP} *.o
