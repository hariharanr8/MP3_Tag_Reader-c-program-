#ifndef TYPES_H
#define TYPES_H

#define ID3_HEADER_SIZE 10
typedef enum{
    failure,
    success
} Status;

typedef unsigned int Uint;

typedef enum{
    Operation_view,
    Operation_edit,
    Operation_help,
    type_error
} OperationType;

#endif