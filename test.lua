#! /usr/bin/env lua

local md5 = require("cmd5")

local path = "./cmd5.c"
local md5sum = md5.file_sum(path)
print(md5sum)
