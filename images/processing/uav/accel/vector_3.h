//
// Created by Liam Kelly on 2/26/19.
//

#ifndef PROJECT_VECTOR_3_H
#define PROJECT_VECTOR_3_H


class Vector3 {

public:

    Vector3();
    ~Vector3();

    float x;
    float y;
    float z;

    void copy_vals(Vector3 &to_copy);

};


#endif //PROJECT_VECTOR_3_H
