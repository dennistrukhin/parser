cmake_minimum_required(VERSION 3.14)
project(parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(parser main.cpp Tokenizer.cpp Tokenizer.h TokenAccessor.cpp TokenAccessor.h TokenEnumerator.h Parser.cpp Parser.h RValue.cpp RValue.h KeywordMapper.cpp KeywordMapper.h)