//
// @author raver119@gmail.com
//

#ifndef LIBND4J_VARIABLE_H
#define LIBND4J_VARIABLE_H


#include <NDArray.h>
#include <graph/generated/node_generated.h>
#include <graph/generated/graph_generated.h>

namespace nd4j {
    namespace graph {
        template <typename T>
        class Variable {
        protected:
            NDArray<T> * _ndarray;

            bool _external;
            bool _readOnly;

        public:
            Variable(NDArray<T> *array);

            Variable(const nd4j::graph::FlatVariable *flatVariable);
            ~Variable();
        };
    }
}

template <typename T>
nd4j::graph::Variable<T>::Variable(NDArray<T> *array) {
    _ndarray = array;

    _external = false;
    _readOnly = false;
}

template <typename T>
nd4j::graph::Variable<T>::Variable(const nd4j::graph::FlatVariable *flatVariable) {
    int shapeLen = flatVariable->shape()->Length();
    int *shape = new int[shapeLen];

    _external = true;
    _readOnly = false;

#pragma omp simd
    for (int e = 0; e < shapeLen; e++) {
        shape[e] = flatVariable->shape()->Get(e);
    }

    int bufLen = flatVariable->values()->Length();
    T *buffer = new T[bufLen];

#pragma omp simd
    for (int e = 0; e < bufLen; e++) {
        buffer[e] = (T) flatVariable->values()->Get(e);
    }

    _ndarray = new NDArray<T>(buffer, shape);
    _ndarray->_allocated = true;
}

template <typename T>
nd4j::graph::Variable<T>::~Variable() {
    if (_ndarray != nullptr)
        delete _ndarray;
}

#endif //LIBND4J_VARIABLE_H
