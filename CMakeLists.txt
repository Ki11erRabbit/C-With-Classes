cmake_minimum_required(VERSION 3.23)
project(CWithClasses)

set(CMAKE_CXX_STANDARD 11)

add_executable(CWithClasses main.cpp Parser.h Token.h Scanner.h Method.h CodeBlock.h Parameter.h Class.h Struct.h Enum.h Function.h CwithClasses.h)
