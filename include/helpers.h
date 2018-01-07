#pragma once
#include "common.h"

int validate_ip(const char* ip);
int validate_port(int port);
char* get_ip_by_name(const char* hostname);
int set_data(char** data, const char* fileName);
