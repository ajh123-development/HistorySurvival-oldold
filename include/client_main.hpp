#pragma once

#ifndef MAIN_C_H
#define MAIN_C_H 1

#include <string>

#ifndef FLT_MAX
#define FLT_MAX 340282346638528859811704183484516925440.0f // Maximum value of a float, from bit pattern 01111111011111111111111111111111
#endif

int client_main(std::string addr, int port);

#endif
