#pragma once


#include <Arduino.h>
#include "scheduler.hpp"


#ifndef __TINBES03__ARDUINOS__INSTRUCTIONS__
#define __TINBES03__ARDUINOS__INSTRUCTIONS__


uint8_t execute(byte instruction, struct task_s *task);


#endif